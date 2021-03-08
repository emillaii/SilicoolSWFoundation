#include "piaxis.h"

PIAxis::PIAxis(QString name, QObject *parent) : SCAxis(name, parent)
{

}

void PIAxis::initImpl()
{
    PIControlCardConfig *piCoreCfg = PICardConfigManager::getIns().getPICoreCfg();
    m_controllerID = piCoreCfg->controllerID();

    piAxisConfig = qobject_cast<PIAxisConfig *>(config());
    if (piAxisConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast AxisConfig to PIAxisConfig! AxisConfig type: %1")
                               .arg(config()->metaObject()->className()),
                           EX_LOCATION);
    }
}

void PIAxis::homeImpl()
{
    refrenceIfNeed();
}

bool PIAxis::isHomeDone() noexcept
{
    // Wait until the reference move is done.
    BOOL bFlag = FALSE;
    QElapsedTimer timer;
    timer.start();
    qInfo() << tr("Wait refrence PI finish!!");
    while (bFlag != TRUE)
    {
        if (PI_IsControllerReady( m_controllerID, &bFlag ) == FALSE)
        {
            throw ActionError(name(), QString("PI_IsControllerReady() in refrenceIfNeed() failed!"));
        }

        if(timer.elapsed() > piAxisConfig->refrenceOverTime())
        {
            PI_STP(m_controllerID);
            throw SilicolAbort(tr("Reference PI overtime, Overtime: %1").arg(piAxisConfig->refrenceOverTime()));
        }

        QThread::msleep(10);
    }
    qInfo() << tr("PI Home finished!! takes time: %1").arg(timer.elapsed());
    return true;
}

void PIAxis::stopImpl() noexcept
{
    if(PI_STP(m_controllerID) != TRUE)
    {
        qCritical() << tr("PI_STP() excute failed in %1 stopImpl()").arg(piAxisConfig->name());
    }
}

void PIAxis::stopHome()
{
    stopImpl();
}

void PIAxis::enableImpl()
{
    return;
    BOOL axisValue = TRUE;
    BOOL res = PI_EAX(m_controllerID, "X Y Z U V W", &axisValue);
    if (res != TRUE)
    {
        throw ActionError(name(), QString("PI_EAX() in enableImpl() failed!"));
    }
}

void PIAxis::disableImpl()
{
    BOOL axisValue = FALSE;
    BOOL res = PI_EAX(m_controllerID, "X Y Z U V W", &axisValue);
    if (res != TRUE)
    {
        throw ActionError(name(), QString("PI_EAX() in disableImpl() failed!"));
    }
}

void PIAxis::clearErrorImpl()
{
    //Get error status of the DLL and, if clear, that of the controller.
    //If the library shows an error condition, its code is returned, if not, the controller error code is checked using PI_qERR() and returned.
    //After this call the DLL internal error state will be cleared;
    //the controller error state will be cleared if it was queried.
    PI_GetError(m_controllerID);
}

void PIAxis::moveToImpl(double targetPos)
{
    /*
    //get current pos and handle targetPos
    double dPos[6] = { 0, 0 ,0 , 0, 0, 0 };
    if(PI_qPOS(m_controllerID, "X Y Z U V W", dPos) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_qPOS() failed in moveToImpl() !"));
    }

    QString axisName = piAxisConfig->name();
    int posIndex = getAxisIndexByName(axisName);
    dPos[posIndex] = targetPos;
    qInfo() << tr("finish handle Axis positions, reult: x: %1, y: %2, z:%3,"
                  " u: %4, v: %5, w: %6").arg(dPos[0]).arg(dPos[1]).arg(dPos[2]).arg(dPos[3]).arg(dPos[4]).arg(dPos[5]);
    //manager positions is possible
    BOOL bMovePossible = FALSE;
    if(PI_qVMO(m_controllerID, "X Y Z U V W", dPos, &bMovePossible) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_qVMO() failed in moveToImpl() !"));
    }
    if(bMovePossible == FALSE)
    {
        throw SilicolAbort(tr("can not move to Position: x: %1, y: %2, z:%3,"
                              " u: %4, v: %5, w: %6").arg(dPos[0]).arg(dPos[1]).arg(dPos[2]).arg(dPos[3]).arg(dPos[4]).arg(dPos[5]));
    }

    moveToPosition(dPos);
    */

    double dPos[1] = {0};
    dPos[0] = targetPos;
    BOOL bMovePossible = FALSE;
    if(PI_qVMO(m_controllerID, piAxisConfig->name().toUtf8(), dPos, &bMovePossible) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_qVMO() failed in %1 moveToImpl() !").arg(piAxisConfig->name()));
    }
    if(bMovePossible == FALSE)
    {
        throw SilicolAbort(tr("can not move to Position: %1: %2").arg(piAxisConfig->name()).arg(dPos[0]));
    }
    if (PI_MOV ( m_controllerID, piAxisConfig->name().toUtf8(), dPos) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_MOV() failed, in %1!").arg(piAxisConfig->name()));
    }

    BOOL bIsMoving[1] = { TRUE};

    QElapsedTimer timer;
    timer.start();

    while (TRUE == bIsMoving[0])
    {
        if (PI_IsMoving ( m_controllerID, piAxisConfig->name().toUtf8(), bIsMoving ) != TRUE)
        {
            throw SilicolAbort(tr("Excute PI_IsMoving() failed in %1 moveToPosition()!").arg(piAxisConfig->name()));
        }

        if(timer.elapsed() > piAxisConfig->moveOverTime())
        {
            PI_STP(m_controllerID);
            throw SilicolAbort(tr("wait for PI %1 axis finish moving overtime, Overtime: %2").arg(piAxisConfig->name()).arg(piAxisConfig->moveOverTime()));
        }
        Sleep (10);
    }
}

