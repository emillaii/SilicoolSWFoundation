#include "xyzrdebugger.h"

XYZRDebugger::XYZRDebugger(XYZRDebuggerConfig *config, QObject *parent) : QObject(parent), m_config(config)
{
    updatePosTimer.setInterval(200);
    updatePosTimer.setSingleShot(false);
    updatePosTimer.callOnTimeout(this, &XYZRDebugger::updateAxesPos);
}

XYZRDebugger::~XYZRDebugger()
{
    updatePosTimer.stop();
}

void XYZRDebugger::stepMove(QString axisType, double step)
{
    QString axisName = getAxisName(axisType);
    if (axisName.isEmpty())
    {
        return;
    }
    if (haveToReverseDir(axisType))
    {
        step = -step;
    }
    QVariantList args;
    args << step;
    MotionManager::getIns().executeInstruction(MotionElement::Axis, axisName, "relForceMoveWithLimit", args);
}

void XYZRDebugger::jogMove(QString axisType, int dir, double velocity)
{
    if (dir != 0 && dir != 1)
    {
        qCCritical(motionCate()) << "Error direction:" << dir;
        return;
    }
    QString axisName = getAxisName(axisType);
    if (axisName.isEmpty())
    {
        return;
    }
    if (haveToReverseDir(axisType))
    {
        dir = 1 - dir;
    }
    QVariantList args;
    args << dir << velocity;
    MotionManager::getIns().executeInstruction(MotionElement::Axis, axisName, "jogMoveWithVel", args);
}

void XYZRDebugger::stopJogMove(QString axisType)
{
    QString axisName = getAxisName(axisType);
    if (axisName.isEmpty())
    {
        return;
    }
    MotionManager::getIns().executeInstruction(MotionElement::Axis, axisName, "stopAndResetState");
}

void XYZRDebugger::startUpdateAxesPos()
{
    updatePosTimer.start();
}

void XYZRDebugger::stopUpdateAxesPos()
{
    updatePosTimer.stop();
}

QPointF XYZRDebugger::getCurrentXYPos()
{
    return QPointF(getAxisCurrentPos("x", false), getAxisCurrentPos("y", false));
}

void XYZRDebugger::updateAxesPos()
{
    if (!MotionManager::getIns().isInit())
    {
        return;
    }
    double xPos = getAxisCurrentPos("x", true);
    double yPos = getAxisCurrentPos("y", true);
    double zPos = getAxisCurrentPos("z", true);
    double rPos = getAxisCurrentPos("r", true);
    emit axesPosPublished(xPos, yPos, zPos, rPos);
}

QString XYZRDebugger::getAxisName(QString axisType)
{
    if (axisType == "x")
    {
        return m_config->xAxisName();
    }
    else if (axisType == "y")
    {
        return m_config->yAxisName();
    }
    else if (axisType == "z")
    {
        return m_config->zAxisName();
    }
    else if (axisType == "r")
    {
        return m_config->rAxisName();
    }
    else
    {
        return "";
    }
}

bool XYZRDebugger::haveToReverseDir(QString axisType)
{
    if (axisType == "x" && m_config->reverseXDir())
    {
        return true;
    }
    if (axisType == "y" && m_config->reverseYDir())
    {
        return true;
    }
    return false;
}

double XYZRDebugger::getAxisCurrentPos(QString axisType, bool isFeedback)
{
    QString axisName = getAxisName(axisType);
    if (axisName.isEmpty())
    {
        return -9999;
    }
    if (isFeedback)
    {
        return MotionManager::getIns().getAxisFeedbackPos(axisName);
    }
    else
    {
        return MotionManager::getIns().getAxisCurrentPos(axisName);
    }
}
