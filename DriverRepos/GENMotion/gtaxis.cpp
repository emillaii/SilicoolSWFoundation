#include "gtaxis.h"

GTAxis::GTAxis(QString name, QObject *parent) : SCAxis(name, parent) {}

void GTAxis::setMoveMode(GTAxis::MoveMode mode, bool checkCurrentMode)
{
    if (checkCurrentMode && mode == currentMode)
    {
        return;
    }
    if (mode == Pos)
    {
        if (gtAxisConfig->isEthercatAxis())
        {
            short res = GTN_SetEcatAxisMode(coreNo, index, gtAxisConfig->posModeIndex());
            CheckGTAxisResult(res, "GTN_SetEcatAxisMode failed");
        }
        else
        {
            short res = GTN_PrfTrap(coreNo, index);
            CheckGTAxisResult(res, "GTN_PrfTrap failed");
        }

        currentMode = Pos;
        scaleMaxAccImpl(lastAccRatio);
        scaleMaxVelImpl(lastVelRatio);
    }
    else if (mode == Jog)
    {
        if (gtAxisConfig->isEthercatAxis())
        {
            short res = GTN_SetEcatAxisMode(coreNo, index, gtAxisConfig->jogModeIndex());
            CheckGTAxisResult(res, "GTN_SetEcatAxisMode failed");
        }
        else
        {
            short res = GTN_PrfJog(coreNo, index);
            CheckGTAxisResult(res, "GTN_PrfJog failed");
        }

        currentMode = Jog;
    }
    else if (mode == Tor)
    {
        if (gtAxisConfig->isEthercatAxis())
        {
            short res = GTN_SetEcatAxisMode(coreNo, index, gtAxisConfig->torModeIndex());
            CheckGTAxisResult(res, "GTN_SetEcatAxisMode failed");
        }
        else
        {
            throw SilicolAbort(tr("Non ethercat axis can not run in torque mode!"), EX_LOCATION);
        }

        currentMode = Tor;
    }
}

double GTAxis::getCurrentOutputPos() noexcept
{
    double value = 0;
    GTN_GetAxisPrfPos(coreNo, index, &value);
    return value / gtAxisConfig->scale();
}

bool GTAxis::hasAlarm() noexcept
{
    long status = getAxisStatus();
    return (status & (1 << 1));
}

bool GTAxis::isInPos() noexcept
{
    long status = getAxisStatus();
    return (status & (1 << 11));
}

bool GTAxis::isRunning() noexcept
{
    long status = getAxisStatus();
    return (status & (1 << 10));
}

bool GTAxis::getNegativeLimitStatus() noexcept
{
    return getAxisStatus() & (1 << 6);
}

bool GTAxis::getPositiveLimitStatus() noexcept
{
    return getAxisStatus() & (1 << 5);
}

double GTAxis::getCurrentVel() noexcept
{
    if (gtAxisConfig->isEthercatAxis())
    {
        long value = 0;
        short res = GTN_GetEcatEncVel(coreNo, index, &value);
        printError(res, QString("%1 GTN_GetEcatEncVel failed!").arg(name()));
        return double(value) * VelCoeff / gtAxisConfig->scale();
    }
    else
    {
        double value = 0;
        short res = GTN_GetPlsVel(coreNo, index, &value);
        printError(res, QString("%1 GTN_GetPlsVel failed!").arg(name()));
        return value * VelCoeff / gtAxisConfig->scale();
    }
}

void GTAxis::clearStatus()
{
    //    if(isReadStatus)
    //    {
    //        isReadStatus = false;
    //        return;
    //    }
    //    isReadStatus = true;

    //    short current;
    //    short tor;
    //    while (isReadStatus)
    //    {
    //        short res = GTN_GetEcatAxisAtlCurrent(coreNo, index, &current);
    //        printError(res, "GTN_GetEcatAxisAtlCurrent");
    //        res = GTN_GetEcatAxisAtlTorque(coreNo, index, &tor);
    //        printError(res, "GTN_GetEcatAxisAtlTorque");
    //        qDebug() << "current" << current << "torque" << tor;
    //        QThread::msleep(200);
    //    }

    long status = getAxisStatus(false);
    qDebug() << QString("Axis %1 status before clear status: %2").arg(name()).arg(QString::number(status, 16));
    short res = GTN_ClrSts(coreNo, index);
    printError(res, QString("%1 GTN_ClrSts failed!").arg(name()));
    status = getAxisStatus(false);
    qDebug() << QString("Axis %1 status after clear status: %2").arg(name()).arg(QString::number(status, 16));
    SCAxis::clearStatus();
}