double PIAxis::getFeedbackPosImpl() noexcept
{
    /*double dPos[6] = { 0, 0 ,0 , 0, 0, 0 };
    if(PI_qPOS(m_controllerID, "X Y Z U V W", dPos) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_qPOS() failed in moveToImpl() !"));
    }

    QString axisName = piAxisConfig->name();
    int posIndex = getAxisIndexByName(axisName);
    return dPos[posIndex];
    */

    double dPos[1] = {0};

    if(PI_qPOS(m_controllerID, piAxisConfig->name().toUtf8(), dPos) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_qPOS() failed in %1 moveToImpl() !").arg(piAxisConfig->name()));
    }

    const int Precision = 100000;
    return round(dPos[0] * Precision) / Precision;
}

void PIAxis::refrenceIfNeed()
{
    BOOL bReferenced;
    char axis[] = "X";
    if (PI_qFRF( m_controllerID, axis, &bReferenced ) == FALSE)
    {
        throw ActionError(name(), QString("PI_qFRF() in refrenceIfNeed() failed!"));
    }

    qInfo() << tr("Need refrence PI state: %1").arg(bReferenced);
    if (bReferenced == FALSE)
    {
        qInfo() << tr("Start refrence PI!!");
        if (PI_FRF ( m_controllerID, axis ) == FALSE)
        {
            throw ActionError(name(), QString("PI_FRF() in refrenceIfNeed() failed!"));
        }
    }
    else
    {
        QString st = tr("PI has refrenced! Are you sure refrence PI by %1 Axis now?");
        if (UIOperation::getIns()->yesNoConfirm(st))
        {
            qInfo() << tr("Start refrence PI again!!");
            if (PI_FRF ( m_controllerID, axis ) == FALSE)
            {
                throw ActionError(name(), QString("PI_FRF() in refrenceIfNeed() failed!"));
            }
        }
    }
    qInfo() << tr("PI Home finished!! ");
}

void PIAxis::moveToPosition(double *positons)
{
    if (PI_MOV ( m_controllerID, "X Y Z U V W", positons ) != TRUE)
    {
        throw SilicolAbort(tr("excute PI_MOV() failed!"));
    }

    BOOL bIsMoving[6] = { TRUE, TRUE, TRUE, TRUE, TRUE, TRUE };

    QElapsedTimer timer;
    timer.start();

    while (( TRUE == bIsMoving[0] ) || ( TRUE == bIsMoving[1] ) ||
           ( TRUE == bIsMoving[2] ) || ( TRUE == bIsMoving[3] ) ||
           ( TRUE == bIsMoving[4] ) || ( TRUE == bIsMoving[5] ) )
    {
        if (PI_IsMoving ( m_controllerID, "X Y Z U V W", bIsMoving ) != TRUE)
        {
            throw SilicolAbort(tr("Excute PI_IsMoving() failed in moveToPosition()!"));
        }

        if(timer.elapsed() > piAxisConfig->moveOverTime())
        {
            PI_STP(m_controllerID);
            throw SilicolAbort(tr("wait for PI finish moving overtime, Overtime: %1").arg(piAxisConfig->moveOverTime()));
        }
        Sleep (10);
    }
}

int PIAxis::getAxisIndexByName(QString axisName)
{
    int posIndex = -1;
    if(axisName.compare("X") == 0)
    {
        posIndex = 0;
    }
    else if(axisName.compare("Y") == 0)
    {
        posIndex =1;
    }
    else if(axisName.compare("Z") == 0)
    {
        posIndex =2;
    }
    else if(axisName.compare("U") == 0)
    {
        posIndex =3;
    }
    else if(axisName.compare("V") == 0)
    {
        posIndex =4;
    }
    else if(axisName.compare("W") == 0)
    {
        posIndex =5;
    }
    else
    {
        throw ActionError(name(), tr("UnDefine Axis, name: %1").arg(axisName));
    }
    return posIndex;
}

bool PIAxis::isInPos() noexcept
{
    BOOL bIsMoving = FALSE;
    BOOL ret = PI_IsMoving( m_controllerID, piAxisConfig->name().toUtf8(), &bIsMoving);
    if (ret != TRUE)
    {
        qCCritical(motionCate()) << tr("PI_IsMoving() in %1 isInPos() failed!").arg(name());
        return false;
    }
    return bIsMoving == TRUE ? true : false;
}

bool PIAxis::isRunning() noexcept
{
    return isInPos();
}

double PIAxis::getCurrentVel() noexcept
{
    double currentVel = 0;
    BOOL ret = PI_qVEL(m_controllerID, piAxisConfig->name().toUtf8().data(), &currentVel);
    if (ret != TRUE)
    {
        qCCritical(motionCate()) << tr("PI_qVEL() in %1 getCurrentVel() failed!").arg(name());
        return 0;
    }
    return currentVel;
}

bool PIAxis::hasAlarm() noexcept
{
    int errCode = 0;
    //Get the error state of the controller.
    //Because the DLL may have queried (and cleared) controller error conditions on its own,
    //it is safer to call PI_GetError() which will first check the internal error state of the library.
    BOOL ret = PI_qERR(m_controllerID, &errCode);
    if (ret != TRUE)
    {
        qCCritical(motionCate()) << tr("PI_qERR() in %1 hasAlarm() failed!").arg(name());
        return true;
    }

    return  errCode == TRUE ? true : false;
}
