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
    CheckACSCResult(acsc_Kill(hComm, acsAxisConfig->index(), nullptr), "acsc_Kill");
}

bool ACSAxis::hasAlarm() noexcept
{
    int acsc_error = -1;
    char errMsg[256];
    int received;

    CheckACSCResult(acsc_GetMotionError(hComm, acsAxisConfig->index(),&acsc_error, NULL), "acsc_GetMotionError failed");

    if(acsc_error >0)
    {
        CheckACSCResult(acsc_GetErrorString(hComm,acsc_error,errMsg,255,&received), "acsc_GetErrorString");
        throw  ActionError(name(), QString("Motor error, errCode: %1; errMsg: %2").arg(acsc_error).arg(errMsg));
    }
}

bool ACSAxis::isInPos() noexcept
{
    int state;
    CheckACSCResult(acsc_GetMotorState(hComm, acsAxisConfig->index(), &state, nullptr), "acsc_GetMotorState");

    return state == ACSC_MST_INPOS;
}

bool ACSAxis::isRunning() noexcept
{
    int state;
    CheckACSCResult(acsc_GetMotorState(hComm, acsAxisConfig->index(), &state, nullptr), "acsc_GetMotorState");

    return state == ACSC_MST_MOVE;
}

double ACSAxis::getCurrentVel() noexcept
{
    double velocity;
    CheckACSCResult(acsc_GetVelocity(hComm, acsAxisConfig->index(), &velocity, nullptr), "acsc_GetVelocity");

    return velocity;
}

void ACSAxis::initImpl()
{
    ACSCardConfig *acsConfig = ACSCardConfigManage::getIns().getACSCardCfg();
    hComm = (HANDLE)acsConfig->handle();
    acsAxisConfig = qobject_cast<ACSAxisConfig *>(config());
    if(acsAxisConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast AxisConfig to ACSCAxisConfig! AxisConfig type: %1")
                               .arg(config()->metaObject()->className()),
                           EX_LOCATION);
    }
}

void ACSAxis::homeImpl()
{
    CheckACSCResult(acsc_RunBuffer(hComm, acsAxisConfig->homeBfIndex(), NULL, NULL), "acsc_RunBuffer");
}

void ACSAxis::stopImpl() noexcept
{
    // The terminated motion finishes using the full third-order deceleration profile and the motion deceleration value.
    CheckACSCResult(acsc_Halt(hComm, acsAxisConfig->index(), nullptr), "acsc_Halt");
}

void ACSAxis::stopHome()
{
    stopImpl();
}

void ACSAxis::enableImpl()
{
    CheckACSCResult(acsc_Enable(hComm, acsAxisConfig->index(), nullptr), "acsc_Enable");
}

void ACSAxis::disableImpl()
{
    CheckACSCResult(acsc_Disable(hComm, acsAxisConfig->index(), nullptr), "acsc_Disable");
}

void ACSAxis::clearErrorImpl()
{
    CheckACSCResult(acsc_RunBuffer(hComm, acsAxisConfig->initBfIndex(), NULL, NULL), "acsc_RunBuffer");
}

void ACSAxis::moveToImpl(double targetPos)
{
    CheckACSCResult(acsc_ToPoint(hComm, 0, acsAxisConfig->index(), targetPos, nullptr), "acsc_ToPoint");
}

double ACSAxis::getFeedbackPosImpl() noexcept
{
    double FPosition;
    CheckACSCResult(acsc_GetFPosition(hComm, acsAxisConfig->index(), &FPosition, nullptr), "acsc_GetFPosition");
    return FPosition;
}
