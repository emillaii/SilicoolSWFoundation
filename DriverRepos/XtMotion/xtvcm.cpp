#include "xtvcm.h"

int XtVcm::vcmCount = 0;
QList<VCM_Resource_struct> XtVcm::vcmResources;
QList<XtVcm *> XtVcm::xtVcms;
bool XtVcm::isInitAllVcm = false;
QMutex XtVcm::getNowPosLocker;
QMutex XtVcm::setSoftLandingParamLocker;

XtVcm::XtVcm(QString name, QObject *parent) : XtAxis(name, parent)
{
    vcmId = ++vcmCount;
    masterAxisId = XtAxis::generateNewAxisID();
    slaverAxisId = masterAxisId;
    threadId = masterAxisId + 20;
    softlandingSlotId = XtAxis::generateNewCalcSlotID();

    originId = XtDI::generateNewDiID();
    origin2Id = XtDI::generateNewDiID();
    xtVcms.append(this);
}

void XtVcm::initAllVcm()
{
    if (xtVcms.isEmpty())
    {
        return;
    }
    if (!isInitAllVcm)
    {
        vcmResources.clear();
        foreach (auto xtVcm, xtVcms)
        {
            xtVcm->createVcmResource();
        }
        VCM_Resource_struct *resources = new VCM_Resource_struct[vcmResources.count()];
        for (int i = 0; i < vcmResources.count(); i++)
        {
            resources[i] = vcmResources[i];
        }
        VCMT_resource_alloc(resources, vcmResources.count());
        Soft_landing_dll_init(1);
        if (!silicoolWait(10000, [] { return Get_Init_Ready() == 1; }))
        {
            throw SilicolAbort(tr("Wait all vcm init ready timeout!"), EX_LOCATION);
        }
        SetDebugLog(1);
        isInitAllVcm = true;
    }
}

void XtVcm::releaseAllVcm()
{
    if (xtVcms.isEmpty())
    {
        return;
    }
    if (isInitAllVcm)
    {
        foreach (auto xtVcm, xtVcms)
        {
            xtVcm->stopImpl();
        }

        Soft_landing_dll_uninit();
        isInitAllVcm = false;
    }
}

void XtVcm::createVcmResource()
{
    VCM_Resource_struct vcmResource;
    vcmResource.CanID = vcmConfig->canID();
    vcmResource.iAxis = masterAxisId;
    vcmResource.IO_ID = originId;
    vcmResource.Z_Index_ID = origin2Id;
    vcmResource.iThread = threadId;
    vcmResource.iThread_Curve = XtAxis::generateNewThreadID();
    vcmResource.Connet_Rebuild = 0;
    vcmResources.append(vcmResource);
}

void XtVcm::setMoveDirection(bool beforeHome)
{
    int dir = beforeHome ? vcmConfig->homeDirection() : vcmConfig->runDirection();
    isDirectionOpposite = (dir != 0);
    SetRunDirect(vcmId, dir, vcmConfig->scale());
}

void XtVcm::setSoftLandingParameter(double vel, double targetPos, double force, double margin)
{
    QMutexLocker tmpLocker(&setSoftLandingParamLocker);
    SetFastSpeed(vcmId, vcmConfig->maxVel() * vcmConfig->velocityRatio());
    SetFastAcc(vcmId, vcmConfig->maxAcc());
    SetSlowSpeed(vcmId, vel);
    SetSlowUpSpeed(vcmId, vcmConfig->maxVel() * vcmConfig->velocityRatio());
    SetSlowAcc(vcmId, vcmConfig->maxAcc());
    SetForce(vcmId, force);
    SetSoftPos(vcmId, targetPos);
    SetSoftLandingMargin(vcmId, margin);
    double currentPos = getCurrentOutputPos();
    SetZeroPos(vcmId, currentPos);
    UpdataParam(vcmId);
}

void XtVcm::checkHomeRes(int res)
{
    if (res == 0)
    {
        throw ActionError("VCM", tr("Home %1 failed!").arg(name()));
    }
    else if (res == -1)
    {
        throw ActionError("VCM", tr("Home %1 timeout!").arg(name()));
    }
}

