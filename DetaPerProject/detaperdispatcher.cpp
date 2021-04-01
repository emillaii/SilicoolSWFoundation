#include "detaperdispatcher.h"

DetaperDispatcher::DetaperDispatcher(MasterMotionManager *masterMotionManager, QObject *parent)
    : Dispatcher(parent), masterMotionManager(masterMotionManager)
{
    //init and register
    uphHelper = new UPHHelper();

    //Map2
    pFeedingTrayConfig = new TrayConfig();
    pFeedingConfigFile
        = new ConfigFile("FeedingTrayConfig", pFeedingTrayConfig, QString("%1/FeedingTrayConfig.json").arg(Worker::getWorkerConfigDir()));
    pFeedingConfigFile->populate(true);
    pFeedingMap = new FeedingMap(pFeedingTrayConfig,
                                 QString("%1/FeedingMapState.json").arg(Worker::getWorkerConfigDir()));

    pFeedingConfig = new FeedingConfig();
    pFeeding = new Feeding (pFeedingTrayConfig,pFeedingMap,pFeedingConfig);

    //Map4
    pDischargeTrayConfig = new TrayConfig();
    pDischargeConfigFile = new ConfigFile("DischargeTrayConfig", pDischargeTrayConfig,
                                          QString("%1/DischargeTrayConfig.json").arg(Worker::getWorkerConfigDir()));
    pDischargeConfigFile->populate(true);
    pDischargeMap = new DischargeMap(pDischargeTrayConfig,
                                   QString("%1/DischargeMapState.json").arg(Worker::getWorkerConfigDir()));
    pDischargeConfig = new DischargeConfig();
    pDisCharge = new Discharge(pDischargeTrayConfig,pDischargeMap,pDischargeConfig);

    //Map3
    pPickArmTrayConfig = new TrayConfig();
    pPickArmMapConfigFile = new ConfigFile("PickArmTrayConfig", pPickArmTrayConfig,
                                        QString("%1/PickArmTrayConfig.json").arg(Worker::getWorkerConfigDir()));
    pPickArmMapConfigFile->populate(true);
    pPickArmMap = new PickArmMap(pPickArmTrayConfig,
                                 QString("%1/PickArmMapState.json").arg(Worker::getWorkerConfigDir()));
    pPickArmConfig = new PickArmConfig();
    pPickArm = new PickArm (pFeeding, pDisCharge, pPickArmMap, pPickArmTrayConfig, pPickArmConfig, uphHelper);

    //pressureMonitor = new PressureMonitor();

    registerWorker(pDisCharge);
    registerWorker(pFeeding);
    registerWorker(pPickArm);
    //registerWorker(pressureMonitor);
}

void DetaperDispatcher::setContextProperty(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("mFeedingTrayConfig", pFeedingTrayConfig);
    engine.rootContext()->setContextProperty("mFeedingMap", pFeedingMap);
    engine.rootContext()->setContextProperty("mFeedingConfig", pFeedingConfig);

    engine.rootContext()->setContextProperty("mPickArmTrayConfig", pPickArmTrayConfig);
    engine.rootContext()->setContextProperty("mPickArmMap", pPickArmMap);
    engine.rootContext()->setContextProperty("mPickArmConfig", pPickArmConfig);
    
    engine.rootContext()->setContextProperty("mDischargeTrayConfig", pDischargeTrayConfig);
    engine.rootContext()->setContextProperty("mDischargeMap", pDischargeMap);
    engine.rootContext()->setContextProperty("mDischargeConfig", pDischargeConfig);

    engine.rootContext()->setContextProperty("mFeeding", pFeeding);
    engine.rootContext()->setContextProperty("mPickArm", pPickArm);
    engine.rootContext()->setContextProperty("mDischarge", pDisCharge);
    engine.rootContext()->setContextProperty("uphHelper", uphHelper);
}

void DetaperDispatcher::handleInit()
{
    VisionManager::getIns().postInit(true);
    masterMotionManager->initMotionManagers();
}

void DetaperDispatcher::handleStart()
{
    VisionManager::getIns().stopAllCameraShowRealtimeImage();
    uphHelper->start();
}

void DetaperDispatcher::handlePause()
{
    QThread::msleep(50);    // masterMotionManager lived in main thread. Sleep to make main thread handle state
    // transition first.
    masterMotionManager->pause();
}

void DetaperDispatcher::handleContinue()
{
    masterMotionManager->unPause();
}

void DetaperDispatcher::handleAbort()
{
    masterMotionManager->handleAbort();
}

void DetaperDispatcher::handleReset()
{
    masterMotionManager->handleReset();
}

void DetaperDispatcher::preHome()
{
    if(!MotionElementContainer::getIns()->getItem<SCDI>(DiName::PressureSNR)->get())
    {
        throw SilicolAbort("机台气压不足!");
    }
    if(MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArm_X)->hasAlarm())
    {
        throw SilicolAbort("机台未上电!");
    }
    if(MotionElementContainer::getIns()->getItem<SCDI>(DiName::CobCarrierSNR)->get())
    {
        throw SilicolAbort("CarrierBoard is between detaper and magazine!");
    }
    else
    {
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmZ_L)->home(false);
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmZ_R)->home(false);
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmZ_L)->waitHomeDone();
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmZ_R)->waitHomeDone();

        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmθ_L)->home(false);
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmθ_R)->home(false);
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmC_Z)->home(true);
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmθ_L)->waitHomeDone();
        MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PickArmθ_R)->waitHomeDone();
    }

}

void DetaperDispatcher::postHome() {}
