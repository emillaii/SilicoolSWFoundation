#include "xyzmodule.h"

XYZModulePos::XYZModulePos() : XYModulePos()
{
    m_transitionPosPrecisions = new ConfigObjectArray(&MoveSequenceTransitionPosPrecision::staticMetaObject, this);
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &XYZModulePos::onTimeout);
    int onTransitionPosPrecisionConfigChangedSlotIndex
        = getMethodIndex(&staticMetaObject, QMetaMethod::Slot, "onTransitionPosPrecisionConfigChanged");
    m_transitionPosPrecisions->uniquelyConnectConfigChangedSignalToSlot(this, onTransitionPosPrecisionConfigChangedSlotIndex, true);

    QStringList args;
    args << "zPos"
         << "safetyMoveSequence"
         << "transitionPosPrecisions";
    setEngineerAuthorities(args);
    init();
}

void XYZModulePos::onTransitionPosPrecisionConfigChanged()
{
    timer.start(30);
}

void XYZModulePos::onTimeout()
{
    transitionPosPrecisionMap.clear();
    for (int i = 0; i < m_transitionPosPrecisions->count(); i++)
    {
        auto transitionPosPrecision = m_transitionPosPrecisions->getConfig<MoveSequenceTransitionPosPrecision>(i);
        transitionPosPrecisionMap[transitionPosPrecision->moveSequence()] = transitionPosPrecision;
    }
}

XYZModuleConfig::XYZModuleConfig(QObject *parent) : AxisModuleConfig(&XYZModulePos::staticMetaObject, parent)
{
    QStringList args;
    args << "xAxisName"
         << "yAxisName"
         << "zAxisName"
         << "zSafetyPos"
         << "xSafetyPos"
         << "ySafetyPos"
         << "useZSafetyPos"
         << "useXSafetyPos"
         << "useYSafetyPos";
    setEngineerAuthorities(args);
    init();
    RenameManager::getIns().subscribeAxisNameChanged(this, "xAxisName");
    RenameManager::getIns().subscribeAxisNameChanged(this, "yAxisName");
    RenameManager::getIns().subscribeAxisNameChanged(this, "zAxisName");
}

XYZModule::XYZModule(QString name, XYZModuleConfig *xyzModuleConfig, QObject *parent)
    : AxisModule(xyzModuleConfig, parent), xyzModuleConfig(xyzModuleConfig)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<XYZModule>(name, this);
    recordPos = new XYZModulePos();
}

void XYZModule::moveToPos(double x, double y, double z, MoveSequence::Sequence safetyMoveSequence, bool waitDone)
{
    const double MinStep = 0.00001;

    QString safetyMoveSeq = MoveSequence::SequenceEnumInfo().enumToName(safetyMoveSequence);
    if (m_xAxis->isProfilerAtPos(x, MinStep) && m_yAxis->isProfilerAtPos(y, MinStep) && m_zAxis->isProfilerAtPos(z, MinStep))
    {
        return;
    }
    qCInfo(motionCate(), N_FUNC_ARG(safetyMoveSeq, x, y, z, waitDone));

    auto groups = safetyMoveSeq.split("_");
    for (int i = 0; i < groups.length(); i++)
    {
        QString group = groups[i];
        QList<AxisTargetInfo> axesTargetInfo;
        for (int j = 0; j < group.length(); j++)
        {
            auto axisName = group[j];
            if (axisName == "S")
            {
                if (axesTargetInfo.count() == 0)
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
                auto lastAxisName = group[j - 1];
                if (lastAxisName == "X")
                {
                    if (!xyzModuleConfig->useXSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 x to safety pos, but useXSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->xSafetyPos();
                }
                else if (lastAxisName == "Y")
                {
                    if (!xyzModuleConfig->useYSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 y to safety pos, but useYSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->ySafetyPos();
                }
                else if (lastAxisName == "Z")
                {
                    if (!xyzModuleConfig->useZSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 z to safety pos, but useZSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->zSafetyPos();
                }
                else
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
            }
            else
            {
                if (axisName == "X")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_xAxis, x));
                }
                else if (axisName == "Y")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_yAxis, y));
                }
                else if (axisName == "Z")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_zAxis, z));
                }
                else
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
            }
        }
        foreach (auto axisTargetInfo, axesTargetInfo)
        {
            axisTargetInfo.axis->absMove(axisTargetInfo.targetPos, false);
        }

        if (i < groups.length() - 1)
        {
            foreach (auto axisTargetInfo, axesTargetInfo)
            {
                axisTargetInfo.axis->waitProfilerStopped(axisTargetInfo.targetPos);
            }
        }
        else
        {
            if (waitDone)
            {
                foreach (auto axisTargetInfo, axesTargetInfo)
                {
                    axisTargetInfo.axis->waitArrivedPos(axisTargetInfo.targetPos, 0.1);
                }
            }
        }
    }
}