void GTAxis::initImpl()
{
    gtAxisConfig = qobject_cast<GTAxisConfig *>(config());
    if (gtAxisConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast AxisConfig to GTAxisConfig! AxisConfig type: %1").arg(config()->metaObject()->className()),
                           EX_LOCATION);
    }

    coreNo = gtAxisConfig->coreNo();
    index = gtAxisConfig->index();

    checkResult1(GTN_ClrSts(coreNo, index));

    if (gtAxisConfig->isEthercatAxis())
    {
        checkResult1(GTN_SetEcatAxisMode(coreNo, index, gtAxisConfig->posModeIndex()));
    }
    else
    {
        checkResult1(GTN_PrfTrap(coreNo, index));
    }
    TTrapPrm trapParam;
    checkResult1(GTN_GetTrapPrm(coreNo, index, &trapParam));
    trapParam.acc = gtAxisConfig->maxAcc() * gtAxisConfig->scale() / AccCoeff;
    trapParam.dec = trapParam.acc;
    checkResult1(GTN_SetTrapPrm(coreNo, index, &trapParam));
    checkResult1(GTN_SetAxisBand(coreNo, index, gtAxisConfig->inPosBand(), gtAxisConfig->inPosHoldTime()));
    if (!isConnectInposParamChangedSig)
    {
        connect(gtAxisConfig, &GTAxisConfig::inPosBandChanged, this, &GTAxis::onInposBandParamChanged);
        connect(gtAxisConfig, &GTAxisConfig::inPosHoldTimeChanged, this, &GTAxis::onInposBandParamChanged);
        isConnectInposParamChangedSig = true;
    }
    clearPosImpl();
    currentMode = Pos;
}

void GTAxis::homeImpl()
{
    clearHasStop();
    auto homeConfig = gtAxisConfig->gtHomeConfig();
    if (gtAxisConfig->isEthercatAxis())
    {
        short res = GTN_SetHomingMode(coreNo, index, 6);
        CheckGTAxisResult(res, "GTN_SetHomingMode failed");
        res = GTN_SetEcatHomingPrm(coreNo, index, homeConfig->ecatHomeMethod(), homeConfig->ecatSearchHomeSpeed(), homeConfig->ecatSearchIndexSpeed(),
                                   homeConfig->ecatHomeAcc(), homeConfig->ecatHomeOffset(), 0);
        CheckGTAxisResult(res, "GTN_SetEcatHomingPrm failed");
        res = GTN_StartEcatHoming(coreNo, index);
        CheckGTAxisResult(res, "GTN_StartEcatHoming failed");
    }
    else
    {
        TTrapPrm trapParam;
        checkResult1(GTN_GetTrapPrm(coreNo, index, &trapParam));
        trapParam.acc = homeConfig->velHigh() * 10 * gtAxisConfig->scale() / AccCoeff;
        checkResult1(GTN_SetTrapPrm(coreNo, index, &trapParam));

        myHomeDone = false;
        myHomeErrMsg.clear();
        QtConcurrent::run([this] { myHomeFunc(); });
    }
}

bool GTAxis::isHomeDone() noexcept
{
    if (gtAxisConfig->isEthercatAxis())
    {
        ushort homeStatus;
        short res = GTN_GetEcatHomingStatus(coreNo, index, &homeStatus);
        printError(res, "GTN_GetEcatHomingStatus");
        return (homeStatus & 0x1) != 0;
    }
    else
    {
        return myHomeDone;
    }
}

