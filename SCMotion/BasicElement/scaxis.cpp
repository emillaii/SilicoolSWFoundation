#include "scaxis.h"

SCAxis::Direction SCAxis::oppositeDir(SCAxis::Direction dir)
{
    if (dir == Positive)
    {
        return Negative;
    }
    else
    {
        return Positive;
    }
}

SCAxis::SCAxis(QString name, QObject *parent) : QObject(parent)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<SCAxis>(name, this);
}

QVariant SCAxis::invoke(const QString &cmd, const QVariantList &args)
{
    Instruction ins(this, cmd, args);
    return ins.execute();
}

void SCAxis::needReHome()
{
    m_hasHome = false;
}

void SCAxis::init()
{
    qCDebug(motionCate(), N_FUNC);

    if (moveProtection == nullptr)
    {
        throw SilicolAbort(tr("MoveProtection is not set!"), EX_LOCATION);
    }
    if (m_config == nullptr)
    {
        throw SilicolAbort(QString("Axis config was not set! Name: %1").arg(name()), EX_LOCATION);
    }
    if (m_config->advancedAxisConfig()->useSoftLimit() && m_config->positiveLimit() < m_config->negativeLimit())
    {
        throw SilicolAbort(tr("Positive limit must greater than negativeLimit! AxisName: %1").arg(name()));
    }
    auto advancedConfig = m_config->advancedAxisConfig();
    m_homeExecutor = advancedConfig->homeExecutor();
    if (m_homeExecutor == AdvancedAxisConfig::Driver || m_homeExecutor == AdvancedAxisConfig::DriverThenController)
    {
        homeDoneDi = MotionElementContainer::getIns()->getItem<SCDI>(advancedConfig->homeDoneDiName());
        startHomeDo = MotionElementContainer::getIns()->getItem<SCDO>(advancedConfig->startHomeDoName());
    }
    if (advancedConfig->homeSettlingPricision() > 0 && advancedConfig->homeSettlingWindow() < 2)
    {
        throw SilicolAbort(tr("homeSettlingWindow must greater than 1 if homeSettlingPricision greater than 0! AxisName: %1").arg(name()));
    }
    if (advancedConfig->settlingByAppLayer() && advancedConfig->settlingWindow() < 2)
    {
        throw SilicolAbort(tr("settlingWindow must greater than 1 if settlingByAppLayer! AxisName: %1").arg(name()));
    }
    initImpl();
    if (hasAlarm())
    {
        clearAlarm();
    }
    enable();
    resetMaxVel();
    resetMaxAcc();
    if (!isConnectConfigChangedSignal)
    {
        connect(m_config, &AxisConfig::velocityRatioChanged, this, &SCAxis::onConfigVelChanged);
        connect(m_config, &AxisConfig::maxVelChanged, this, &SCAxis::onConfigVelChanged);
        connect(m_config, &AxisConfig::maxAccChanged, this, &SCAxis::onConfigAccChanged);
        connect(m_config->advancedAxisConfig(), &AdvancedAxisConfig::homeExecutorChanged, this, &SCAxis::onHomeExecutorChanged);
        isConnectConfigChangedSignal = true;
    }
    m_isInit = true;
}

void SCAxis::home(bool waitDone)
{
    if (m_homeExecutor == AdvancedAxisConfig::ClearPos)
    {
        qCInfo(motionCate()) << tr("%1 home, just clear pos...").arg(name());
        clearPosImpl();
        return;
    }

    qCInfo(motionCate(), N_FUNC_ARG(waitDone));

    GlobalState::getIns().waitNotPaused(name());

    m_hasHome = false;
    startHome();
    currentTargetPos = 0;
    m_hasStop = false;
    if (waitDone)
    {
        waitHomeDone();
    }
}

void SCAxis::waitHomeDone()
{
    if (m_homeExecutor == AdvancedAxisConfig::ClearPos)
    {
        return;
    }

    if (m_homeExecutor == AdvancedAxisConfig::Driver)
    {
        waitDriverHomeDone();
    }
    else
    {
        waitControllerHomeDone();
    }
}

double SCAxis::getFeedbackPos(int cacheInvalidationTime)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastUpdateFeedbackPosTimeStamp > cacheInvalidationTime)
    {
        lastFeedbackPos = round(getFeedbackPosImpl() * 100000) / 100000;
        lastUpdateFeedbackPosTimeStamp = now;
    }
    return lastFeedbackPos;
}

void SCAxis::enable()
{
    qCDebug(motionCate(), N_FUNC);

    enableImpl();
    QThread::msleep(100);
    syncProfilerAndFeedbackPos();
    m_isEnable = true;
}

void SCAxis::disable()
{
    qCDebug(motionCate(), N_FUNC);

    if (isRunning())
    {
        qCWarning(motionCate()) << tr("Disable %1 while running! Try to stop axis first!").arg(name());
        stop();
        QElapsedTimer timer;
        timer.start();
        while (isRunning())
        {
            if (timer.elapsed() > 300)
            {
                break;
            }
            QThread::msleep(10);
        }
    }
    else
    {
        m_hasStop = true;    // If someone calling waitArrivedPos, set hasStop to notify caller
    }
    disableImpl();
    m_isEnable = false;
    m_hasHome = false;
}

