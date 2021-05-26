#include "xtaxis.h"

int XtAxis::axisCount = -1;
int XtAxis::threadResource = 0;
int XtAxis::calculationSlotResource = 0;

XtAxis::XtAxis(QString name, QObject *parent) : SCAxis(name, parent) {}

int XtAxis::generateNewAxisID()
{
    if (axisCount < 0)
    {
        axisCount = Profile_Get_Axis_Count();
    }
    return ++axisCount;
}

int XtAxis::generateNewThreadID()
{
    return ++threadResource;
}

int XtAxis::generateNewCalcSlotID()
{
    return ++calculationSlotResource;
}

double XtAxis::getCurrentOutputPos() noexcept
{
    double currentPos = 0;
    if (Get_Cur_Axis_Pos(masterAxisId, currentPos) != 1)
    {
        qCCritical(motionCate()) << tr("Get axis pos failed! %1").arg(name());
    }
    return currentPos;
}

bool XtAxis::hasAlarm() noexcept
{
    if (alarmDI != nullptr)
    {
        bool res = alarmDI->get();
        if (xtAxisConfig->reverseAlarmInput())
        {
            res = !res;
        }
        if (res)
        {
            needReHome();
        }
        return res;
    }
    return false;
}

bool XtAxis::isReady() noexcept
{
    if (readyDI != nullptr)
    {
        bool res = readyDI->get();
        if (xtAxisConfig->reverseReadyInput())
        {
            return !res;
        }
        else
        {
            return res;
        }
    }
    return true;
}

bool XtAxis::isInPos() noexcept
{
    if (inposDI != nullptr)
    {
        bool res = inposDI->get();
        return xtAxisConfig->reverseInposInput() ? !res : res;
    }
    return true;
}

bool XtAxis::isRunning() noexcept
{
    int isRun = 1;
    double vel = 0;
    Get_Cur_Axis_State(masterAxisId, isRun);
    Get_Cur_Axis_Vel(masterAxisId, vel);
    return isRun || !qFuzzyCompare(vel, 0);
}

void XtAxis::stopImpl() noexcept
{
    ClearInsBuffer(threadId);
    STOP_S(threadId, masterAxisId);
}

double XtAxis::getCurrentVel() noexcept
{
    double vel = 0;
    Get_Cur_Axis_Vel(masterAxisId, vel);
    return round(vel * 100000) / 100000;
}

void XtAxis::initImpl()
{
    if (isInit())
    {
        return;
    }
    xtAxisConfig = qobject_cast<XtAxisConfig *>(config());
    if (xtAxisConfig == nullptr)
    {
        throw SilicolAbort(tr("Can not cast axis config to type: XtAxisConfig!"), EX_LOCATION);
    }
    slaverAxisId = Profile_Find_Axis_Name(const_cast<LPWSTR>(name().toStdWString().c_str()));
    if (slaverAxisId < 0)
    {
        throw SilicolAbort(tr("Did not find axis: %1").arg(name()), EX_LOCATION);
    }
    threadId = slaverAxisId + 20;
    masterAxisId = slaverAxisId;

    int enableDoID = Profile_Get_Axis_Enable_Output_ID(slaverAxisId);
    if (enableDoID < 0)
    {
        qCWarning(motionCate()) << tr("Did not find enable output in axis: %1").arg(name());
    }
    else
    {
        QString enableDOName = QString::fromStdWString(Profile_Get_IoOut_Name(enableDoID));
        enableDO = new XtDO(enableDOName, enableDoID, this);
    }

    int clearErrorDoID = Profile_Get_Axis_ClrAlm_Output_ID(slaverAxisId);
    if (clearErrorDoID < 0)
    {
        qCWarning(motionCate()) << tr("Did not find clearError output in axis: %1").arg(name());
    }
    else
    {
        QString clearErrorDoName = QString::fromStdWString(Profile_Get_IoOut_Name(clearErrorDoID));
        clearErrorDO = new XtDO(clearErrorDoName, clearErrorDoID, this);
    }

    int inposDiID = Profile_Get_Axis_InPos_Input_ID(slaverAxisId);
    if (inposDiID < 0)
    {
        qCWarning(motionCate()) << tr("Did not find inPos input in axis: %1").arg(name());
    }
    else
    {
        QString inposDiName = QString::fromStdWString(Profile_Get_IoIn_Name(inposDiID));
        inposDI = new XtDI(inposDiName, inposDiID, this);
    }

    int alarmDiID = Profile_Get_Axis_Alarm_Input_ID(slaverAxisId);
    if (alarmDiID < 0)
    {
        qCWarning(motionCate()) << tr("Did not find alarm input in axis: %1").arg(name());
    }
    else
    {
        QString alarmDiName = QString::fromStdWString(Profile_Get_IoIn_Name(alarmDiID));
        alarmDI = new XtDI(alarmDiName, alarmDiID, this);
    }

    int readyDiID = Profile_Get_Axis_Ready_Input_ID(slaverAxisId);
    if (readyDiID < 0)
    {
        qCWarning(motionCate()) << tr("Did not find ready input in axis: %1").arg(name());
    }
    else
    {
        QString readyDiName = QString::fromStdWString(Profile_Get_IoIn_Name(readyDiID));
        readyDI = new XtDI(readyDiName, readyDiID, this);
    }

    m_maxVel = Profile_Get_Axis_Vel(slaverAxisId);
    m_maxAcc = Profile_Get_Axis_Acc(slaverAxisId);
    m_maxJerk = Profile_Get_Axis_Jerk(slaverAxisId);

    m_positiveLimit = Profile_Get_Axis_MaxPos(slaverAxisId);
    m_negativeLimit = Profile_Get_Axis_MinPos(slaverAxisId);

    int tmp_masterId = Profile_Get_Axis_Master(slaverAxisId);
    if (tmp_masterId > 0 && tmp_masterId != masterAxisId)
    {
        if (1 != Set_Axis_Bind(slaverAxisId, tmp_masterId, 1, 1))
        {
            throw SilicolAbort(tr("Axis %1 bind to master axis failed! MasterId: %2, SlaverId: %3.").arg(name()).arg(masterAxisId).arg(slaverAxisId),
                               EX_LOCATION);
        }
        masterAxisId = tmp_masterId;
        WaitForAllInsFinish(threadId);
    }
    SET_MAX_JERK(threadId, masterAxisId, m_maxJerk);
}