QString GTAxis::homeErrorMsg()
{
    if (gtAxisConfig->isEthercatAxis())
    {
        ushort homeStatus;
        GTN_GetEcatHomingStatus(coreNo, index, &homeStatus);
        if ((homeStatus & 0x2) != 0)
        {
            return "";
        }
        else
        {
            return "Driver home error";
        }
    }
    else
    {
        return myHomeErrMsg;
    }
}

void GTAxis::operationAfterHome()
{
    GTN_ClrSts(coreNo, index);
    short res = GTN_SetHomingMode(coreNo, index, 8);
    CheckGTAxisResult(res, "GTN_SetHomingMode failed");
}

void GTAxis::stopImpl() noexcept
{
    short res = GTN_Stop(coreNo, 1 << (index - 1), 0);
    printError(res, QString("%1 GTN_Stop failed!").arg(name()));
}

void GTAxis::stopHome()
{
    if (gtAxisConfig->isEthercatAxis())
    {
        short res = GTN_StopEcatHoming(coreNo, index);
        printError(res, QString("%1 GTN_StopEcatHoming failed!").arg(name()));
    }
    else
    {
        stopImpl();
    }
}

void GTAxis::enableImpl()
{
    short res = GTN_AxisOn(coreNo, index);
    printError(res, QString("%1 GTN_AxisOn failed!").arg(name()));
}

void GTAxis::disableImpl()
{
    short res = GTN_AxisOff(coreNo, index);
    printError(res, QString("%1 GTN_AxisOff failed!").arg(name()));
}

void GTAxis::clearErrorImpl()
{
    short res = GTN_SetDoBit(coreNo, MC_CLEAR, index, 1);
    printError(res, QString("%1 GTN_SetDoBit failed!").arg(name()));
    QThread::msleep(100);
    GTN_SetDoBit(coreNo, MC_CLEAR, index, 0);
    QThread::msleep(10);
    res = GTN_ClrSts(coreNo, index);
    printError(res, QString("%1 GTN_ClrSts failed!").arg(name()));
}

void GTAxis::moveToImpl(double targetPos)
{
    setMoveMode(Pos);

    short res = GTN_SetPos(coreNo, index, targetPos * gtAxisConfig->scale());
    CheckGTAxisResult(res, "GTN_SetPos failed");
    res = GTN_Update(coreNo, 1 << (index - 1));
    CheckGTAxisResult(res, "GTN_Update failed");
}

void GTAxis::velocityMoveImpl(Direction dir, double vel, double acc)
{
    setMoveMode(Jog);

	try
    {
        TJogPrm jogParam;
        checkResult1(GTN_GetJogPrm(coreNo, index, &jogParam));
        jogParam.acc = acc * gtAxisConfig->scale() / AccCoeff;
        jogParam.dec = jogParam.acc;
        checkResult1(GTN_SetJogPrm(coreNo, index, &jogParam));
    }
    catch (SilicoolException &se)
    {
        qCCritical(motionCate()) << se.what();
    }

    vel = vel * gtAxisConfig->scale() / VelCoeff;
    if (dir == Direction::Negative)
    {
        vel *= -1;
    }
	
    if (gtAxisConfig->isEthercatAxis())
    {
        short res = GTN_SetEcatAxisPV(coreNo, index, vel);
        printError(res, QString("%1 GTN_SetEcatAxisPV failed!").arg(name()));
    }
    else
    {
        short res = GTN_SetVel(coreNo, index, vel);
        printError(res, QString("%1 GTN_SetVel failed!").arg(name()));
    }

    short res = GTN_Update(coreNo, 1 << (index - 1));
    printError(res, QString("%1 GTN_Update failed!").arg(name()));
}

double GTAxis::getFeedbackPosImpl() noexcept
{
    if (gtAxisConfig->isEthercatAxis())
    {
        long value = 0;
        short res = GTN_GetEcatEncPos(coreNo, index, &value);
        printError(res, QString("%1 GTN_GetEcatEncPos failed!").arg(name()));
        return double(value) / gtAxisConfig->scale();
    }
    else
    {
        double value = 0;
        GTN_GetAxisEncPos(coreNo, index, &value);
        return value / gtAxisConfig->scale();
    }
}