void XYZModule::moveToWithOffset(
    QString posName, MoveSequence::Sequence safetyMoveSequence, double xOffset, double yOffset, double zOffset, bool waitDone, bool logIt)
{
    QString safetyMoveSeq = MoveSequence::SequenceEnumInfo().enumToName(safetyMoveSequence);
    XYZModulePos *pos = (posName == RecordPosName ? recordPos : xyzModuleConfig->getPos<XYZModulePos>(posName));
    if (m_xAxis->isProfilerAtPos(pos->xPos() + xOffset, pos->xPrecision()) && m_yAxis->isProfilerAtPos(pos->yPos() + yOffset, pos->yPrecision())
        && m_zAxis->isProfilerAtPos(pos->zPos() + zOffset, pos->zPrecision()))
    {
        return;
    }
    if (logIt)
    {
        qCInfo(motionCate(), N_FUNC_ARG(posName, safetyMoveSeq, xOffset, yOffset, zOffset));
    }

    auto groups = safetyMoveSeq.split("_");
    for (int i = 0; i < groups.length(); i++)
    {
        QString group = groups[i];
        QList<AxisTargetInfo> axesTargetInfo;
        for (int j = 0; j < group.length(); j++)
        {
            auto axisName = group[j];
            if (axisName == "S")
            {
                if (axesTargetInfo.count() == 0)
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
                auto lastAxisName = group[j - 1];
                if (lastAxisName == "X")
                {
                    if (!xyzModuleConfig->useXSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 x to safety pos, but useXSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->xSafetyPos();
                }
                else if (lastAxisName == "Y")
                {
                    if (!xyzModuleConfig->useYSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 y to safety pos, but useYSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->ySafetyPos();
                }
                else if (lastAxisName == "Z")
                {
                    if (!xyzModuleConfig->useZSafetyPos())
                    {
                        throw SilicolAbort(tr("Try to move %1 z to safety pos, but useZSafetyPos is false!").arg(name()), EX_LOCATION);
                    }
                    axesTargetInfo[axesTargetInfo.count() - 1].targetPos = xyzModuleConfig->zSafetyPos();
                }
                else
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
                axesTargetInfo[axesTargetInfo.count() - 1].velRatio = axesTargetInfo[axesTargetInfo.count() - 1].axis->config()->velocityRatio();
                axesTargetInfo[axesTargetInfo.count() - 1].accRatio = 1;
            }
            else
            {
                if (axisName == "X")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_xAxis, pos->xPos() + xOffset, pos->xPrecision(), pos->xVelRatio(), pos->xAccRatio()));
                }
                else if (axisName == "Y")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_yAxis, pos->yPos() + yOffset, pos->yPrecision(), pos->yVelRatio(), pos->yAccRatio()));
                }
                else if (axisName == "Z")
                {
                    axesTargetInfo.append(AxisTargetInfo(m_zAxis, pos->zPos() + zOffset, pos->zPrecision(), pos->zVelRatio(), pos->zAccRatio()));
                }
                else
                {
                    throw SilicolAbort(tr("Unkown safetyMoveSeq %1(%2, %3)").arg(safetyMoveSeq).arg(i).arg(j), EX_LOCATION);
                }
            }
        }
        foreach (auto axisTargetInfo, axesTargetInfo)
        {
            bool hasMoveThisAxis = false;
            for (int k = 0; k < i; k++)
            {
                if (groups[k].contains(axisTargetInfo.axis->name()))
                {
                    hasMoveThisAxis = true;
                    break;
                }
            }
            if (hasMoveThisAxis)
            {
                axisTargetInfo.axis->absForceMove(axisTargetInfo.targetPos);
            }
            else
            {
                double velRatio = useVelInPosCfg() ? axisTargetInfo.velRatio : -1;
                double accRatio = useAccInPosCfg() ? axisTargetInfo.accRatio : -1;
                axisTargetInfo.axis->setNextMoveVelAccRatio(velRatio, accRatio);
                axisTargetInfo.axis->absMove(axisTargetInfo.targetPos, false);
            }
        }
        if (i < groups.length() - 1)
        {
            double transitionPosPrecision = -1;
            auto transitionPosPrecisionCfg = pos->transitionPosPrecision(safetyMoveSequence);
            if (transitionPosPrecisionCfg != nullptr)
            {
                transitionPosPrecision = transitionPosPrecisionCfg->transitionPosPrecision(i);
            }
            if (transitionPosPrecision > 0)
            {
                foreach (auto axisTargetInfo, axesTargetInfo)
                {
                    axisTargetInfo.axis->waitApproachPos(axisTargetInfo.targetPos, transitionPosPrecision);
                }
            }
            else
            {
                foreach (auto axisTargetInfo, axesTargetInfo)
                {
                    axisTargetInfo.axis->waitProfilerStopped(axisTargetInfo.targetPos);
                }
            }
        }
        else
        {
            if (waitDone)
            {
                foreach (auto axisTargetInfo, axesTargetInfo)
                {
                    axisTargetInfo.axis->waitArrivedPos(axisTargetInfo.targetPos, axisTargetInfo.precision);
                }
            }
        }
    }
}

