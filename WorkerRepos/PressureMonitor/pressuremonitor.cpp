#include "pressuremonitor.h"

PressureMonitor::PressureMonitor(QObject *parent) : Worker(WorkerName::PressureMonitor, parent) {}

void PressureMonitor::initEventHandle()
{
    lowPressureAlarm = MotionElementContainer::getIns()->getItem<SCDI>(DiName::LowPressureAlarm);
    if (lowPressureAlarm->get())
    {
        UIOperation::getIns()->showError(tr("Low air pressure!"));
    }
}

void PressureMonitor::startEventHandle(const WorkerEvent &event)
{
    auto runParam = event.eventArgs.toMap();
    if (runParam.contains("RunMode") && runParam["RunMode"] == "DryRun")
    {
        return;
    }
    pushEvent(WorkerEvent("checkAirPressure"));
}

void PressureMonitor::homeWorker(bool moveToReadyPosAfterHome)
{
    Q_UNUSED(moveToReadyPosAfterHome)
}

void PressureMonitor::checkAirPressure()
{
    for (int i = 0; i < 300; i++)
    {
        QThread::msleep(200);
        if (hasAbortOrAutoStopEvent())
        {
            return;
        }
    }

    ErrorHandler::tryToHandleError([this] {
        if (lowPressureAlarm->get())
        {
            throw GeneralError(WorkerName::PressureMonitor, tr("Low air pressure!"), true);
        }
    });
    pushEvent(WorkerEvent("checkAirPressure"));
}
