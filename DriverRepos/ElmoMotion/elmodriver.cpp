#include "elmodriver.h"

ElmoDriver::ElmoDriver(QString name, QObject *parent) : SCAxis(name, parent)
{
    nameBuf = name.toUtf8();
    elmo_createInstance(nameBuf);
}

ElmoDriver::~ElmoDriver()
{
    elmo_deleteInstance(nameBuf);
}

void ElmoDriver::setCurrent(double current)
{
    ErrorHandler::tryToHandleError([this, current] {
        sendCommand(QString("TC=%1").arg(current));
        while (true)
        {
            int ms = getMotionStatus();
            if (ms == 1)
            {
                return;
            }
            if (ms == 3)
            {
                throw GeneralError(name(), tr("Set current failed! Motro is disabled!"));
            }
            QThread::msleep(1);
        }
    });
}

void ElmoDriver::elmoMoveTo(double targetPos)
{
    int acc = elmoConfig->maxAcc() * elmoConfig->scale();
    int vel = elmoConfig->maxVel() * elmoConfig->scale() * elmoConfig->velocityRatio();
    int pos = static_cast<int>(round(targetPos * elmoConfig->scale()));
    sendCommand(QString("XQ##moveTo(%1,%2,%3)").arg(vel).arg(acc).arg(pos));
}

double ElmoDriver::getCurrentOutputPos() noexcept
{
    return getFeedbackPosImpl();
}

bool ElmoDriver::isEnable() noexcept
{
    QString rsp;
    if (!getResponse("SO", rsp))
    {
        return false;
    }
    return rsp.toInt() == 1;
}

bool ElmoDriver::hasAlarm() noexcept
{
    QString rsp;
    if (!getResponse("MF", rsp))
    {
        return true;
    }
    ulong errCode = rsp.toULong();
    if (errCode == 0)
    {
        return false;
    }
    else
    {
        qCCritical(motionCate()) << QString("%1 error. Error code: %2").arg(name()).arg(errCode);
        return true;
    }
}

bool ElmoDriver::isReady() noexcept
{
    return isEnable();
}

bool ElmoDriver::isInPos() noexcept
{
    return getMotionStatus() == 0;
}

bool ElmoDriver::isRunning() noexcept
{
    int ms = getMotionStatus();
    return ms < 0 || ms > 1;
}

double ElmoDriver::getCurrentVel() noexcept
{
    QString rsp;
    if (!getResponse(QString("FV[%1]").arg(elmoConfig->posSensorSocketNum()), rsp))
    {
        return 0;
    }
    return double(rsp.toInt()) / elmoConfig->scale();
}

void ElmoDriver::initImpl()
{
    elmoConfig = qobject_cast<ElmoDriverConfig *>(config());
    if (elmoConfig == nullptr)
    {
        throw SilicolAbort("Can not cast axis config to ElmoDriverConfig!");
    }
    if (!elmo_connect(nameBuf, elmoConfig->targetIp().toUtf8(), elmoConfig->hostIp().toUtf8(), elmoConfig->port(),
                      reinterpret_cast<unsigned char *>(errMsg)))
    {
        throw SilicolAbort(tr("Connect to elmo driver %1 failed! Error message: %2").arg(name()).arg(errMsg));
    }
    sendCommand("MO=0");
    QThread::msleep(100);
    QString rsp;
    if (!getResponse("UM", rsp))
    {
        throw SilicolAbort(QString("%1 get unit mode failed!").arg(name()));
    }
    if (rsp.toInt() != 5)
    {
        sendCommand("UM=5");
    }
}

void ElmoDriver::homeImpl()
{
    sendCommand(QString("XQ##homing(%1,%2,%3,%4)")
                    .arg(elmoConfig->homeAcc() * elmoConfig->scale())
                    .arg(elmoConfig->homeVel1() * elmoConfig->scale())
                    .arg(elmoConfig->homeVel2() * elmoConfig->scale())
                    .arg(elmoConfig->homeMaxCurRatio()));
}

bool ElmoDriver::isHomeDone() noexcept
{
    QString rsp;
    if (!getResponse(QString("PS"), rsp))
    {
        return false;
    }
    return rsp.toInt() != 1;
}

QString ElmoDriver::homeErrorMsg()
{
    QString rsp;
    if (!getResponse(QString("PS"), rsp))
    {
        return "Get home error message failed!";
    }
    switch (rsp.toInt())
    {
        case 0:
            return "Home program halted!";
        case -2:
            return "Home program not exist!";
        default:
            return "";
    }
}

