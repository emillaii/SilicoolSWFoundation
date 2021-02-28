#include "twoaxisplanner.h"

TwoAxisPlanner::TwoAxisPlanner(QObject *parent) : QObject(parent) {}

void TwoAxisPlanner::init(SCAxis *axis1, SCAxis *axis2)
{
    this->axis1 = axis1;
    this->axis2 = axis2;
}

void TwoAxisPlanner::moveTo(double axis1TargetPos,
                            double axis2TargetPos,
                            double axis2TransisionPos,
                            double axis1TriggerPos,
                            TwoAxisPlanner::CmpOperation axis1TriggerCondition,
                            bool needMoveAxis1,
                            bool waitDone)
{
    Q_ASSERT(axis1 != nullptr && axis2 != nullptr);

    qCDebug(motionCate(),
            FUNC_ARG(axis1TargetPos, axis2TargetPos, axis2TransisionPos, axis1TriggerPos, axis1TriggerCondition, needMoveAxis1, waitDone));

    if (needMoveAxis1)
    {
        axis1->absMove(axis1TargetPos, false);
    }
    axis2->absMove(axis2TransisionPos, false);
    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        double axis1CurrentPos = axis1->getFeedbackPos(-1);
        if (qAbs(axis1CurrentPos - axis1TargetPos) < 0.1)
        {
            axis2->absForceMove(axis2TargetPos);
            break;
        }
        if (meetCondition(axis1CurrentPos, axis1TriggerPos, axis1TriggerCondition))
        {
            axis2->absForceMove(axis2TargetPos);
            break;
        }
        if (timer.elapsed() > 5000 && !axis1->isRunning())
        {
            throw SilicolAbort(tr("Axis %1 is not running!").arg(axis1->name()), EX_LOCATION);
        }
        QThread::msleep(1);
    }
    if (waitDone)
    {
        axis1->waitProfilerStopped();
        axis2->waitProfilerStopped();
    }
}