bool GTAxis::bindToMasterAxisImpl(bool bind)
{
    if (gtAxisConfig->isEthercatAxis())
    {
        qCCritical(motionCate()) << tr("Ethercat axis can not bind to another axis! Axis name: %1").arg(name());
        return false;
    }
    if (bind)
    {
        auto masterGtAxis = qobject_cast<GTAxis *>(masterAxis());
        try
        {
            if (masterGtAxis == nullptr)
            {
                throw SilicolAbort(tr("Master gt axis is null!"), EX_LOCATION);
            }
            if (masterGtAxis->coreNo != coreNo)
            {
                throw SilicolAbort(tr("Master gt axis and slaver gt axis must in the same core!"), EX_LOCATION);
            }
            if (masterGtAxis->index > index)
            {
                qCWarning(motionCate()) << tr("Master gt axis index being less than slaver gt axis index is recommended!");
            }
            checkResult1(GTN_PrfGear(coreNo, index));
            checkResult1(GTN_SetGearMaster(coreNo, index, masterGtAxis->index));
            checkResult1(GTN_SetGearRatio(coreNo, index, 1, 1, 0));
            checkResult1(GTN_GearStart(coreNo, 1 << (index - 1)));
            currentMode = Gear;
            return true;
        }
        catch (SilicoolException &se)
        {
            qCCritical(motionCate()) << se.what();
            return false;
        }
    }
    else
    {
        setMoveMode(Pos, false);
        return true;
    }
}

void GTAxis::scaleMaxAccImpl(double ratio)
{
    lastAccRatio = ratio;
    if (currentMode == Pos)
    {
        TTrapPrm trapParam;
        try
        {
            checkResult1(GTN_GetTrapPrm(coreNo, index, &trapParam));
            trapParam.acc = gtAxisConfig->maxAcc() * gtAxisConfig->scale() / AccCoeff * ratio;
            trapParam.dec = trapParam.acc;
            checkResult1(GTN_SetTrapPrm(coreNo, index, &trapParam));
        }
        catch (SilicoolException &se)
        {
            qCCritical(motionCate()) << se.what();
        }
    }
}

void GTAxis::scaleMaxVelImpl(double ratio)
{
    lastVelRatio = ratio;
    if (currentMode == Pos)
    {
        short res = GTN_SetVel(coreNo, index, gtAxisConfig->maxVel() * gtAxisConfig->scale() * ratio / VelCoeff);
        printError(res, QString("%1 GTN_SetVel failed!").arg(name()))
    }
}

void GTAxis::clearPosImpl()
{
    short res = GTN_ZeroPos(coreNo, index);
    printError(res, QString("%1 GTN_ZeroPos failed! ").arg(name()));
    res = GTN_SynchAxisPos(coreNo, 1 << (index - 1));
    printError(res, QString("%1 GTN_SynchAxisPos failed!").arg(name()));
}

void GTAxis::syncProfilerAndFeedbackPos()
{
    double pos = getFeedbackPosImpl();
    pos *= gtAxisConfig->scale();
    short res = GTN_SetPrfPos(coreNo, index, pos);
    printError(res, QString("%1 GTN_SetPrfPos failed!").arg(name()));
    res = GTN_SynchAxisPos(coreNo, 1 << (index - 1));
    printError(res, QString("%1 GTN_SynchAxisPos failed!").arg(name()));
}

