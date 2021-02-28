#include "singleaxis.h"

SingleAxisPos::SingleAxisPos() : ModulePos()
{
    init();
}

SingleAxisConfig::SingleAxisConfig(QObject *parent) : AxisModuleConfig(&SingleAxisPos::staticMetaObject, parent)
{
    init();
}

SingleAxis::SingleAxis(QString moduleName, SingleAxisConfig *sAxisConfig, QObject *parent) : AxisModule(sAxisConfig, parent), sAxisConfig(sAxisConfig)
{
    setObjectName(moduleName);
    MotionElementContainer::getIns()->registerItem<SingleAxis>(moduleName, this);
}

void SingleAxis::moveToWithOffset(QString posName, double offset, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName, offset));

    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    m_axis->setNextMoveVelAccRatio(pos->velRatio(), pos->accRatio());
    m_axis->absMove(pos->pos() + offset, waitDone, pos->precision());
}

void SingleAxis::waitArrivedPosWithOffset(QString posName, double offset)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    m_axis->waitArrivedPos(pos->pos() + offset, pos->precision());
}

void SingleAxis::tryToMove(QString posName, uint checkInterval, uint stepCount)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    m_axis->setNextMoveVelAccRatio(pos->velRatio(), pos->accRatio());
    m_axis->tryToMove(pos->pos(), checkInterval, stepCount, pos->precision());
}

bool SingleAxis::greaterThanPos(QString posName)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    return axis()->getFeedbackPos() > (pos->pos() - pos->precision());
}

bool SingleAxis::lessThanPos(QString posName)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    return axis()->getFeedbackPos() < (pos->pos() + pos->precision());
}

void SingleAxis::waitGreaterThanPos(QString posName)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    axis()->waitGreaterThanPos(pos->pos(), pos->precision());
}

void SingleAxis::waitLessThanPos(QString posName)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    axis()->waitLessThanPos(pos->pos(), pos->precision());
}

QVariantMap SingleAxis::getModuleCurrentPos()
{
    QVariantMap axesCurrentPos;
    axesCurrentPos["pos"] = m_axis->getCurrentOutputPos();
    return axesCurrentPos;
}

void SingleAxis::initImpl()
{
    m_axis = MotionElementContainer::getIns()->getItem<SCAxis>(sAxisConfig->axisName());
}

void SingleAxis::moveTo(QString posName, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName));

    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    m_axis->setNextMoveVelAccRatio(pos->velRatio(), pos->accRatio());
    m_axis->absMove(pos->pos(), waitDone, pos->precision());
}

void SingleAxis::waitArrivedPos(QString posName)
{
    SingleAxisPos *pos = sAxisConfig->getPos<SingleAxisPos>(posName);
    m_axis->waitArrivedPos(pos->pos(), pos->precision());
}
