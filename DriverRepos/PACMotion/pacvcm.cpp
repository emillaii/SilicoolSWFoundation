#include "pacvcm.h"

QMap<int, QString> PACVcm::errCode2Str;
bool PACVcm::hasFillErrCode2Str(false);
int PACVcm::pacVcmCount(0);

PACVcm::PACVcm(QString name, QObject *parent) : SCAxis(name, parent)
{
    pacVcmCount++;
    if (!hasFillErrCode2Str)
    {
        errCode2Str[EC_NORMAL] = "EC_NORMAL";
        errCode2Str[EC_MOTOR_DISABLED] = "EC_MOTOR_DISABLED";
        errCode2Str[EC_MOTOR_MOVING] = "EC_MOTOR_MOVING";
        errCode2Str[EC_WARNING_POS_ERR] = "EC_WARNING_POS_ERR";
        errCode2Str[EC_FATAL_POS_ERR] = "EC_FATAL_POS_ERR";
        errCode2Str[EC_OVERCURRENT] = "EC_OVERCURRENT";
        errCode2Str[EC_DRIVER_OVERHEAT] = "EC_DRIVER_OVERHEAT";
        errCode2Str[EC_OPERA_DENY] = "EC_OPERA_DENY";
        errCode2Str[EC_SN_ERROR] = "EC_SN_ERROR";
        errCode2Str[EC_DRIVER_FAULT] = "EC_DRIVER_FAULT";
        errCode2Str[EC_CTRL_CONFLICT] = "EC_CTRL_CONFLICT";
        errCode2Str[EC_SERVO_MODE] = "EC_SERVO_MODE";
        hasFillErrCode2Str = true;
    }
}

PACVcm::~PACVcm()
{
    pacVcmCount--;
    if (pacVcmCount <= 0)
    {
        PAC_Disconnect();
    }
}

void PACVcm::keepForce(int current)
{
    ErrorHandler::tryToHandleError([&] {
        checkState();
        setServoMode(SM_TQ);
        if (!PAC_CmdCur(vcmAddress, current))
        {
            throw ActionError(name(), tr("PAC_CmdCur failed!"));
        }
    });
}

void PACVcm::setConfig(AxisConfig *config)
{
    m_vcmConfig = qobject_cast<PACVcmConfig *>(config);
    if (m_vcmConfig == nullptr)
    {
        qCritical() << "Can not cast config to PACVcmConfig!"
                    << "Config type:" << config->metaObject()->className();
    }
    SCAxis::setConfig(config);
}

double PACVcm::getCurrentOutputPos() noexcept
{
    return getFeedbackPosImpl();
}

bool PACVcm::hasAlarm() noexcept
{
    ushort errCode = 0;
    PAC_GetErrorCode(vcmAddress, &errCode);
    if (errCode != EC_NORMAL)
    {
        QString errStr = errCode2Str.contains(errCode) ? errCode2Str[errCode] : "Unknown error";
        qCCritical(motionCate()) << QString("%1 error. Code: %2, Msg: %3").arg(name()).arg(errCode).arg(errStr);
        return true;
    }
    else
    {
        return false;
    }
}

bool PACVcm::isReady() noexcept
{
    return true;
}

bool PACVcm::isInPos() noexcept
{
    BOOL t_isInPos = false;
    PAC_IsInPosition(vcmAddress, &t_isInPos);
    return t_isInPos;
}

bool PACVcm::isRunning() noexcept
{
    return !isInPos();
}

double PACVcm::getCurrentVel() noexcept
{
    short currentVel;
    PAC_GetActVel(vcmAddress, &currentVel);
    return currentVel;
}

void PACVcm::initImpl()
{
    vcmAddress = m_vcmConfig->vcmAddress();
    if (!PAC_AutoConnect(vcmAddress))
    {
        throw SilicolAbort(tr("%1 connect to driver failed! Driver address: %2").arg(name()).arg(vcmAddress));
    }
    if (!PAC_SetOpMode(vcmAddress, OM_NETWORK))
    {
        throw SilicolAbort(tr("%1 set operation mode to 'OM_NETWORK' failed!").arg(name()));
    }
    setServoMode(SM_POS);
}