void SCAxis::clearAlarm()
{
    disable();
    QThread::msleep(100);
    clearErrorImpl();
    QThread::msleep(100);
    enable();
}

void SCAxis::stop()
{
    m_hasStop = true;
    stopImpl();
    if (m_homeExecutor == AdvancedAxisConfig::Driver || m_homeExecutor == AdvancedAxisConfig::DriverThenController)
    {
        if (startHomeDo != nullptr)
        {
            startHomeDo->set(false);
        }
    }
}

void SCAxis::stopAndResetState()
{
    stop();
    currentTargetPos = getCurrentOutputPos();
    resetMaxVel();
}

void SCAxis::absMoveWithLimit(double targetPos)
{
    if (m_config->advancedAxisConfig()->useSoftLimit())
    {
        getPosInLimitRange(targetPos);
    }
    absMove(targetPos);
}

void SCAxis::relMoveWithLimit(double step)
{
    qCInfo(motionCate(), N_FUNC_ARG(step));

    double targetPos = getCurrentOutputPos() + step;
    absMoveWithLimit(targetPos);
}

bool SCAxis::isInLimitRange(double targetPos) const
{
    if (!m_config->advancedAxisConfig()->useSoftLimit())
    {
        return true;
    }
    return targetPos <= positiveLimit() && targetPos >= negativeLimit();
}

void SCAxis::absMove(double targetPos, bool waitDone, double precision, int timeout)
{
    if (isProfilerAtPos(targetPos, MinStep))
    {
        currentTargetPos = targetPos;
        return;
    }

    if (isinf(targetPos) || isnan(targetPos))
    {
        throw SilicolAbort(QString("Invalid target pos: %1").arg(targetPos), EX_LOCATION);
    }

    qCInfo(motionCate(), N_FUNC_ARG(targetPos, waitDone, precision));

    GlobalState::getIns().waitNotPaused(name());

    absMoveImpl(targetPos);
    m_hasStop = false;
    if (waitDone)
    {
        waitArrivedPos(targetPos, precision, timeout);
    }
}

void SCAxis::relMove(double step, bool waitDone, double precision, int timeout)
{
    qCInfo(motionCate(), N_FUNC_ARG(step, waitDone, precision));

    double targetPos = getCurrentOutputPos() + step;
    absMove(targetPos, waitDone, precision, timeout);
}

void SCAxis::stepMove(double step, bool waitDone, double precision, int timeout)
{
    if (qFuzzyCompare(step, 0))
    {
        return;
    }

    qCInfo(motionCate(), N_FUNC_ARG(step, waitDone, precision));

    double targetPos = getCurrentOutputPos() + step;
    GlobalState::getIns().waitNotPaused(name());

    ErrorHandler::tryToHandleError([&] {
        checkState(false);
        checkIsRunning();
        if (nextMoveVelRatio > 0)
        {
            scaleMaxVel(nextMoveVelRatio);
        }
        if (nextMoveAccRatio > 0)
        {
            scaleMaxAcc(nextMoveAccRatio);
        }
        currentTargetPos = targetPos;
        axisMoveTimer.start();
        moveToImpl(targetPos);
    });
    m_hasStop = false;
    if (waitDone)
    {
        waitArrivedPos(targetPos, precision, timeout);
    }
}

void SCAxis::tryToMove(double targetPos, uint checkInterval, int stepCount, double precision, int timeout)
{
    double curPos = getCurrentOutputPos();
    if (qAbs(curPos - targetPos) < MinStep)
    {
        return;
    }

    qCInfo(motionCate(), N_FUNC_ARG(targetPos, checkInterval, stepCount, precision));

    GlobalState::getIns().waitNotPaused(name());

    if (moveProtection->axisIsAbleToMove(name(), targetPos))
    {
        absMove(targetPos, true, precision);
        return;
    }

    if (stepCount < 1)
    {
        throw SilicolAbort(tr("StepCount must greater than 0!"), EX_LOCATION);
    }
    double stepSize = (targetPos - curPos) / stepCount;
    double longestPos = curPos;
    bool getNewLongestPos = false;
    ErrorHandler::tryToHandleGeneralError<void>([&] {
        QElapsedTimer timer;
        timer.start();
        while (true)
        {
            double tmpLongestPos = longestPos;
            while (true)
            {
                if (timer.elapsed() > timeout)
                {
                    throw GeneralError("Axis", tr("Try to move timeout!"));
                }
                if (qAbs(targetPos - tmpLongestPos) > qAbs(stepSize))
                {
                    tmpLongestPos += stepSize;
                }
                else
                {
                    tmpLongestPos = targetPos;
                }
                if (moveProtection->axisIsAbleToMove(name(), tmpLongestPos))
                {
                    longestPos = tmpLongestPos;
                    getNewLongestPos = true;
                    if (qFuzzyCompare(longestPos, targetPos))
                    {
                        break;
                    }
                    QThread::msleep(1);
                }
                else
                {
                    break;
                }
            }

            if (getNewLongestPos)
            {
                absForceMove(longestPos);
                getNewLongestPos = false;
            }
            if (qFuzzyCompare(longestPos, targetPos))
            {
                break;
            }
            QThread::msleep(checkInterval);
        }
    });
    waitArrivedPos(targetPos, precision);
}