void GTAxis::softLandDownImpl(double vel, double targetPos, double force, double margin)
{
    Q_UNUSED(vel)

    startSoftLandDownPos = getFeedbackPosImpl();
    targetTorque = force;
    margin = qAbs(margin);
    double minLandingPos = targetPos - margin;
    double maxLandingPos = targetPos + margin;
    if (startSoftLandDownPos < minLandingPos - 0.1)
    {
        qCDebug(motionCate()) << name() << "softlanding... Fast move to" << minLandingPos;
        moveToImpl(minLandingPos);
        waitProfilerStopped(minLandingPos);
        force = qAbs(force);
    }
    else if (startSoftLandDownPos > maxLandingPos + 0.1)
    {
        qCDebug(motionCate()) << name() << "softlanding... Fast move to" << maxLandingPos;
        moveToImpl(maxLandingPos);
        waitProfilerStopped(maxLandingPos);
        force = -qAbs(force);
    }
    else
    {
        force = (targetPos > startSoftLandDownPos ? qAbs(force) : -qAbs(force));
    }
    setMoveMode(Tor);
    short res = GTN_SetEcatAxisPT(coreNo, index, force);
    CheckGTAxisResult(res, "GTN_SetEcatAxisPT failed");
}

void GTAxis::softLandUpImpl()
{
    setMoveMode(Pos, false);
    resetMaxAcc();
    resetMaxVel();
    moveToImpl(startSoftLandDownPos);
}

bool GTAxis::isSoftLandDownFinished() noexcept
{
    try
    {
        waitSettling(gtAxisConfig->softLandingWindowLen(), gtAxisConfig->softLandingWindowSize() / gtAxisConfig->scale(), 1000);
        return true;
    }
    catch (SilicoolException &se)
    {
        se.what();
        return false;
    }
}

bool GTAxis::isSoftLandUpFinished() noexcept
{
    return isProfilerAtPos(startSoftLandDownPos, 0.1);
}

void GTAxis::onInposBandParamChanged(int param)
{
    Q_UNUSED(param)
    short res = GTN_SetAxisBand(coreNo, index, gtAxisConfig->inPosBand(), gtAxisConfig->inPosHoldTime());
    printError(res, QString("%1 GTN_SetAxisBand failed!").arg(name()));
}

bool GTAxis::refreshLimitStatus(bool isPositiveLimit)
{
    short res = GTN_ClrSts(coreNo, index);
    printError(res, QString("%1 GTN_ClrSts failed!").arg(name()));
    long status = getAxisStatus();
    int bitIndex = isPositiveLimit ? 5 : 6;
    return status & (1 << bitIndex);
}

void GTAxis::leaveLimitPos(bool isPositiveLimit)
{
    double step = 0.5;
    if (isPositiveLimit)
    {
        step *= -1;
    }
    while (refreshLimitStatus(isPositiveLimit))
    {
        if (hasStop())
        {
            throw ActionError("Axis", QObject::tr("Axis %1 has been stopped!").arg(name()));
        }
        stepMove(step);
    }
}

long GTAxis::getAxisStatus(bool checkCache)
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (!checkCache || now - lastUpdateAxisStatusCacheTime >= AxisStatusCacheInvalidTime)
    {
        GTN_GetSts(coreNo, index, &axisStatusCache);
        lastUpdateAxisStatusCacheTime = now;
    }
    return axisStatusCache;
}