void XYZModule::waitArrivedPosWithOffset(QString posName, double xOffset, double yOffset, double zOffset)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos() + xOffset, pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos() + yOffset, pos->yPrecision());
    m_zAxis->waitArrivedPos(pos->zPos() + zOffset, pos->zPrecision());
}

void XYZModule::relMove(double xStep, double yStep, double zStep)
{
    if (!qFuzzyCompare(xStep, 0))
    {
        m_xAxis->relMove(xStep, false);
    }
    if (!qFuzzyCompare(yStep, 0))
    {
        m_yAxis->relMove(yStep, false);
    }
    if (!qFuzzyCompare(zStep, 0))
    {
        m_zAxis->relMove(zStep, false);
    }
    if (!qFuzzyCompare(xStep, 0))
    {
        m_xAxis->waitArrivedPos();
    }
    if (!qFuzzyCompare(yStep, 0))
    {
        m_yAxis->waitArrivedPos();
    }
    if (!qFuzzyCompare(zStep, 0))
    {
        m_zAxis->waitArrivedPos();
    }
}

void XYZModule::absMove(double x, double y, double z)
{
    m_xAxis->absMove(x, false);
    m_yAxis->absMove(y, false);
    m_zAxis->absMove(z);
    m_xAxis->waitArrivedPos();
    m_yAxis->waitArrivedPos();
}

void XYZModule::moveXYToPos(QString posName, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName));

    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    setNextPosXyVelAcc(pos);
    m_xAxis->absMove(pos->xPos(), false);
    m_yAxis->absMove(pos->yPos(), false);
    if (waitDone)
    {
        m_xAxis->waitArrivedPos(pos->xPos(), pos->xPrecision());
        m_yAxis->waitArrivedPos(pos->yPos(), pos->yPrecision());
    }
}

void XYZModule::moveZToPos(QString posName, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName));

    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    setNextPosZVelAcc(pos);
    m_zAxis->absMove(pos->zPos(), false);
    if (waitDone)
    {
        m_zAxis->waitArrivedPos(pos->zPos(), pos->zPrecision());
    }
}

void XYZModule::waitXYArrivedPos(QString posName)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos(), pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos(), pos->yPrecision());
}

void XYZModule::waitZArrivedPos(QString posName)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    m_zAxis->waitArrivedPos(pos->zPos(), pos->zPrecision());
}