void XtVcm::setConfig(AxisConfig *config)
{
    vcmConfig = qobject_cast<XtVcmConfig *>(config);
    if (vcmConfig == nullptr)
    {
        qCritical() << "Can not cast config to XtVcmConfig!"
                    << "Config type:" << config->metaObject()->className();
    }
    XtAxis::setConfig(config);
}

bool XtVcm::hasAlarm() noexcept
{
    int errCode = get_motor_error(vcmId);
    if (errCode != 0)
    {
        needReHome();
        QString errMsg;
        switch (errCode)
        {
            case 1:
            {
                errMsg = tr("error_over_current");
                break;
            }
            case 1 << 1:
            {
                errMsg = tr("error_go_zero");
                break;
            }
            case 1 << 2:
            {
                errMsg = tr("error_over_temp");
                break;
            }
            case 1 << 3:
            {
                errMsg = tr("error_leakage_current");
                break;
            }
            case -1:
            {
                errMsg = tr("ControllerError");
                break;
            }
            default:
            {
                errMsg = tr("Unknown");
                break;
            }
        }
        qCritical(motionCate()) << tr("VCM %1 error, errCode:%2, errMsg: %3").arg(name()).arg(errCode).arg(errMsg);
        return true;
    }
    else
    {
        return false;
    }
}

void XtVcm::clearErrorImpl()
{
    int res = ResetDevice(vcmId);
    qCInfo(motionCate()) << tr("%1 clear error. Result: %2.").arg(name()).arg(res);
}

bool XtVcm::isInPos() noexcept
{
    return CheckPosReady(vcmId) == 1 && XtAxis::isInPos();
}