void SCAxis::waitStopped()
{
    int remainedDelay = CheckRunningDelayAfterMove - axisMoveTimer.elapsedMs();
    if (remainedDelay > 0)
    {
        QThread::msleep(remainedDelay);
    }
    QElapsedTimer timer;
    timer.start();
    while (isRunning())
    {
        QThread::msleep(1);
        if (timer.elapsed() > 30000)
        {
            timer.restart();
            qCWarning(motionCate()) << "Waiting axis stopped..." << name();
        }
    }
}

void SCAxis::waitProfilerStopped(double targetPos)
{
    int remainedDelay = CheckRunningDelayAfterMove - axisMoveTimer.elapsedMs();
    if (remainedDelay > 0)
    {
        QThread::msleep(remainedDelay);
    }

    if (!isRunning())
    {
        if (m_config->advancedAxisConfig()->checkProfilerPrecision())
        {
            if (isArrivedPos(targetPos, getCurrentOutputPos(), getProfilerPrecision()))
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    ErrorHandler::tryToHandleError(
        [&] {
            double prfPos = 0;
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                QThread::msleep(1);
                if (!isRunning())
                {
                    break;
                }
                if (timer.elapsed() > 30000)
                {
                    timer.restart();
                    qCWarning(motionCate()) << "Waiting axis stopped..." << name();
                }
            }
            if (m_config->advancedAxisConfig()->checkProfilerPrecision())
            {
                timer.restart();
                while (true)
                {
                    prfPos = getCurrentOutputPos();
                    if (isArrivedPos(targetPos, prfPos, getProfilerPrecision()))
                    {
                        return;
                    }
                    if (timer.elapsed() > 200)
                    {
                        throw ActionError(
                            "Axis", QObject::tr("Axis %1 may be stopped! Target pos %2! Profiler pos: %3").arg(name()).arg(targetPos).arg(prfPos));
                    }
                    QThread::msleep(1);
                }
            }
        },
        [&] {
            absMoveImpl(targetPos);
            m_hasStop = false;
        });
}

void SCAxis::waitProfilerStopped()
{
    waitProfilerStopped(currentTargetPos);
}

void SCAxis::waitInPos(double targetPos, double precision, int timeout)
{
    if (isInPos() && isArrivedPos(getCurrentOutputPos(), getFeedbackPos(-1), precision))
    {
        return;
    }
    ErrorHandler::tryToHandleError([&] {
        double feedbackPos = 0;
        QElapsedTimer timer;
        timer.start();
        while (true)
        {
            QThread::msleep(1);
            if (isInPos())
            {
                break;
            }
            if (timer.elapsed() > timeout)
            {
                throw ActionError("Axis", QObject::tr("Axis %1 wait inpos timeout!").arg(name()));
            }
        }

        double prfPos = getCurrentOutputPos();
        timer.restart();
        while (true)
        {
            feedbackPos = getFeedbackPos(-1);
            if (isArrivedPos(prfPos, feedbackPos, precision))
            {
                return;
            }
            if (timer.elapsed() > timeout)
            {
                throw ActionError(
                    "Axis", QObject::tr("Axis %1 wait arrived pos timeout!\r\nTarget pos: %2, Profiler pos: %3, Feedback pos: %4, Inpos precision %5")
                                .arg(name())
                                .arg(targetPos)
                                .arg(prfPos)
                                .arg(feedbackPos)
                                .arg(precision));
            }
            QThread::msleep(1);
        }
    });
}

void SCAxis::waitInPos(double precision, int timeout)
{
    waitInPos(currentTargetPos, precision, timeout);
}

void SCAxis::waitSettling(double precision, int timeout)
{
    ErrorHandler::tryToHandleError([&] { waitSettling(qAbs(m_config->advancedAxisConfig()->settlingWindow()), precision, timeout); });
}

void SCAxis::absForceMove(double targetPos)
{
    if (isProfilerAtPos(targetPos, MinStep))
    {
        currentTargetPos = targetPos;
        return;
    }

    if (isinf(targetPos) || isnan(targetPos))
    {
        throw SilicolAbort(QString("Invalid target pos: %1").arg(targetPos), EX_LOCATION);
    }

    qCInfo(motionCate(), N_FUNC_ARG(targetPos));

    GlobalState::getIns().waitNotPaused(name());

    absMoveImpl(targetPos, false);
    m_hasStop = false;
}

void SCAxis::relForceMove(double step)
{
    qCDebug(motionCate(), N_FUNC_ARG(step));

    double targetPos = getCurrentOutputPos() + step;
    absForceMove(targetPos);
}

void SCAxis::relForceMoveWithLimit(double step)
{
    qCDebug(motionCate(), N_FUNC_ARG(step));

    double targetPos = getCurrentOutputPos() + step;
    if (m_config->advancedAxisConfig()->useSoftLimit())
    {
        getPosInLimitRange(targetPos);
    }
    absForceMove(targetPos);
}

void SCAxis::scaleMaxVel(double ratio)
{
    if (qFuzzyCompare(ratio, currentVelRatio))
    {
        return;
    }
    if (ratio < 0.0001 || ratio > 10)
    {
        qCWarning(motionCate()) << QString("Vel ratio exceeded limit! Ignored "
                                           "scaleMaxVel command. Axis: %1, Ratio: "
                                           "%2, Limit: [0.0001, 10].")
                                       .arg(name())
                                       .arg(ratio);
        return;
    }
    qCDebug(motionCate(), N_FUNC_ARG(ratio));
    scaleMaxVelImpl(ratio);
    currentVelRatio = ratio;
    nextMoveVelRatio = ratio;
}

void SCAxis::scaleMaxAcc(double ratio)
{
    if (qFuzzyCompare(ratio, currentAccRatio))
    {
        return;
    }
    if (ratio < 0.0001 || ratio > 10)
    {
        qCWarning(motionCate()) << QString("Acc ratio exceeded limit! Ignored "
                                           "scaleMaxAcc command. Axis: %1, Ratio: "
                                           "%2, Limit: [0.0001, 10].")
                                       .arg(name())
                                       .arg(ratio);
        return;
    }
    qCDebug(motionCate(), N_FUNC_ARG(ratio));
    scaleMaxAccImpl(ratio);
    currentAccRatio = ratio;
    nextMoveAccRatio = ratio;
}

void SCAxis::resetMaxAcc()
{
    currentAccRatio = -1;
    scaleMaxAcc(1);
}

void SCAxis::resetMaxVel()
{
    currentVelRatio = -1;
    scaleMaxVel(m_config->velocityRatio());
}

void SCAxis::setNextMoveVelAccRatio(double velRatio, double accRatio)
{
    nextMoveVelRatio = velRatio > 0 ? velRatio : config()->velocityRatio();
    nextMoveAccRatio = accRatio > 0 ? accRatio : 1;
}

void SCAxis::setNextMoveVelAcc(double vel, double acc)
{
    double velRatio = vel / maxVelocity();
    double accRatio = acc / maxAcc();
    setNextMoveVelAccRatio(velRatio, accRatio);
}

void SCAxis::jogMove(int direction)
{
    double vel = m_config->advancedAxisConfig()->jogMoveMaxVel();
    if (!m_config->advancedAxisConfig()->useSoftLimit())
    {
        vel = m_config->maxVel() * m_config->velocityRatio();
    }
    jogMoveWithVel(direction, vel);
}

void SCAxis::jogMoveWithVel(int direction, double velocity)
{
    double minVel = qMin(velocity, m_config->maxVel() * m_config->velocityRatio());
    double minVelRatio = minVel / m_config->maxVel();

    if (direction == 0)
    {
        if (!m_config->advancedAxisConfig()->useSoftLimit())
        {
            velocityMove(Direction::Negative, minVel, minVel * 10);
        }
        else
        {
            moveToNegativeLimit(false, minVelRatio);
        }
    }
    if (direction == 1)
    {
        if (!m_config->advancedAxisConfig()->useSoftLimit())
        {
            velocityMove(Direction::Positive, minVel, minVel * 10);
        }
        else
        {
            moveToPositiveLimit(false, minVelRatio);
        }
    }
}

void SCAxis::moveToPositiveLimit(bool waitDone, double velocityRatioLimit)
{
    qCDebug(motionCate(), N_FUNC_ARG(waitDone, velocityRatioLimit));

    slowMoveTo(positiveLimit(), waitDone, velocityRatioLimit);
}

void SCAxis::moveToNegativeLimit(bool waitDone, double velocityRatioLimit)
{
    qCDebug(motionCate(), N_FUNC_ARG(waitDone, velocityRatioLimit));

    slowMoveTo(negativeLimit(), waitDone, velocityRatioLimit);
}

void SCAxis::slowMoveTo(double targetPos, bool waitDone, double velocityRatioLimit)
{
    if (velocityRatioLimit < config()->velocityRatio())
    {
        scaleMaxVel(velocityRatioLimit);
    }

    try
    {
        absMove(targetPos, waitDone);
        if (waitDone && velocityRatioLimit < config()->velocityRatio())
        {
            resetMaxVel();
        }
    }
    catch (SilicoolException &se)
    {
        if (waitDone && velocityRatioLimit < config()->velocityRatio())
        {
            resetMaxVel();
        }
        throw se;
    }
}

void SCAxis::velocityMove(Direction dir, double vel, double acc)
{
    if (!m_config->advancedAxisConfig()->useSoftLimit())
    {
        if (vel <= 0)
        {
            vel = m_config->maxVel() * m_config->velocityRatio();
        }
        if (acc <= 0)
        {
            acc = m_config->maxAcc();
        }
        velocityMoveImpl(dir, vel, acc);
    }
    else
    {
        throw SilicolAbort(tr("Can not move on velocity mode if useSoftLimit! Axis: %1").arg(name()), EX_LOCATION);
    }
}

void SCAxis::startReciprocate(double pos1, int delay1, double pos2, int delay2, int times)
{
    qCDebug(motionCate(), N_FUNC_ARG(pos1, delay1, pos2, delay2, times));

    if (isReciprocating)
    {
        qCritical() << "Reciprocating is running!";
        return;
    }
    isReciprocating = true;
    AutoResetBool t(&isReciprocating, false);

    double profileTime = 0;
    double settlingTime = 0;
    QElapsedTimer timer;

    for (int i = 0; i < times; i++)
    {
        if (!isReciprocating)
        {
            return;
        }

        timer.restart();
        absMove(pos1, false);
        waitProfilerStopped(pos1);
        profileTime = timer.elapsed();
        timer.restart();
        waitArrivedPos(pos1, 0.1);
        settlingTime = timer.elapsed();
        qDebug(motionCate()) << QString("Move to %1, Profile time: %2ms, Settling time: %3ms.").arg(pos1).arg(profileTime).arg(settlingTime);

        if (delay1 > 0)
        {
            QThread::msleep(static_cast<uint>(delay1));
        }
        if (!isReciprocating)
        {
            return;
        }

        timer.restart();
        absMove(pos2, false);
        waitProfilerStopped(pos2);
        profileTime = timer.elapsed();
        timer.restart();
        waitArrivedPos(pos2, 0.1);
        settlingTime = timer.elapsed();
        qDebug(motionCate()) << QString("Move to %1, Profile time: %2ms, Settling time: %3ms.").arg(pos2).arg(profileTime).arg(settlingTime);

        if (delay2 > 0)
        {
            QThread::msleep(static_cast<uint>(delay2));
        }
    }
}

void SCAxis::stopReciprocate()
{
    qCDebug(motionCate(), N_FUNC);

    isReciprocating = false;
}

void SCAxis::setMasterAxis(SCAxis *masterAxis)
{
    m_masterAxis = masterAxis;
}

SCAxis *SCAxis::masterAxis() const
{
    return m_masterAxis;
}

bool SCAxis::hasMasterAxis()
{
    return m_masterAxis != nullptr;
}

bool SCAxis::hasBindedToMaterAxis()
{
    return m_hasBindToMasterAxis;
}

bool SCAxis::bindToMasterAxis()
{
    if (!hasMasterAxis())
    {
        qCCritical(motionCate()) << tr("Bind to master axis failed! %1 did not have master axis!").arg(name());
        return false;
    }
    if (m_hasBindToMasterAxis)
    {
        return true;
    }
    else
    {
        if (bindToMasterAxisImpl(true))
        {
            m_hasBindToMasterAxis = true;
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool SCAxis::unBindToMasterAxis()
{
    if (!m_hasBindToMasterAxis)
    {
        return true;
    }
    else
    {
        if (bindToMasterAxisImpl(false))
        {
            m_hasBindToMasterAxis = false;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void SCAxis::softLandPosDown(QString softLandingPos, bool waitDone)
{
    auto pos = m_config->getPos(softLandingPos);
    softLandDown(pos->velocity(), pos->targetPos(), pos->force(), pos->margin(), waitDone, pos->timeout());
}

void SCAxis::softLandDown(double vel, double targetPos, double force, double margin, bool waitDone, int timeout)
{
    GlobalState::getIns().waitNotPaused(name());

    double currentPos = getCurrentOutputPos();
    qCInfo(motionCate(), N_FUNC_ARG(vel, currentPos, targetPos, force, margin, waitDone));
    SCTimer timer(QString("%1 softLandDown").arg(name()), motionCate(), waitDone);

    startSoftLandDown(vel, targetPos, force, margin);
    m_hasStop = false;
    if (waitDone)
    {
        QThread::msleep(10);
        waitSoftLandDownFinished(vel, targetPos, force, margin, timeout);
    }
}

void SCAxis::waitSoftLandDownFinished(double vel, double targetPos, double force, double margin, int timeout)
{
    ErrorHandler::tryToHandleError(
        [&] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (m_hasStop)
                {
                    throw ActionError("Axis", QObject::tr("Axis %1 has been stopped! CurrentPos: %2").arg(name()).arg(getFeedbackPos(-1)));
                }
                QThread::msleep(1);
                if (isSoftLandDownFinished())
                {
                    return;
                }

                if (timer.elapsed() > timeout)
                {
                    stop();
                    throw ActionError("Axis", QObject::tr("Axis %1 wait soft land down finished timeout!").arg(name()));
                }
            }
        },
        [&] {
            startSoftLandDown(vel, targetPos, force, margin);
            m_hasStop = false;
        });
}

void SCAxis::waitSoftLandUpFinished(int timeout)
{
    ErrorHandler::tryToHandleError(
        [&] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (m_hasStop)
                {
                    throw ActionError("Axis", QObject::tr("Axis %1 has been stopped! CurrentPos: %2").arg(name()).arg(getFeedbackPos(-1)));
                }
                QThread::msleep(1);
                if (isSoftLandUpFinished())
                {
                    return;
                }

                if (timer.elapsed() > timeout)
                {
                    stop();
                    throw ActionError("Axis", QObject::tr("Axis %1 wait soft land up finished timeout!").arg(name()));
                }
            }
        },
        [&] {
            startSoftLandUp();
            m_hasStop = false;
        });
}

void SCAxis::softLandUp(bool waitDone, int timeout)
{
    startSoftLandUp();
    m_hasStop = false;
    if (waitDone)
    {
        QThread::msleep(10);
        waitSoftLandUpFinished(timeout);
    }
}

double SCAxis::measureHeight(double vel, double force, int holdTime)
{
    double currentPos = getCurrentOutputPos();
    double targetPos = (positiveLimit() - currentPos) / 2.0 + currentPos;
    double margin = (positiveLimit() - currentPos) / 2.0 - 0.01;
    softLandDown(vel, targetPos, force, margin);
    double posBeforeHold = getFeedbackPos(-1);
    if (holdTime > 0)
    {
        QThread::msleep(holdTime);
    }
    double height = getFeedbackPos(-1);
    qCInfo(motionCate()) << QString("Axis %1 measure height. Pos before hold: %2, after hold: %3.").arg(posBeforeHold).arg(height);
    softLandUp();
    return height;
}

void SCAxis::startSoftLandDown(double vel, double targetPos, double force, double margin)
{
    ErrorHandler::tryToHandleError([this, vel, targetPos, force, margin] {
        checkState();
        checkIsRunning();
        checkInLimitRange(targetPos);
        softLandDownImpl(vel, targetPos, force, margin);
    });
    m_isSoftLandDown = true;
}

void SCAxis::startSoftLandUp()
{
    ErrorHandler::tryToHandleError([this] {
        checkState();
        softLandUpImpl();
    });
    m_isSoftLandDown = false;
}

void SCAxis::onConfigVelChanged(double v)
{
    Q_UNUSED(v)
    resetMaxVel();
}

void SCAxis::onConfigAccChanged(double v)
{
    Q_UNUSED(v)
    resetMaxAcc();
}

void SCAxis::onHomeExecutorChanged(AdvancedAxisConfig::HomeExecutor homeExecutor)
{
    m_homeExecutor = homeExecutor;
    auto advancedConfig = m_config->advancedAxisConfig();
    if (m_homeExecutor == AdvancedAxisConfig::Driver || m_homeExecutor == AdvancedAxisConfig::DriverThenController)
    {
        try
        {
            homeDoneDi = MotionElementContainer::getIns()->getItem<SCDI>(advancedConfig->homeDoneDiName());
            startHomeDo = MotionElementContainer::getIns()->getItem<SCDO>(advancedConfig->startHomeDoName());
        }
        catch (SilicoolException &se)
        {
            UIOperation::getIns()->showError(se.what());
        }
    }
    else
    {
        homeDoneDi = nullptr;
        startHomeDo = nullptr;
    }
}

void SCAxis::waitArrivedPos(double targetPos, double precision, int timeout)
{
    waitProfilerStopped(targetPos);
    if (m_config->advancedAxisConfig()->settlingByAppLayer())
    {
        waitSettling(precision, timeout);
    }
    else
    {
        waitInPos(targetPos, precision, timeout);
    }
}

void SCAxis::waitArrivedPos(double precision, int timeout)
{
    waitArrivedPos(currentTargetPos, precision, timeout);
}

void SCAxis::waitGreaterThanPos(double targetPos, double precision, int timeout)
{
    ErrorHandler::tryToHandleError([&] {
        QElapsedTimer timer;
        timer.start();
        bool profilerStopped = false;
        while (true)
        {
            if (getFeedbackPos(-1) > (targetPos - precision))
            {
                return;
            }
            if (!profilerStopped)
            {
                if (!isRunning())
                {
                    profilerStopped = true;
                    timeout = 3000;
                    timer.restart();
                }
            }

            QThread::msleep(1);
            if (timer.elapsed() > timeout)
            {
                throw ActionError("Axis", QObject::tr("Axis %1 wait greater than pos timeout! Target pos:%2, Current "
                                                      "pos: %3, Compare precision %4")
                                              .arg(name())
                                              .arg(targetPos)
                                              .arg(getFeedbackPos(-1))
                                              .arg(precision));
            }
        }
    });
}

void SCAxis::waitLessThanPos(double targetPos, double precision, int timeout)
{
    ErrorHandler::tryToHandleError([&] {
        QElapsedTimer timer;
        timer.start();
        bool profilerStopped = false;
        while (true)
        {
            if (getFeedbackPos(-1) < (targetPos + precision))
            {
                return;
            }
            if (!profilerStopped)
            {
                if (!isRunning())
                {
                    profilerStopped = true;
                    timeout = 3000;
                    timer.restart();
                }
            }

            QThread::msleep(1);
            if (timer.elapsed() > timeout)
            {
                throw ActionError("Axis", QObject::tr("Axis %1 wait less than pos timeout! Target pos:%2, Current "
                                                      "pos: %3, Compare precision %4")
                                              .arg(name())
                                              .arg(targetPos)
                                              .arg(getFeedbackPos())
                                              .arg(precision));
            }
        }
    });
}

void SCAxis::waitApproachPos(double targetPos, double precision, int timeout)
{
    ErrorHandler::tryToHandleError([&] {
        QElapsedTimer timer;
        timer.start();
        bool profilerStopped = false;
        while (true)
        {
            if (qAbs(targetPos - getFeedbackPos(-1)) < precision)
            {
                return;
            }
            if (!profilerStopped)
            {
                if (!isRunning())
                {
                    profilerStopped = true;
                    timeout = 3000;
                    timer.restart();
                }
            }

            QThread::msleep(1);
            if (timer.elapsed() > timeout)
            {
                throw ActionError("Axis", QObject::tr("Axis %1 wait approach pos timeout! Target pos:%2, Current "
                                                      "pos: %3, Compare precision %4")
                                              .arg(name())
                                              .arg(targetPos)
                                              .arg(getFeedbackPos())
                                              .arg(precision));
            }
        }
    });
}

void SCAxis::checkState(bool checkHasHome)
{
    if (!isInit())
    {
        throw ActionError("Axis", QObject::tr("Axis %1 has not been inited!").arg(name()));
    }
    if (!isEnable())
    {
        throw ActionError("Axis", QObject::tr("Axis %1 has not been enabled!").arg(name()));
    }
    if (checkHasHome && !hasHome())
    {
        throw ActionError("Axis", QObject::tr("Axis %1 has not been homed!").arg(name()));
    }
    if (hasAlarm())
    {
        throw ActionError("Axis", QObject::tr("Axis %1 has Alarm!").arg(name()), "Please clear alarm first!");
    }
}

void SCAxis::checkIsRunning()
{
    if (isRunning())
    {
        throw ActionError("Axis", QObject::tr("Axis %1 is running!").arg(name()));
    }
}

void SCAxis::checkInLimitRange(double targetPos) const
{
    if (!isInLimitRange(targetPos))
    {
        throw ActionError(
            "Axis",
            QObject::tr("Axis %1 target pos %2 exceeded limit [%3, %4]!").arg(name()).arg(targetPos).arg(negativeLimit()).arg(positiveLimit()));
    }
}

void SCAxis::setHasHome(bool value)
{
    if (value)
    {
        qCInfo(motionCate()) << QString("%1 set has home!").arg(name());
    }

    m_hasHome = value;
}

void SCAxis::waitSettling(int window, double precision, int timeout)
{
    QList<double> feedbackPos;
    bool gotIt;
    double ave;
    int startIndex;
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        feedbackPos.append(getFeedbackPosImpl());
        gotIt = true;
        startIndex = feedbackPos.count() - window;
        if (startIndex >= 0)
        {
            double sum = 0;
            for (int i = startIndex; i < feedbackPos.count(); i++)
            {
                sum += feedbackPos[i];
            }
            ave = sum / window;
            for (int i = startIndex; i < feedbackPos.count(); i++)
            {
                if (qAbs(ave - feedbackPos[i]) > precision)
                {
                    gotIt = false;
                    break;
                }
            }
        }
        else
        {
            gotIt = false;
        }

        if (gotIt)
        {
            return;
        }
        if (timer.elapsed() > timeout)
        {
            throw ActionError(
                "Axis",
                QObject::tr("Axis %1 wait settling timeout! Settling window length: %2, window size: %3").arg(name()).arg(window).arg(precision));
        }
        QThread::usleep(500);
    }
}

void SCAxis::startHome()
{
    ErrorHandler::tryToHandleError([&] {
        checkState(false);
        checkIsRunning();

        if (m_homeExecutor == AdvancedAxisConfig::Driver || m_homeExecutor == AdvancedAxisConfig::DriverThenController)
        {
            SC_ASSERT(startHomeDo != nullptr)

            qCInfo(motionCate()) << tr("%1 start home. Handled by driver.").arg(name());
            if (startHomeDo->get())
            {
                startHomeDo->set(false);
                QThread::msleep(100);
            }
            if (m_config->advancedAxisConfig()->resetDriverTime() > 0)
            {
                disable();
                QThread::msleep(m_config->advancedAxisConfig()->resetDriverTime());
                enable();
            }
            startHomeDo->set(true);
            if (m_homeExecutor == AdvancedAxisConfig::DriverThenController)
            {
                m_hasStop = false;
                waitDriverHomeDone();
                stepMove(m_config->advancedAxisConfig()->homeOffsetBetweenDriverAndController());
                m_hasHome = false;
                homeImpl();
            }
        }
        else
        {
            clearPosImpl();
            homeImpl();
        }
    });
}

void SCAxis::absMoveImpl(double targetPos, bool checkRunning)
{
    ErrorHandler::tryToHandleError([&] {
        checkState();
        if (checkRunning)
        {
            checkIsRunning();
        }
        checkInLimitRange(targetPos);
        moveProtection->checkAxisIsAbleToMove(name(), targetPos);
        if (nextMoveVelRatio > 0)
        {
            scaleMaxVel(nextMoveVelRatio);
        }
        if (nextMoveAccRatio > 0)
        {
            scaleMaxAcc(nextMoveAccRatio);
        }
        currentTargetPos = targetPos;
        axisMoveTimer.start();
        moveToImpl(targetPos);
    });
}

bool SCAxis::isArrivedPos(double targetPos, double currentPos, double precision)
{
    if (abs(currentPos - targetPos) <= precision)
    {
        return true;
    }
    return false;
}

void SCAxis::getPosInLimitRange(double &targetPos)
{
    if (targetPos > positiveLimit())
    {
        targetPos = positiveLimit();
    }
    else if (targetPos < negativeLimit())
    {
        targetPos = negativeLimit();
    }
}

double SCAxis::getProfilerPrecision() const
{
    double precision = m_config->advancedAxisConfig()->maxCompensation();
    if (precision <= 0)
    {
        precision = 1.01 / m_config->scale();
    }
    return precision;
}

void SCAxis::waitControllerHomeDone()
{
    ErrorHandler::tryToHandleError(
        [&] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (m_hasStop)
                {
                    throw ActionError("Axis", QObject::tr("Axis %1 has been stopped!").arg(name()));
                }
                if (isHomeDone())
                {
                    QString homeErrMsg = homeErrorMsg();
                    if (!homeErrMsg.isEmpty())
                    {
                        throw ActionError("Axis", QObject::tr("Axis %1 home failed!\r\n%2").arg(name()).arg(homeErrMsg));
                    }
                    if (m_hasStop)
                    {
                        throw ActionError("Axis", QObject::tr("Axis %1 has been stopped!").arg(name()));
                    }
                    waitSettlingAfterHome();
                    clearPosImpl();
                    operationAfterHome();
                    resetMaxAcc();
                    resetMaxVel();
                    m_hasHome = true;
                    currentTargetPos = 0;
                    qCDebug(motionCate()) << tr("Axis %1 home done!").arg(name());
                    return;
                }
                QThread::msleep(5);
                if (timer.elapsed() > m_config->homeTimeout())
                {
                    stopHome();
                    throw ActionError("Axis", QObject::tr("Axis %1 wait home done timeout!").arg(name()));
                }
            }
        },
        [&] {
            startHome();
            m_hasStop = false;
        });
}

