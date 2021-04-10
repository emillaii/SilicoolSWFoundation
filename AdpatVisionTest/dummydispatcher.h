#ifndef DUMMYDISPATCHER_H
#define DUMMYDISPATCHER_H

#include "../WorkerRepos/PressureMonitor/pressuremonitor.h"
#include "VisionManager/visionmanager.h"
#include "Workers/PickArm/pickarm.h"
#include "Workers/TrayLoader/trayloader.h"
#include "mastermotionmanager.h"
#include "systemCore/dispatcher.h"
#include "uphhelper.h"
#include <QObject>

class DummyDispatcher : public Dispatcher
{
    Q_OBJECT

public:
    explicit DummyDispatcher(MasterMotionManager *masterMotionManager, QObject *parent = nullptr);

    void setContextProperty(QQmlApplicationEngine &engine);

    // Dispatcher interface
protected:
    void handleInit() override;
    void handleStart() override;
    void handlePause() override;
    void handleContinue() override;
    void handleAbort() override;
    void handleReset() override;
    void preHome() override;
    virtual void postHome() override;

private:
    MasterMotionManager *masterMotionManager;
    UPHHelper *uphHelper;

    TrayConfig *trayConfig;
    ConfigFile *trayConfigFile;
    DutTrayMap *trayMap;

    TrayLoaderConfig *tlConfig;
    TrayLoader *tl;
    PickArm *pa;
    PressureMonitor *pressureMonitor;
};

#endif    // DUMMYDISPATCHER_H
