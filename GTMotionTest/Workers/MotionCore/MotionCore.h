#ifndef TRAYLOADER_H
#define TRAYLOADER_H

#include "systemCore/worker.h"
#include "MotionCoreConfig.h"
#include <QObject>

class MotionCore : public Worker
{
    Q_OBJECT
public:
    explicit MotionCore(QObject *parent = nullptr);

public slots:
    double calculateProfileTime(double dist, double maxVel, double maxAcc, double maxDec);

    // Worker interface
protected:
    virtual void initEventHandle() override{}
    virtual void startEventHandle(const WorkerEvent &event) override{}
    virtual void resetEventHandle() override{}
    virtual void homeWorker(bool moveToReadyPosAfterHome) override{}

private:
//    MotionCoreConfig *motionCoreConfig;
};

#endif    // TRAYLOADER_H
