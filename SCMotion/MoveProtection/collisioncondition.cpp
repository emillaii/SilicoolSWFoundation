#include "collisioncondition.h"

CollisionCondition *CollisionCondition::createInstance(CollisionCondition::Type type, QObject *parent)
{
    switch (type)
    {
        case SingleAxisCollisionCondition:
            return new SingleAxisCollision(parent);
        case TwoAxisCollisionCondition:
            return new TwoAxisCollision(parent);
        case CylinderCollisionCondition:
            return new CylinderCollision(parent);
        case DICollisionCondition:
            return new DICollision(parent);
    }
    qCritical() << "Unknown CollisionCondition::Type:" << type;
    return nullptr;
}

bool SingleAxisCollision::meetCondition(QString name, double targetState)
{
    currentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(axisName());
    if (qFuzzyCompare(currentPos, UNKNOWN_AXIS_POS))
    {
        return true;
    }
    targetPos = name == axisName() ? targetState : MotionStateGetter::getIns().reqGetAxisTargetPos(axisName());
    return cmpOperation()->compare2(currentPos, targetPos);
}

QString SingleAxisCollision::getCollisionDescription()
{
    if (qFuzzyCompare(currentPos, UNKNOWN_AXIS_POS))
    {
        return QString("Get %1 axis pos failed! Axis may not be inited or homed!").arg(axisName());
    }
    else
    {
        return QString("%1 from %2 to %3, %4").arg(axisName()).arg(currentPos).arg(targetPos).arg(cmpOperation()->getCmpDescription());
    }
}

QStringList SingleAxisCollision::getAffectedMotionElementNames()
{
    QStringList names;
    names.append(axisName());
    return names;
}

double SingleAxisCollision::getCurrentState()
{
    return MotionStateGetter::getIns().reqGetAxisFeedbackPos(axisName());
}

TwoAxisCollision::TwoAxisCollision(QObject *parent) : CollisionCondition(parent)
{
    setOptionalProperty("axis1Name", GlobalMotionElements::getIns().axisNames());
    setOptionalProperty("axis2Name", GlobalMotionElements::getIns().axisNames());
    QStringList args;
    args << "axis1Name"
         << "posComposition"
         << "axis2Name";
    setEngineerAuthorities(args);
    RenameManager::getIns().subscribeAxisNameChanged(this, "axis1Name");
    RenameManager::getIns().subscribeAxisNameChanged(this, "axis2Name");
}

bool TwoAxisCollision::meetCondition(QString name, double targetState)
{
    axis1CurrentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(axis1Name());
    axis2CurrentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(axis2Name());
    if (qFuzzyCompare(axis1CurrentPos, UNKNOWN_AXIS_POS) || qFuzzyCompare(axis2CurrentPos, UNKNOWN_AXIS_POS))
    {
        return true;
    }
    axis1TargetPos = name == axis1Name() ? targetState : MotionStateGetter::getIns().reqGetAxisTargetPos(axis1Name());
    axis2TargetPos = name == axis2Name() ? targetState : MotionStateGetter::getIns().reqGetAxisTargetPos(axis2Name());
    switch (posComposition())
    {
        case Add:
        {
            return cmpOperation()->compare2(axis1CurrentPos + axis2CurrentPos, axis1CurrentPos + axis2TargetPos)
                   || cmpOperation()->compare2(axis1TargetPos + axis2CurrentPos, axis1TargetPos + axis2TargetPos)
                   || cmpOperation()->compare2(axis1CurrentPos + axis2CurrentPos, axis1TargetPos + axis2CurrentPos)
                   || cmpOperation()->compare2(axis1CurrentPos + axis2TargetPos, axis1TargetPos + axis2TargetPos);
        }
        case Minus:
        {
            return cmpOperation()->compare2(axis1CurrentPos - axis2CurrentPos, axis1CurrentPos - axis2TargetPos)
                   || cmpOperation()->compare2(axis1TargetPos - axis2CurrentPos, axis1TargetPos - axis2TargetPos)
                   || cmpOperation()->compare2(axis1CurrentPos - axis2CurrentPos, axis1TargetPos - axis2CurrentPos)
                   || cmpOperation()->compare2(axis1CurrentPos - axis2TargetPos, axis1TargetPos - axis2TargetPos);
        }
    }
    qCritical() << "Unknown TwoAxisCollision::PosComposition:" << posComposition();
    return true;
}

