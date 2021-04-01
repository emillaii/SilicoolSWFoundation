#ifndef DUMMYDISPATCHER_H
#define DUMMYDISPATCHER_H

#include "../WorkerRepos/PressureMonitor/pressuremonitor.h"
#include "VisionManager/visionmanager.h"
#include "Workers/Discharge/discharge.h"
#include "Workers/Discharge/dischargeconfig.h"
#include "Workers/Feeding/feeding.h"
#include "Workers/Feeding/feedingconfig.h"
#include "Workers/PickArm/pickarm.h"
#include "Workers/PickArm/pickarmconfig.h"
#include "mastermotionmanager.h"
#include "systemCore/dispatcher.h"
#include "uphhelper.h"

class DetaperDispatcher : public Dispatcher
{
    Q_OBJECT

public:
    explicit DetaperDispatcher(MasterMotionManager *masterMotionManager, QObject *parent = nullptr);

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

//    TrayLoaderConfig *pTrayLoaderConfig;
//    TrayConfig *trayConfig;
//    ConfigFile *trayConfigFile;
//    PrdTrayMap *trayMap;

    FeedingConfig *pFeedingConfig;
    TrayConfig *pFeedingTrayConfig;
    ConfigFile *pFeedingConfigFile;
    FeedingMap *pFeedingMap;

    PickArmConfig *pPickArmConfig;
    TrayConfig *pPickArmTrayConfig;
    ConfigFile *pPickArmMapConfigFile;
    PickArmMap *pPickArmMap;

    DischargeConfig *pDischargeConfig;
    TrayConfig *pDischargeTrayConfig;
    ConfigFile *pDischargeConfigFile;
    DischargeMap *pDischargeMap;

    Discharge *pDisCharge;
    Feeding *pFeeding;
    PickArm *pPickArm;
    //TrayLoader *pTrayLoader;

    PressureMonitor *pressureMonitor;
};

#endif    // DUMMYDISPATCHER_H