void XtVcm::fastSoftLanding(double vel, double targetPos)
{
    double velRatio = vel / vcmConfig->maxVel();
    scaleMaxVelImpl(velRatio);
    XtAxis::moveToImpl(targetPos);
    QList<double> feedbackPos;
    QElapsedTimer timer;
    bool gotIt;
    double ave;
    const double fslInposError = qAbs(vcmConfig->fslInPosError());
    QThread::msleep(20);
    while (true)
    {
        feedbackPos.append(getFeedbackPosImpl());

        gotIt = true;
        int startIndex = feedbackPos.count() - vcmConfig->fslWindowLen();
        if (startIndex >= 0)
        {
            double sum = 0;
            for (int i = startIndex; i < feedbackPos.count(); i++)
            {
                sum += feedbackPos[i];
            }
            ave = sum / vcmConfig->fslWindowLen();
            for (int i = startIndex; i < feedbackPos.count(); i++)
            {
                if (qAbs(ave - feedbackPos[i]) > fslInposError)
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
            resetMaxVel();
            if (qAbs(ave - targetPos) <= fslInposError)
            {
                qCWarning(motionCate()) << QString("Fast soft landing reached target pos. %1 may not touch something.").arg(name());
            }
            else
            {
                XtAxis::moveToImpl(ave);
            }
            break;
        }

        timer.restart();
        while (timer.elapsed() < vcmConfig->fslDetectionInterval())
        {
        }
    }
}

void XtVcm::scaleMaxAccImpl(double ratio)
{
    SetPosModeAcc(vcmId, vcmConfig->maxAcc() * ratio);
    SET_MAX_ACC(threadId, masterAxisId, vcmConfig->maxAcc() * ratio);
}

void XtVcm::initImpl()
{
    setMoveDirection(false);
    SetCurrentLimit(vcmId, vcmConfig->maxCurrentLimit(), vcmConfig->minCurrentLimit());
    SetPosLimit(vcmId, vcmConfig->positiveLimit(), vcmConfig->negativeLimit());
    SetPosModejerk(vcmId, vcmConfig->maxJerk());
    SetSoftlandingSlot(vcmId, softlandingSlotId);

    if (vcmConfig->needMapCurrent2Force())
    {
        QString currenArray = vcmConfig->currentArray();
        QString forceArray = vcmConfig->forceArray();
        auto sCurrent = currenArray.split(",");
        auto sForce = forceArray.split(",");
        QString errMsg = QString("\r\nVcm: %1, currentArray: %2, forceArray: %3").arg(name()).arg(currenArray).arg(forceArray);
        if (sCurrent.length() != sForce.length())
        {
            throw SilicolAbort(tr("Current array length must equal to force array length!") + errMsg, EX_LOCATION);
        }
        if (sCurrent.length() < 4)
        {
            throw SilicolAbort(tr("Array length must greater than 3!") + errMsg, EX_LOCATION);
        }
        double *dCurrent = new double[sCurrent.length()];
        double *dForce = new double[sForce.length()];
        bool isOk = false;
        for (int i = 0; i < sCurrent.length(); i++)
        {
            double value = sCurrent[i].toDouble(&isOk);
            if (!isOk)
            {
                throw SilicolAbort(tr("Parse current string to double error! Index: %1").arg(i) + errMsg, EX_LOCATION);
            }
            dCurrent[i] = value;
        }
        for (int i = 0; i < sForce.length(); i++)
        {
            double value = sForce[i].toDouble(&isOk);
            if (!isOk)
            {
                throw SilicolAbort(tr("Parse force string to double error! Index: %1").arg(i) + errMsg, EX_LOCATION);
            }
            dForce[i] = value;
        }
        MapCurrent2Force(vcmId, dCurrent, dForce, sCurrent.length());
    }
}

void XtVcm::homeImpl()
{
    setMoveDirection(true);
    double maxGoZeroDistance = (vcmConfig->positiveLimit() - vcmConfig->negativeLimit()) * 2;
    maxGoZeroDistance = qMax(maxGoZeroDistance, 21.0);    // ????Bug, maxGoZeroDistance????????20
    XtVcmConfig::HomeMode homeMode = vcmConfig->homeMode();
    int res = -1;
    if (homeMode == XtVcmConfig::TouchFindIndexIO)
    {
        qCInfo(motionCate()) << tr("Home %1, mode: TouchFindIndexIO").arg(name());
        res = Touch_Go_Zero(vcmId, vcmConfig->touchLimitCurrent(), maxGoZeroDistance);
    }
    else if (homeMode == XtVcmConfig::FindZeroIOThenIndexIO)
    {
        qCInfo(motionCate()) << tr("Home %1, mode: FindZeroIOThenIndexIO").arg(name());
        if (!gotIndexIO)
        {
            int zIndexBefore = GetZindexIO(vcmId);
            qCInfo(motionCate()) << tr("%1 Try to go zero and find index io... Index io before go zero: %2").arg(name()).arg(zIndexBefore);
            SetZeroPos(vcmId, 0);
            SetGoZeroDistance(vcmId, maxGoZeroDistance);
            res = Init_Go_Zero(vcmId);
            checkHomeRes(res);
            int zIndexAfter = GetZindexIO(vcmId);
            qCInfo(motionCate()) << tr("%1 Index io after go zero: %2").arg(name()).arg(zIndexAfter);
            if (zIndexBefore == zIndexAfter)
            {
                double targetPos = vcmConfig->findIndexIOMaxDistance();
                qCInfo(motionCate()) << tr("%1 Try move to %2 to find index io...").arg(name()).arg(targetPos);
                SET_MAX_VEL(threadId, masterAxisId, qMin(10.0, vcmConfig->findIndexIOMaxVelocity()));
                SGO(threadId, masterAxisId, targetPos);
                QThread::msleep(50);
                while (true)
                {
                    zIndexAfter = GetZindexIO(vcmId);
                    if (zIndexAfter != zIndexBefore)
                    {
                        qCDebug(motionCate()) << tr("%1 got index io jump.").arg(name());
                        gotIndexIO = true;
                        XtAxis::stopImpl();
                        break;
                    }
                    if (!XtAxis::isRunning())
                    {
                        qCDebug(motionCate()) << tr("VCM %1 stopped.").arg(name());
                        break;
                    }
                    QThread::msleep(15);
                }
                resetMaxVel();
                if (!gotIndexIO)
                {
                    throw SilicolAbort(tr("%1 Find index io failed!").arg(name()), EX_LOCATION);
                }
            }
            else
            {
                gotIndexIO = true;
            }
        }
        res = Touch_Go_Zero(vcmId, vcmConfig->touchLimitCurrent(), 0, vcmConfig->findIndexIODir());
    }
    else
    {
        qCInfo(motionCate()) << tr("Home %1, mode: FindZeroIO").arg(name());
        SetZeroPos(vcmId, 0);
        SetGoZeroDistance(vcmId, maxGoZeroDistance);
        res = Init_Go_Zero(vcmId);
    }

    setMoveDirection(false);
    resetMaxVel();
    checkHomeRes(res);
}

bool XtVcm::isHomeDone() noexcept
{
    return true;
}

void XtVcm::operationAfterHome() {}

void XtVcm::stopImpl() noexcept
{
    StopVmc(vcmId);
    XtAxis::stopImpl();
}

void XtVcm::enableImpl()
{
    SetServoOnOff(vcmId, true);
}

void XtVcm::disableImpl()
{
    gotIndexIO = false;
    SetServoOnOff(vcmId, false);
}

double XtVcm::getFeedbackPosImpl() noexcept
{
    double pos = -9999;
    int currentTimes = 0;
    while (true)
    {
        int res;
        {
            QMutexLocker tmpLocker(&getNowPosLocker);
            res = GetNowPos(vcmId, pos);
        }
        if (res == 1)
        {
            pos = round(pos * 100000) / 100000;
//            if (isDirectionOpposite)
//            {
//                pos = -pos;
//            }
            return pos;
        }
        if (++currentTimes > 20)
        {
            currentTimes = 0;
            qCWarning(motionCate()) << tr("Axis %1 get feedback pos failed!").arg(name());
        }
        QThread::msleep(1);
    }
}

void XtVcm::moveToImpl(double targetPos)
{
    if (isSoftlandingDown())
    {
        qWarning(motionCate()) << tr("??????????????????????????????????????????????????????");
        softLandUp();
    }
    XtAxis::moveToImpl(targetPos);
}

void XtVcm::scaleMaxVelImpl(double ratio)
{
    SetPosModeSpeed(vcmId, vcmConfig->maxVel() * ratio);
    SET_MAX_VEL(threadId, masterAxisId, vcmConfig->maxVel() * ratio);
}

void XtVcm::clearPosImpl()
{
    SetZero(vcmId);
    XtAxis::clearPosImpl();
}

void XtVcm::syncProfilerAndFeedbackPos()
{
    SET_AXIS_ZEROS(threadId, masterAxisId, getFeedbackPosImpl());
}

void XtVcm::softLandDownImpl(double vel, double targetPos, double force, double margin)
{
    setSoftLandingParameter(vel, targetPos, force, margin);
    int res = Do_SoftDown(vcmId);
    if (res != 0)
    {
        throw ActionError("VCM", tr("%1 Soft land down failed! Error code: %2").arg(name()).arg(res));
    }
}

void XtVcm::softLandUpImpl()
{
    int res = Do_SoftUp(vcmId);
    if (res != 0)
    {
        throw ActionError("VCM", tr("%1 Soft land up failed! Error code: %2").arg(name()).arg(res));
    }
}

bool XtVcm::isSoftLandDownFinished() noexcept
{
    return Get_Busy(vcmId, 0) == 0;
}

bool XtVcm::isSoftLandUpFinished() noexcept
{
    return Get_Busy(vcmId, 1) == 0;
}

void XtVcm::showZeroIoState(bool isShow)
{
    if (!isInit())
    {
        qCCritical(motionCate()) << tr("Axis %1 is not inited!").arg(name());
        return;
    }

    if (isShow)
    {
        if (isShowZeroIOState)
        {
            return;
        }
        int zeroIo;
        int zIndexIo;
        isShowZeroIOState = true;
        while (isShowZeroIOState)
        {
            zeroIo = GetZeroIO(vcmId);
            zIndexIo = GetZindexIO(vcmId);
            qCInfo(motionCate()) << tr("Axis: %1, Zero io: %2, ZIndex io: %3").arg(name()).arg(zeroIo).arg(zIndexIo);
            QThread::msleep(100);
        }
    }
    else
    {
        isShowZeroIOState = false;
    }
}