void GTAxis::myHomeFunc()
{
    const long LongMax = INT32_MAX / 4;
    const long LongMin = INT32_MIN / 4;
    try
    {
        auto homeConfig = gtAxisConfig->gtHomeConfig();
        if (homeConfig->gtHomeMode() == GTHomeConfig::MODE_LIMIT_HOME
            || homeConfig->gtHomeMode() == GTHomeConfig::MODE_LIMIT_INDEX)    // move to limit pos
        {
            if (homeConfig->homeDir() == homeConfig->indexDir())
            {
                throw SilicolAbort(tr("Error home parameter!"));
            }
            bool limitStatus;
            int targetLimitPos;
            if (homeConfig->homeDir() == 1)
            {
                limitStatus = true;
                targetLimitPos = LongMax;
            }
            else
            {
                limitStatus = false;
                targetLimitPos = LongMin;
            }
            if (!refreshLimitStatus(limitStatus))
            {
                checkResult1(GTN_SetVel(coreNo, index, homeConfig->velHigh() * gtAxisConfig->scale() / VelCoeff));
                checkResult1(GTN_SetPos(coreNo, index, targetLimitPos));
                checkResult1(GTN_Update(coreNo, 1 << (index - 1)));

                QThread::msleep(5);
                while (true)
                {
                    if (!isRunning())
                    {
                        if (hasStop())
                        {
                            throw SilicolAbort(tr("Axis has been stopped!"));
                        }
                        break;
                    }
                    QThread::msleep(1);
                }
            }
        }

        long encPos;
        checkResult1(GTN_GetEcatEncPos(coreNo, index, &encPos));
        checkResult1(GTN_SetPrfPos(coreNo, index, encPos));
        checkResult1(GTN_SetEncPos(coreNo, index, encPos));
        checkResult1(GTN_SynchAxisPos(coreNo, 1 << (index - 1)));

        short probeParameter;
        int targetStatusIndex;
        if (homeConfig->gtHomeMode() == GTHomeConfig::MODE_INDEX || homeConfig->gtHomeMode() == GTHomeConfig::MODE_LIMIT_INDEX)
        {
            probeParameter = homeConfig->edge() == 1 ? 0x1100 : 0x2100;
            targetStatusIndex = homeConfig->edge() == 1 ? 9 : 10;
        }
        else
        {
            probeParameter = homeConfig->edge() == 1 ? 0x11 : 0x21;
            targetStatusIndex = homeConfig->edge() == 1 ? 1 : 2;
        }
        GTN_SetTouchProbeFunction(coreNo, index, 0);
        QThread::msleep(20);
        checkResult1(GTN_SetTouchProbeFunction(coreNo, index, probeParameter));

        checkResult1(GTN_ClrSts(coreNo, index));
        int targetIndexPos = homeConfig->indexDir() == 1 ? LongMax : LongMin;
        checkResult1(GTN_SetVel(coreNo, index, homeConfig->velLow() * gtAxisConfig->scale() / VelCoeff));
        checkResult1(GTN_SetPos(coreNo, index, targetIndexPos));
        checkResult1(GTN_Update(coreNo, 1 << (index - 1)));

        QThread::msleep(5);

        ushort probeOldStatus = 0;
        ushort probeStatus = 0;
        long probe1RiseValue, probe1FallValue, probe2RiseValue, probe2FallValue;
        qDebug(motionCate()) << name() << "Start grab";
        while (true)
        {
            if (!isRunning())
            {
                throw SilicolAbort(tr("Axis has been stopped!"));
            }
            GTN_GetTouchProbeStatus(coreNo, index, &probeStatus, &probe1RiseValue, &probe1FallValue, &probe2RiseValue, &probe2FallValue);
            if (probeStatus != probeOldStatus)
            {
                qDebug(motionCate()) << name() << "Probe status changed." << probeStatus;
                probeOldStatus = probeStatus;
            }
            if ((probeStatus & (1 << targetStatusIndex)) != 0)
            {
                stopImpl();
                break;
            }
            QThread::msleep(1);
        }
        long grabPos;
        if (homeConfig->gtHomeMode() == GTHomeConfig::MODE_INDEX || homeConfig->gtHomeMode() == GTHomeConfig::MODE_LIMIT_INDEX)
        {
            grabPos = homeConfig->edge() == 1 ? probe2RiseValue : probe2FallValue;
        }
        else
        {
            grabPos = homeConfig->edge() == 1 ? probe1RiseValue : probe1FallValue;
        }
        qDebug(motionCate()) << name() << "Grab pos" << grabPos;
        checkResult1(GTN_ClrSts(coreNo, index));
        checkResult1(GTN_SetPos(coreNo, index, grabPos));
        checkResult1(GTN_Update(coreNo, 1 << (index - 1)));
        while (isRunning())
        {
            QThread::msleep(1);
        }

        GTN_SetTouchProbeFunction(coreNo, index, 0);
        myHomeErrMsg.clear();
        myHomeDone = true;
    }
    catch (SilicoolException &se)
    {
        GTN_SetTouchProbeFunction(coreNo, index, 0);
        myHomeErrMsg = se.what();
        myHomeDone = true;
    }
}
