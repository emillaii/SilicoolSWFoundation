#include "xymodule.h"

XYModulePos::XYModulePos() : ModulePos()
{
    init();
}

XYModuleConfig::XYModuleConfig(QObject *parent) : AxisModuleConfig(&XYModulePos::staticMetaObject, parent)
{
    init();
}

XYModule::XYModule(QString name, XYModuleConfig *xyModuleConfig, QObject *parent) : AxisModule(xyModuleConfig, parent), xyModuleConfig(xyModuleConfig)
{
    setObjectName(name);
    MotionElementContainer::getIns()->registerItem<XYModule>(name, this);
}

void XYModule::moveToWithOffset(QString posName, QPointF offset, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName, offset.x(), offset.y()));

    XYModulePos *pos = xyModuleConfig->getPos<XYModulePos>(posName);
    setNextPosVelAcc(pos);
    m_xAxis->absMove(pos->xPos() + offset.x(), false);
    m_yAxis->absMove(pos->yPos() + offset.y(), false);
    if (waitDone)
    {
        m_xAxis->waitArrivedPos(pos->xPos() + offset.x(), pos->xPrecision());
        m_yAxis->waitArrivedPos(pos->yPos() + offset.y(), pos->yPrecision());
    }
}

void XYModule::waitArrivedPosWithOffset(QString posName, QPointF offset)
{
    XYModulePos *pos = xyModuleConfig->getPos<XYModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos() + offset.x(), pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos() + offset.y(), pos->yPrecision());
}

void XYModule::setNextPosVelAcc(QString posName)
{
    XYModulePos *pos = xyModuleConfig->getPos<XYModulePos>(posName);
    setNextPosVelAcc(pos);
}

void XYModule::moveTo(QPointF pos, bool waitDone, double precision)
{
    qCInfo(motionCate(), N_FUNC_ARG(pos.x(), pos.y()));

    m_xAxis->absMove(pos.x(), false);
    m_yAxis->absMove(pos.y(), false);
    if (waitDone)
    {
        m_xAxis->waitArrivedPos(pos.x(), precision);
        m_yAxis->waitArrivedPos(pos.y(), precision);
    }
}

void XYModule::waitArrivedPos(QPointF pos, double precision)
{
    m_xAxis->waitArrivedPos(pos.x(), precision);
    m_yAxis->waitArrivedPos(pos.y(), precision);
}

void XYModule::relMove(QPointF step, double precision)
{
    qCInfo(motionCate(), N_FUNC_ARG(step.x(), step.y()));

    m_xAxis->relMove(step.x(), false);
    m_yAxis->relMove(step.y(), false);
    m_xAxis->waitArrivedPos(precision, 3000);
    m_yAxis->waitArrivedPos(precision, 3000);
}

QVariantMap XYModule::getModuleCurrentPos()
{
    QVariantMap axesCurrentPos;
    axesCurrentPos["xPos"] = m_xAxis->getCurrentOutputPos();
    axesCurrentPos["yPos"] = m_yAxis->getCurrentOutputPos();
    return axesCurrentPos;
}

void XYModule::initImpl()
{
    m_xAxis = MotionElementContainer::getIns()->getItem<SCAxis>(xyModuleConfig->xAxisName());
    m_yAxis = MotionElementContainer::getIns()->getItem<SCAxis>(xyModuleConfig->yAxisName());
}

void XYModule::moveTo(QString posName, bool waitDone)
{
    qCInfo(motionCate(), N_FUNC_ARG(posName));

    XYModulePos *pos = xyModuleConfig->getPos<XYModulePos>(posName);
    setNextPosVelAcc(pos);
    m_xAxis->absMove(pos->xPos(), false);
    m_yAxis->absMove(pos->yPos(), false);
    if (waitDone)
    {
        m_xAxis->waitArrivedPos(pos->xPos(), pos->xPrecision());
        m_yAxis->waitArrivedPos(pos->yPos(), pos->yPrecision());
    }
}

void XYModule::waitArrivedPos(QString posName)
{
    XYModulePos *pos = xyModuleConfig->getPos<XYModulePos>(posName);
    m_xAxis->waitArrivedPos(pos->xPos(), pos->xPrecision());
    m_yAxis->waitArrivedPos(pos->yPos(), pos->yPrecision());
}