QString TwoAxisCollision::getCollisionDescription()
{
    if (qFuzzyCompare(axis1CurrentPos, UNKNOWN_AXIS_POS))
    {
        return QString("Get %1 axis pos failed! Axis may not be inited or homed!").arg(axis1Name());
    }
    if (qFuzzyCompare(axis2CurrentPos, UNKNOWN_AXIS_POS))
    {
        return QString("Get %1 axis pos failed! Axis may not be inited or homed!").arg(axis2Name());
    }
    return QString("%1 from %2 to %3, %4, %5 from %6 to %7, %8")
        .arg(axis1Name())
        .arg(axis1CurrentPos)
        .arg(axis1TargetPos)
        .arg(getPosComposition())
        .arg(axis2Name())
        .arg(axis2CurrentPos)
        .arg(axis2TargetPos)
        .arg(cmpOperation()->getCmpDescription());
}

QStringList TwoAxisCollision::getAffectedMotionElementNames()
{
    QStringList names;
    names.append(axis1Name());
    names.append(axis2Name());
    return names;
}

double TwoAxisCollision::getCurrentState()
{
    double t_axis1CurrentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(axis1Name());
    double t_axis2CurrentPos = MotionStateGetter::getIns().reqGetAxisFeedbackPos(axis2Name());
    switch (posComposition())
    {
        case Add:
        {
            return t_axis1CurrentPos + t_axis2CurrentPos;
        }
        case Minus:
        {
            return t_axis1CurrentPos - t_axis2CurrentPos;
        }
    }
    qCritical() << "Unknown TwoAxisCollision::PosComposition:" << posComposition();
    return -9999;
}

QString TwoAxisCollision::getPosComposition()
{
    switch (posComposition())
    {
        case Add:
        {
            return "+";
        }
        case Minus:
        {
            return "-";
        }
    }
    qCritical() << "Unknown TwoAxisCollision::PosComposition:" << posComposition();
    return "";
}

bool CylinderCollision::meetCondition(QString name, double targetState)
{
    cylState = name == cylName() ? targetState : MotionStateGetter::getIns().reqGetCylinderCurrentState(cylName());
    return cmpOperation()->compare(cylState);
}

QString CylinderCollision::getCollisionDescription()
{
    return QString("%1 was in state %2 or will shift to %2 state, %3").arg(cylName()).arg(cylState).arg(cmpOperation()->getCmpDescription());
}

QStringList CylinderCollision::getAffectedMotionElementNames()
{
    QStringList names;
    names.append(cylName());
    return names;
}

double CylinderCollision::getCurrentState()
{
    return MotionStateGetter::getIns().reqGetCylinderCurrentState(cylName());
}

bool DICollision::meetCondition(QString name, double targetState)
{
    Q_UNUSED(name)
    Q_UNUSED(targetState)
    diState = MotionStateGetter::getIns().reqGetDiCurrentState(diName());
    return cmpOperation()->compare(diState);
}

QString DICollision::getCollisionDescription()
{
    return QString("%1 was in state %2, %3").arg(diName()).arg(diState).arg(cmpOperation()->getCmpDescription());
}

QStringList DICollision::getAffectedMotionElementNames()
{
    QStringList names;
    names.append(diName());
    return names;
}

double DICollision::getCurrentState()
{
    return MotionStateGetter::getIns().reqGetDiCurrentState(diName());
}
