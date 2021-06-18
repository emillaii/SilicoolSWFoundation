#include "acsaxis.h"

ACSAxis::ACSAxis(QString name, QObject *parent) : SCAxis(name, parent)
{

}

void ACSAxis::kill()
{
    checkACSCom(acsc_Kill(hComm, acsAxisConfig->index(), nullptr), "acsc_Kill");
}

bool ACSAxis::hasAlarm() noexcept
{
    int acsc_error = -1;
    char errMsg[256];
    int received;

    int result = acsc_GetMotorError(hComm, acsAxisConfig->index(),&acsc_error, NULL);
    if( result == 0)
    {
        qCritical(motionCate()) << tr("%1 axis acsc_GetMotionError faile").arg(acsAxisConfig->name());
        return true;
    }

    if(acsc_error != 0 && acsc_error != 5001)
    {
        result = acsc_GetErrorString(hComm, acsc_error, errMsg, 255, &received);
        qWarning(motionCate()) << QString("Motor error, errCode: %1; errMsg: %2").arg(acsc_error).arg(errMsg);
        return true;
    }

    return false;
}

bool ACSAxis::isInPos() noexcept
{
    int state = ACSC_MST_MOVE;
    checkACSCom(acsc_GetMotorState(hComm, acsAxisConfig->index(), &state, nullptr), "acsc_GetMotorState");
    return state & ACSC_MST_INPOS;
}

bool ACSAxis::isRunning() noexcept
{
    int state = ACSC_AST_MOVE;
    checkACSCom(acsc_GetAxisState(hComm, acsAxisConfig->index(), &state, nullptr), "acsc_GetAxisState");
    return state & ACSC_AST_MOVE;
}

double ACSAxis::getCurrentVel() noexcept
{
    double velocity = 0;
    checkACSCom(acsc_GetVelocity(hComm, acsAxisConfig->index(), &velocity, nullptr), "acsc_GetVelocity");

    return velocity* VelCoeff / acsAxisConfig->scale();
}

void ACSAxis::clearStatus()
{
    clearErrorImpl();
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
    checkACSCom(acsc_RunBuffer(hComm, acsAxisConfig->homeBfIndex(), NULL, NULL), "acsc_RunBuffer");
}

bool ACSAxis::isHomeDone() noexcept
{
    int state = ACSC_PST_RUN;
    checkACSCom(acsc_GetProgramState(hComm, acsAxisConfig->homeBfIndex(), &state, NULL), "acsc_GetProgramState");
    return state == ACSC_PST_COMPILED;
}

void ACSAxis::stopImpl() noexcept
{
    // The terminated motion finishes using the full third-order deceleration profile and the motion deceleration value.
    qInfo() << "ACS stopImpl..";
    checkACSCom(acsc_Halt(hComm, acsAxisConfig->index(), nullptr), "acsc_Halt");
}

void ACSAxis::stopHome()
{
    stopImpl();
}

void ACSAxis::enableImpl()
{
    checkACSCom(acsc_Enable(hComm, acsAxisConfig->index(), nullptr), "acsc_Enable");
}

void ACSAxis::disableImpl()
{
    checkACSCom(acsc_Disable(hComm, acsAxisConfig->index(), nullptr), "acsc_Disable");
}

void ACSAxis::clearErrorImpl()
{
    char cmd[] = "FCLEAR ALL\r";
    char *cm = cmd;

    checkACSCom(acsc_Command(hComm, cm, strlen(cm), NULL), "acsc_Command");
}

void ACSAxis::moveToImpl(double targetPos)
{
    checkACSCom(acsc_ToPoint(hComm, 0, acsAxisConfig->index(), targetPos, nullptr), "acsc_ToPoint");
}

double ACSAxis::getFeedbackPosImpl() noexcept
{
    double FPosition = 0;
    checkACSCom(acsc_GetFPosition(hComm, acsAxisConfig->index(), &FPosition, nullptr), "acsc_GetFPosition");
    return FPosition / acsAxisConfig->scale();
}

void ACSAxis::scaleMaxAccImpl(double ratio)
{
    checkACSCom(acsc_SetAcceleration(hComm, acsAxisConfig->index(), acsAxisConfig->maxAcc()*acsAxisConfig->scale() / AccCoeff * ratio, NULL), "acsc_SetAcceleration");
}

void ACSAxis::velocityMoveImpl(SCAxis::Direction dir, double vel, double acc)
{

}

void ACSAxis::scaleMaxVelImpl(double ratio)
{
    checkACSCom(acsc_SetVelocity(hComm, acsAxisConfig->index(), acsAxisConfig->maxVel()*acsAxisConfig->scale() / VelCoeff * ratio, NULL), "acsc_SetVelocity");
}

void ACSAxis::checkACSCom(int errCode, QString msg)
{
    if (errCode == 0)                                                                                            \
    {                                                                                                                  \
        qCritical(motionCate()) << msg << "failed! Error code:" << errCode;                                                    \
    }
}
