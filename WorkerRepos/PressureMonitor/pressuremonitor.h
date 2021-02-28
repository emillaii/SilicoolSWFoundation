#ifndef PRESSUREMONITOR_H
#define PRESSUREMONITOR_H

#include "BasicElement/scdi.h"
#include "namedefinition.h"
#include "systemCore/worker.h"

namespace WorkerName
{
    const QString PressureMonitor = "PressureMonitor";
}

namespace DiName
{
    const QString LowPressureAlarm = "LowPressureAlarm";
}

class PressureMonitor : public Worker
{
    Q_OBJECT
public:
    explicit PressureMonitor(QObject *parent = nullptr);

    // Worker interface
protected:
    virtual void initEventHandle() override;
    virtual void startEventHandle(const WorkerEvent &event) override;
    virtual void resetEventHandle() override {}
    virtual void homeWorker(bool moveToReadyPosAfterHome) override;

private slots:
    void checkAirPressure();

private:
    SCDI *lowPressureAlarm;
};

#endif    // PRESSUREMONITOR_H