void SCAxis::waitDriverHomeDone()
{
    SC_ASSERT(homeDoneDi != nullptr)
    SC_ASSERT(startHomeDo != nullptr)

    QThread::msleep(50);
    ErrorHandler::tryToHandleError(
        [this] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (m_hasStop)
                {
                    throw ActionError("Axis", QObject::tr("Axis %1 has been stopped!").arg(name()));
                }
                if (homeDoneDi->get())
                {
                    startHomeDo->set(false);
                    waitSettlingAfterHome();
                    clearPosImpl();
                    operationAfterHome();
                    resetMaxAcc();
                    resetMaxVel();
                    m_hasHome = true;
                    currentTargetPos = 0;
                    qCDebug(motionCate()) << tr("Axis %1 home done!").arg(name());
                    return;
                }
                QThread::msleep(5);
                if (timer.elapsed() > m_config->homeTimeout())
                {
                    startHomeDo->set(false);
                    throw ActionError("Axis", QObject::tr("Axis %1 wait home done timeout!").arg(name()));
                }
            }
        },
        [&] {
            startHome();
            m_hasStop = false;
        });
}

void SCAxis::waitSettlingAfterHome()
{
    ErrorHandler::tryToHandleError([this] {
        auto advancedAxisConfig = m_config->advancedAxisConfig();
        if (advancedAxisConfig->homeSettlingPricision() < 0)
        {
            QThread::msleep(300);
        }
        else
        {
            double dPricision = advancedAxisConfig->homeSettlingPricision() / m_config->scale();
            waitSettling(advancedAxisConfig->homeSettlingWindow(), dPricision, advancedAxisConfig->homeSettlingTimeout());
        }
    });
}