void ElmoDriver::operationAfterHome() {}

void ElmoDriver::stopImpl() noexcept
{
    sendCommand("ST");
}

void ElmoDriver::stopHome()
{
    sendCommand("KL");
}

void ElmoDriver::enableImpl()
{
    sendCommand("MO=1");
}

void ElmoDriver::disableImpl()
{
    sendCommand("MO=0");
}

void ElmoDriver::clearErrorImpl() {}

void ElmoDriver::moveToImpl(double targetPos)
{
    sendCommand(QString("PA=%1").arg(static_cast<int>(round(targetPos * elmoConfig->scale()))));
    sendCommand(QString("BG"));
}

void ElmoDriver::velocityMoveImpl(SCAxis::Direction dir, double vel, double acc)
{
    Q_UNUSED(dir)
    Q_UNUSED(vel)
    Q_UNUSED(acc)
    throw SilicolAbort("Unimplemented function: velocityMoveImpl");
}

double ElmoDriver::getFeedbackPosImpl() noexcept
{
    QString rsp;
    if (!getResponse(QString("FP[%1]").arg(elmoConfig->posSensorSocketNum()), rsp))
    {
        return 0;
    }
    return double(rsp.toInt()) / elmoConfig->scale();
}

bool ElmoDriver::bindToMasterAxisImpl(bool bind)
{
    Q_UNUSED(bind)
    throw SilicolAbort("Unimplemented function: bindToMasterAxisImpl");
}

void ElmoDriver::scaleMaxAccImpl(double ratio)
{
    int acc = elmoConfig->maxAcc() * elmoConfig->scale() * ratio;
    sendCommand(QString("AC=%1").arg(acc));
    sendCommand(QString("DC=%1").arg(acc));
}

void ElmoDriver::scaleMaxVelImpl(double ratio)
{
    sendCommand(QString("SP=%1").arg(int(elmoConfig->maxVel() * elmoConfig->scale() * ratio)));
}

void ElmoDriver::clearPosImpl()
{
    sendCommand(QString("FP[%1]=0").arg(elmoConfig->posSensorSocketNum()));
}

void ElmoDriver::syncProfilerAndFeedbackPos() {}

void ElmoDriver::softLandDownImpl(double vel, double targetPos, double force, double margin)
{
    Q_UNUSED(targetPos)
    Q_UNUSED(margin)
    posBeforeSoftlanding = getFeedbackPosImpl();
    sendCommand(
        QString("XQ##softlanding(%1,%2,%3)").arg(vel * elmoConfig->scale()).arg(elmoConfig->softlandingAcc() * elmoConfig->scale()).arg(force));
}

void ElmoDriver::softLandUpImpl()
{
    elmoMoveTo(posBeforeSoftlanding);
}

bool ElmoDriver::isSoftLandDownFinished() noexcept
{
    QString rsp;
    if (!getResponse(QString("PS"), rsp))
    {
        return false;
    }
    return rsp.toInt() != 1;
}

bool ElmoDriver::isSoftLandUpFinished() noexcept
{
    return !isRunning();
}

void ElmoDriver::sendCommand(QString cmd)
{
    unsigned char rsp[10] = {};
    if (!elmo_sendCommand(nameBuf, cmd.toUtf8(), rsp, reinterpret_cast<unsigned char *>(errMsg)))
    {
        qCCritical(motionCate()) << QString("%1 send command failed! Command: %2, error message: %3").arg(name()).arg(cmd).arg(errMsg);
        memset(errMsg, 0, 1024);
    }
}

bool ElmoDriver::getResponse(QString cmd, QString &response)
{
    char rsp[30] = {};
    if (!elmo_sendCommand(nameBuf, cmd.toUtf8(), reinterpret_cast<unsigned char *>(rsp), reinterpret_cast<unsigned char *>(errMsg)))
    {
        qCCritical(motionCate()) << QString("%1 get response failed! Command: %2, error message: %3").arg(name()).arg(cmd).arg(errMsg);
        memset(errMsg, 0, 1024);
        return false;
    }
    response = rsp;
    return true;
}

int ElmoDriver::getMotionStatus()
{
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastUpdateMsTime >= CacheInvalidTime)
    {
        QString rsp;
        if (getResponse("MS", rsp))
        {
            msCache = rsp.toInt();
            lastUpdateMsTime = now;
        }
        else
        {
            msCache = -1;
        }
    }
    return msCache;
}
