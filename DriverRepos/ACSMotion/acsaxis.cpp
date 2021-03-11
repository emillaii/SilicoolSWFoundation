#include "acsaxis.h"

ACSAxis::ACSAxis(QString name, QObject *parent) : SCAxis(name, parent)
{
    hComm = (HANDLE)ACSCardConfigManage::getIns().getACSCardCfg()->handle();

    acsAxisConfig = qobject_cast<ACSAxisConfig *>(config());
    if(acsAxisConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast AxisConfig to ACSAxisConfig! AxisConfig type: %1")
                               .arg(config()->metaObject()->className()),
                           EX_LOCATION);
    }
}

void ACSAxis::kill() noexcept
{
    int ret = acsc_Kill(hComm, acsAxisConfig->channel(), nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_Kill() in kill() failed!").arg(name());
    }
}

bool ACSAxis::hasAlarm() noexcept
{
    int errorCode = acsc_GetLastError();
    if (errorCode == 0)
        return false;
    else
        return true;
    int Error;
    char ErrorMsg[256];
    CheckACSCResult(acsc_GetMotionError(hComm));
}

bool ACSAxis::isInPos() noexcept
{
    int state;
    int ret = acsc_GetMotorState(hComm, acsAxisConfig->channel(), &state, nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_GetMotorState() in isInPos() failed!").arg(name());
        return false;
    }
    if (state == ACSC_MST_INPOS)
        return true;
    else
        return false;
}

bool ACSAxis::isRunning() noexcept
{
    int state;
    int ret = acsc_GetMotorState(hComm, acsAxisConfig->channel(), &state, nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_GetMotorState() in isRunning() failed!").arg(name());
        return false;
    }
    if (state == ACSC_MST_MOVE)
        return true;
    else
        return false;
}

double ACSAxis::getCurrentVel() noexcept
{
    double velocity;
    int ret = acsc_GetVelocity(hComm, acsAxisConfig->channel(), &velocity, nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_GetVelocity() in getCurrentVel() failed!").arg(name());
        return 0;
    }

    return velocity;
}

void ACSAxis::initImpl()
{
    hComm = acsc_OpenCommSerial(acsAxisConfig->channel(), acsAxisConfig->rate());
}

void ACSAxis::homeImpl()
{

}

void ACSAxis::stopImpl() noexcept
{
    // The terminated motion finishes using the full third-order deceleration profile and the motion deceleration value.
    int ret = acsc_Halt(hComm, acsAxisConfig->channel(), nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_Halt() in stopImpl() failed!").arg(name());
    }
}

void ACSAxis::stopHome()
{
    stopImpl();
}

void ACSAxis::enableImpl()
{
    int ret = acsc_Enable(hComm, acsAxisConfig->channel(), nullptr);
    if (ret == 0)
    {
        throw ActionError(name(), QString("acsc_Enable() in enableImpl() failed!"));
    }
}

void ACSAxis::disableImpl()
{
    int ret = acsc_Disable(hComm, acsAxisConfig->channel(), nullptr);
    if (ret == 0)
    {
        throw ActionError(name(), QString("acsc_Disable() in disableImpl() failed!"));
    }
}

void ACSAxis::clearErrorImpl()
{

}

void ACSAxis::moveToImpl(double targetPos)
{
    int ret = acsc_ToPoint(hComm, 0, acsAxisConfig->channel(), targetPos, nullptr);
    if (ret == 0)
    {
        throw ActionError(name(), QString("acsc_ToPoint() in moveToImpl() failed!"));
    }
}

double ACSAxis::getFeedbackPosImpl() noexcept
{
    double FPosition;
    int ret = acsc_GetFPosition(hComm, acsAxisConfig->channel(), &FPosition, nullptr);
    if (ret == 0)
    {
        qCCritical(motionCate()) << tr("acsc_GetFPosition() in getFeedbackPosImpl() failed!").arg(name());
        return 0;
    }

    return FPosition;
}