void XYZModule::moveXyToPosWithOffset(QString posName, double xOffset, double yOffset, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName, xOffset, yOffset));

    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    setNextPosXyVelAcc(pos);
    m_xAxis->absMove(pos->xPos() + xOffset, false);
    m_yAxis->absMove(pos->yPos() + yOffset, false);
    if (waitDone)
    {
        m_xAxis->waitArrivedPos(pos->xPos() + xOffset, pos->xPrecision());
        m_yAxis->waitArrivedPos(pos->yPos() + yOffset, pos->yPrecision());
    }
}

void XYZModule::waitXyArrivedPosWithOffset(QString posName, double xOffset, double yOffset)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos() + xOffset, pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos() + yOffset, pos->yPrecision());
}

void XYZModule::setNextPosXYVelAcc(QString posName)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    setNextPosXyVelAcc(pos);
}

void XYZModule::moveXY(double xTarget, double yTarget, double precision)
{
    m_xAxis->absMove(xTarget, false);
    m_yAxis->absMove(yTarget, false);
    m_xAxis->waitArrivedPos(xTarget, precision);
    m_yAxis->waitArrivedPos(yTarget, precision);
}

void XYZModule::moveXyWithoutWait(const QPointF &xyTarget)
{
    m_xAxis->absMove(xyTarget.x(), false);
    m_yAxis->absMove(xyTarget.y(), false);
}

void XYZModule::waitXyApproachPos(const QPointF &xyTarget, double approachWindow)
{
    m_xAxis->waitApproachPos(xyTarget.x(), approachWindow);
    m_yAxis->waitApproachPos(xyTarget.y(), approachWindow);
}

void XYZModule::waitXyArrivedPos(const QPointF &xyTarget, double precision)
{
    m_xAxis->waitArrivedPos(xyTarget.x(), precision);
    m_yAxis->waitArrivedPos(xyTarget.y(), precision);
}

void XYZModule::waitXyzProfilerStopped()
{
    m_xAxis->waitProfilerStopped();
    m_yAxis->waitProfilerStopped();
    m_zAxis->waitProfilerStopped();
}

void XYZModule::moveToPos(QString posName, MoveSequence::Sequence safetyMoveSequence, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName, safetyMoveSequence));

    moveToWithOffset(posName, safetyMoveSequence, 0, 0, 0, waitDone, false);
}

void XYZModule::recordCurrentPos()
{
    recordPos->setXPos(m_xAxis->getCurrentOutputPos());
    recordPos->setYPos(m_yAxis->getCurrentOutputPos());
    recordPos->setZPos(m_zAxis->getCurrentOutputPos());
}

void XYZModule::moveToRecordPos(MoveSequence::Sequence safetyMoveSequence)
{
    qCDebug(motionCate()) << tr("Move to record pos, x,y,z:") << recordPos->xPos() << recordPos->yPos() << recordPos->zPos();
    moveToWithOffset(RecordPosName, safetyMoveSequence, 0, 0, 0, true, false);
}

QVariantMap XYZModule::getModuleCurrentPos()
{
    QVariantMap axesCurrentPos;
    axesCurrentPos["xPos"] = m_xAxis->getCurrentOutputPos();
    axesCurrentPos["yPos"] = m_yAxis->getCurrentOutputPos();
    axesCurrentPos["zPos"] = m_zAxis->getCurrentOutputPos();
    return axesCurrentPos;
}

void XYZModule::initImpl()
{
    m_xAxis = MotionElementContainer::getIns()->getItem<SCAxis>(xyzModuleConfig->xAxisName());
    m_yAxis = MotionElementContainer::getIns()->getItem<SCAxis>(xyzModuleConfig->yAxisName());
    m_zAxis = MotionElementContainer::getIns()->getItem<SCAxis>(xyzModuleConfig->zAxisName());
}

void XYZModule::moveTo(QString posName, bool waitDone)
{
    auto pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    moveToPos(posName, pos->safetyMoveSequence(), waitDone);
}

void XYZModule::waitArrivedPos(QString posName)
{
    XYZModulePos *pos = xyzModuleConfig->getPos<XYZModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos(), pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos(), pos->yPrecision());
    m_zAxis->waitArrivedPos(pos->zPos(), pos->zPrecision());
}
