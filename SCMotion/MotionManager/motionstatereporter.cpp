#include "motionstatereporter.h"

MotionStateReporter::MotionStateReporter(int reportInterval, QObject *parent) : QThread(parent), reportInterval(reportInterval) {}

void MotionStateReporter::setElementToBeWatched(
    QList<SCAxis *> axes, QList<SCDI *> scdis, QList<SCDO *> scdos, QList<SCCylinder *> cylinders, QList<SCVacuum *> vacuums)
{
    this->axes = axes;
    this->scdis = scdis;
    this->scdos = scdos;
    this->cylinders = cylinders;
    this->vacuums = vacuums;
}

void MotionStateReporter::startReport()
{
    if (!isReporting)
    {
        isReporting = true;
        start();
    }
}

void MotionStateReporter::stopReport()
{
    if (isReporting)
    {
        isReporting = false;
        wait();
    }
}

void MotionStateReporter::run()
{
    QElapsedTimer timer;
    while (isReporting)
    {
        QVariantMap motionState;

        QVariantMap axisStates;
        foreach (auto axis, axes)
        {
            QVariantMap axisState;
            axisState["isEnabled"] = axis->isEnable();
            axisState["hasHome"] = axis->hasHome();
            axisState["positiveLimitStatus"] = axis->getPositiveLimitStatus();
            axisState["negativeLimitStatus"] = axis->getNegativeLimitStatus();
            axisState["profilerPos"] = axis->getCurrentOutputPos();
            axisState["feedbackPos"] = axis->getFeedbackPos();
            axisState["hasAlarm"] = axis->hasAlarm();
            axisState["isRunning"] = axis->isRunning();
            axisState["isInPos"] = axis->isInPos();
            axisState["velocityRatio"] = axis->velocityRatio();
            axisState["hasMasterAxis"] = axis->hasMasterAxis();
            axisState["bindState"] = axis->hasBindedToMaterAxis();
            axisState["jogMovable"] = axis->config()->advancedAxisConfig()->jogMovable();
            axisStates[axis->name()] = axisState;
        }
        motionState["axisStates"] = axisStates;

        QVariantMap diStates;
        foreach (auto scdi, scdis)
        {
            diStates[scdi->name()] = scdi->get();
        }
        motionState["diStates"] = diStates;

        QVariantMap doStates;
        foreach (auto scdo, scdos)
        {
            doStates[scdo->name()] = scdo->get();
        }
        motionState["doStates"] = doStates;

        QVariantMap cylStates;
        foreach (auto cyl, cylinders)
        {
            cylStates[cyl->name()] = cyl->getDetailState();
        }
        motionState["cylStates"] = cylStates;

        QVariantMap vacuumStates;
        foreach (auto vacuum, vacuums)
        {
            vacuumStates[vacuum->name()] = vacuum->getDetailState();
        }
        motionState["vacuumStates"] = vacuumStates;

        emit reportMotionState(motionState);

        timer.restart();
        while (isReporting && timer.elapsed() < reportInterval)
        {
            QThread::msleep(20);
        }
    }
}
