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
        short res = GTN_PrfTrap(coreNo, index);
        if (res != CMD_SUCCESS)
        {
            throw ActionError(name(), QString("GTN_PrfTrap failed! Error code: %1").arg(res));
        }
        currentMode = Pos;
        scaleMaxAccImpl(lastAccRatio);
        scaleMaxVelImpl(lastVelRatio);
    }
    else if (mode == Jog)
    {
        short res = GTN_PrfJog(coreNo, index);
        if (res != CMD_SUCCESS)
        {
            throw ActionError(name(), QString("GTN_PrfJog failed! Error code: %1").arg(res));
        }
        currentMode = Jog;
    }
}

void GTAxis::followMasterAxis(GTAxis *masterAxis, long masterEven, long slaveEven)
{
    if (masterAxis == nullptr)
    {
        throw SilicolAbort(tr("Master gt axis is null!"), EX_LOCATION);
    }
    if (masterAxis->coreNo != coreNo)
    {
        throw SilicolAbort(tr("Master gt axis and slaver gt axis must in the same core!"), EX_LOCATION);
    }
    if (masterAxis->index > index)
    {
        qCWarning(motionCate()) << tr("Master gt axis index being less than slaver gt axis index is recommended!");
    }
    checkResult1(GTN_PrfGear(coreNo, index));
    checkResult1(GTN_SetGearMaster(coreNo, index, masterAxis->index));
    checkResult1(GTN_SetGearRatio(coreNo, index, masterEven, slaveEven, 0));
    checkResult1(GTN_GearStart(coreNo, 1 << (index - 1)));
    currentMode = Gear;
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
    double value = 0;
    GTN_GetPlsVel(coreNo, index, &value);
    return value * VelCoeff / gtAxisConfig->scale();
}

void GTAxis::clearStatus()
{
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
    checkResult1(GTN_PrfTrap(coreNo, index));
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
    if (homeConfig->gtHomeMode() <= HOME_MODE_LIMIT_HOME_INDEX)
    {
        if (homeConfig->homeDir() == 1 && refreshLimitStatus(true))
        {
            leaveLimitPos(true);
        }
        if (homeConfig->homeDir() == 0 && refreshLimitStatus(false))
        {
            leaveLimitPos(false);
        }
    }
    THomePrm homeParam;
    short res = GTN_GetHomePrm(coreNo, index, &homeParam);
    CheckGTAxisResult(res, "GTN_GetHomePrm failed");
    homeParam.acc = gtAxisConfig->maxAcc() * gtAxisConfig->scale() / AccCoeff * 0.1;
    homeParam.dec = homeParam.acc;
    homeParam.edge = homeConfig->edge();
    homeParam.mode = homeConfig->gtHomeMode();
    homeParam.velLow = homeConfig->velLow() * gtAxisConfig->scale() / VelCoeff;
    homeParam.moveDir = homeConfig->homeDir();
    homeParam.velHigh = homeConfig->velHigh() * gtAxisConfig->scale() / VelCoeff;
    homeParam.escapeStep = homeConfig->escapeStep() * gtAxisConfig->scale();
    homeParam.indexDir = homeConfig->indexDir();
    homeParam.homeOffset = homeConfig->gtHomeOffset() * gtAxisConfig->scale();
    homeParam.searchHomeDistance = 0;
    homeParam.searchIndexDistance = 0;
    res = GTN_GoHome(coreNo, index, &homeParam);
    CheckGTAxisResult(res, "GTN_GoHome failed");
}

bool GTAxis::isHomeDone() noexcept
{
    GTN_GetHomeStatus(coreNo, index, &homeStatus);
    return (homeStatus.run == 0);
}

QString GTAxis::homeErrorMsg()
{
    GTN_GetHomeStatus(coreNo, index, &homeStatus);
    switch (homeStatus.error)
    {
        case HOME_ERROR_NONE:
            return "";
        case HOME_ERROR_NOT_TRAP_MODE:
            return "HOME_ERROR_NOT_TRAP_MODE";
        case HOME_ERROR_DISABLE:
            return "HOME_ERROR_DISABLE";
        case HOME_ERROR_ALARM:
            return "HOME_ERROR_ALARM";
        case HOME_ERROR_STOP:
            return "HOME_ERROR_STOP";
        case HOME_ERROR_STAGE:
            return "HOME_ERROR_STAGE";
        case HOME_ERROR_HOME_MODE:
            return "HOME_ERROR_HOME_MODE";
        case HOME_ERROR_SET_CAPTURE_HOME:
            return "HOME_ERROR_SET_CAPTURE_HOME";
        case HOME_ERROR_NO_HOME:
            return "HOME_ERROR_NO_HOME";
        case HOME_ERROR_SET_CAPTURE_INDEX:
            return "HOME_ERROR_SET_CAPTURE_INDEX";
        case HOME_ERROR_NO_INDEX:
            return "HOME_ERROR_NO_INDEX";
        default:
            return QString("Unknown error. Code: %1").arg(homeStatus.error);
    }
}

void GTAxis::operationAfterHome()
{
    GTN_ClrSts(coreNo, index);
}

void GTAxis::stopImpl() noexcept
{
    short res = GTN_Stop(coreNo, 1 << (index - 1), 0);
    printError(res, QString("%1 GTN_Stop failed!").arg(name()));
}

void GTAxis::stopHome()
{
    stopImpl();
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

    short res = GTN_SetPos(coreNo, index, round(targetPos * gtAxisConfig->scale()));
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
    short res = GTN_SetVel(coreNo, index, vel);
    printError(res, QString("%1 GTN_SetVel failed!").arg(name()));

    res = GTN_Update(coreNo, 1 << (index - 1));
    printError(res, QString("%1 GTN_Update failed!").arg(name()));
}

double GTAxis::getFeedbackPosImpl() noexcept
{
    double value = 0;
    GTN_GetAxisEncPos(coreNo, index, &value);
    return value / gtAxisConfig->scale();
}

bool GTAxis::bindToMasterAxisImpl(bool bind)
{
    if (bind)
    {
        auto masterGtAxis = qobject_cast<GTAxis *>(masterAxis());
        try
        {
            followMasterAxis(masterGtAxis, 1, 1);
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
    double pos = 0;
    GTN_GetAxisEncPos(coreNo, index, &pos);
    short res = GTN_SetPrfPos(coreNo, index, pos);
    printError(res, QString("%1 GTN_SetPrfPos failed!").arg(name()));
    res = GTN_SynchAxisPos(coreNo, 1 << (index - 1));
    printError(res, QString("%1 GTN_SynchAxisPos failed!").arg(name()));
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