void PACVcm::homeImpl()
{
    startSoftLanding(m_vcmConfig->homeVel(), m_vcmConfig->findHardLimitDir(), m_vcmConfig->findHardLimitCurrent());
    waitActionDone(10000);
    if (!PAC_FindIndex(vcmAddress, m_vcmConfig->homeVel(), m_vcmConfig->maxAcc(), m_vcmConfig->findIndexDir(), false))
    {
        throw ActionError(name(), tr("PAC_FindIndex failed!"));
    }
}

bool PACVcm::isHomeDone() noexcept
{
    return isInPos();
}

QString PACVcm::homeErrorMsg()
{
    return "";
}

void PACVcm::operationAfterHome() {}

void PACVcm::stopHome()
{
    stop();
}

void PACVcm::enableImpl()
{
    PAC_Enable(vcmAddress, true);
}

void PACVcm::disableImpl()
{
    PAC_Enable(vcmAddress, false);
}

void PACVcm::clearErrorImpl()
{
    PAC_ClearError(vcmAddress);
}

void PACVcm::moveToImpl(double targetPos)
{
    setServoMode(SM_POS);
    if (!PAC_MoveAbs(vcmAddress, targetPos * m_vcmConfig->scale(), false))
    {
        throw ActionError(name(), "PAC_MoveAbs failed!");
    }
}

void PACVcm::clearPosImpl()
{
    PAC_SetActPos(vcmAddress, 0);
}

void PACVcm::velocityMoveImpl(SCAxis::Direction dir, double vel, double acc)
{
    setServoMode(SM_VEL);
    if (dir == Direction::Negative)
    {
        vel *= -1;
    }
    PAC_CmdVel(vcmAddress, vel);
}

double PACVcm::getFeedbackPosImpl() noexcept
{
    int pos = 0;
    PAC_GetActPos(vcmAddress, &pos);
    return pos / m_vcmConfig->scale();
}

bool PACVcm::bindToMasterAxisImpl(bool bind)
{
    Q_UNUSED(bind)
    qCCritical(motionCate()) << "UnImplemented funciont: bindToMasterAxisImpl";
    return false;
}

void PACVcm::scaleMaxAccImpl(double ratio)
{
    PAC_SetAcc(vcmAddress, m_vcmConfig->maxAcc() * ratio);
}

void PACVcm::scaleMaxVelImpl(double ratio)
{
    PAC_SetAcc(vcmAddress, m_vcmConfig->maxVel() * ratio);
}

void PACVcm::softLandDownImpl(double vel, double targetPos, double force, double margin)
{
    startSoftLandDownPos = getFeedbackPosImpl();
    margin = qAbs(margin);
    double minLandingPos = targetPos - margin;
    double maxLandingPos = targetPos + margin;
    if (startSoftLandDownPos < minLandingPos - 0.1)
    {
        qCDebug(motionCate()) << name() << "softlanding... Fast move to" << minLandingPos;
        moveToImpl(minLandingPos);
        waitActionDone(30000);
    }
    else if (startSoftLandDownPos > maxLandingPos + 0.1)
    {
        qCDebug(motionCate()) << name() << "softlanding... Fast move to" << maxLandingPos;
        moveToImpl(maxLandingPos);
        waitActionDone(30000);
    }
    startSoftLanding(vel, DIR_POSITIVE, force);
}

void PACVcm::softLandUpImpl()
{
    setServoMode(SM_POS, false);
    resetMaxVel();
    moveToImpl(startSoftLandDownPos);
}

bool PACVcm::isSoftLandDownFinished() noexcept
{
    return isInPos();
}

bool PACVcm::isSoftLandUpFinished() noexcept
{
    return isInPos();
}

void PACVcm::setServoMode(BYTE ucMode, bool checkMode)
{
    if (checkMode && ucMode == currentServoMode)
    {
        return;
    }
    ErrorHandler::tryToHandleError([&] {
        if (!PAC_SetServoMode(vcmAddress, ucMode))
        {
            throw ActionError(name(), tr("Set servo mode to %1 failed!").arg(ucMode));
        }
    });
    currentServoMode = ucMode;
}

void PACVcm::startSoftLanding(double vel, BYTE dir, double force)
{
    if (!PAC_SoftLanding(vcmAddress, vel, dir, force, false))
    {
        throw ActionError(name(), tr("Start soft landing failed..."));
    }
}

void PACVcm::waitActionDone(int timeout)
{
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (isInPos())
        {
            return;
        }
        QThread::msleep(1);
        if (timer.elapsed() > timeout)
        {
            stop();
            throw ActionError(name(), tr("Wait action done timeout!"));
        }
    }
}