void XtAxis::homeImpl()
{
    PROFILE_AXIS_SEEK_ORIGIN(threadId, slaverAxisId);
}

bool XtAxis::isHomeDone() noexcept
{
    int insBufferLen, isFinish;
    GetThreadInsBufferRemainCount(threadId, insBufferLen, isFinish);
    return insBufferLen == 0 && isFinish == 1;
}

QString XtAxis::homeErrorMsg()
{
    return "";
}

void XtAxis::operationAfterHome() {}

void XtAxis::stopHome()
{
    stop();
}

void XtAxis::enableImpl()
{
    if (enableDO != nullptr)
    {
        enableDO->set(true);
    }
}

void XtAxis::disableImpl()
{
    if (enableDO != nullptr)
    {
        enableDO->set(false);
    }
}

void XtAxis::clearErrorImpl()
{
    if (clearErrorDO != nullptr)
    {
        clearErrorDO->set(true);
        QThread::msleep(50);
        clearErrorDO->set(false);
    }
}

void XtAxis::moveToImpl(double targetPos)
{
    SGO(threadId, masterAxisId, targetPos);
}

void XtAxis::clearPosImpl()
{
    SET_AXIS_ZEROS(threadId, masterAxisId, 0);
}

void XtAxis::syncProfilerAndFeedbackPos() {}

void XtAxis::velocityMoveImpl(SCAxis::Direction dir, double vel, double acc)
{
    SET_MAX_VEL(threadId, masterAxisId, vel);
    SET_MAX_ACC(threadId, masterAxisId, acc);
    if (dir == Direction::Positive)
    {
        SGO_INCREASE(threadId, masterAxisId, INT_MAX);
    }
    else
    {
        SGO_INCREASE(threadId, masterAxisId, INT_MIN);
    }
}

double XtAxis::getFeedbackPosImpl() noexcept
{
    // Todo
    return getCurrentOutputPos();
}

void XtAxis::scaleMaxAccImpl(double ratio)
{
    SET_MAX_ACC(threadId, masterAxisId, m_maxAcc * ratio);
}

void XtAxis::scaleMaxVelImpl(double ratio)
{
    SET_MAX_VEL(threadId, masterAxisId, m_maxVel * ratio);
}

bool XtAxis::bindToMasterAxisImpl(bool bind)
{
    qCInfo(motionCate(), N_FUNC_ARG(bind));

    auto masterXtAxis = qobject_cast<XtAxis *>(masterAxis());
    if (masterXtAxis == nullptr)
    {
        qCCritical(motionCate()) << tr("Master axis is null!");
        return false;
    }
    if (bind)
    {
        if (1 != Set_Axis_Bind(slaverAxisId, masterXtAxis->masterAxisId, 1, 1))
        {
            return false;
        }
        masterAxisId = masterXtAxis->masterAxisId;
        WaitForAllInsFinish(threadId);
        return true;
    }
    else
    {
        if (1 != Set_Axis_Bind(slaverAxisId, masterXtAxis->masterAxisId, 1, 0))
        {
            return false;
        }
        WaitForAllInsFinish(threadId);
        masterAxisId = slaverAxisId;
        return true;
    }
}
