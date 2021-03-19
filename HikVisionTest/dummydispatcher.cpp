#include "dummydispatcher.h"

DummyDispatcher::DummyDispatcher(MasterMotionManager *masterMotionManager, QObject *parent)
    : Dispatcher(parent), masterMotionManager(masterMotionManager)
{
    uphHelper = new UPHHelper();

    trayConfig = new TrayConfig();
    trayConfigFile
        = new ConfigFile("TrayConfig", trayConfig, QString("%1/trayConfig.json").arg(Worker::getWorkerConfigDir()));
    trayConfigFile->populate();
    trayMap = new DutTrayMap(trayConfig, QString("%1/trayMap.json").arg(Worker::getWorkerConfigDir()));
    tlConfig = new TrayLoaderConfig();
    tl = new TrayLoader(trayConfig, trayMap, tlConfig);
    pa = new PickArm(trayMap, tl, uphHelper);
    pressureMonitor = new PressureMonitor();

    registerWorker(tl);
    registerWorker(pa);
    registerWorker(pressureMonitor);
}

void DummyDispatcher::setContextProperty(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("dutTrayConfig", trayConfig);
    engine.rootContext()->setContextProperty("dutTrayMap", trayMap);
    engine.rootContext()->setContextProperty("tlConfig", tlConfig);
    engine.rootContext()->setContextProperty("tl", tl);
    engine.rootContext()->setContextProperty("pa", pa);
    engine.rootContext()->setContextProperty("uphHelper", uphHelper);
}

void DummyDispatcher::handleInit()
{
    VisionManager::getIns().postInit(false);
    masterMotionManager->initMotionManagers();
}

void DummyDispatcher::handleStart()
{
    VisionManager::getIns().stopAllCameraShowRealtimeImage();
    uphHelper->start();
}

void DummyDispatcher::handlePause()
{
    QThread::msleep(50);    // masterMotionManager lived in main thread. Sleep to make main thread handle state
    // transition first.
    masterMotionManager->pause();
}

void DummyDispatcher::handleContinue()
{
    masterMotionManager->unPause();
}

void DummyDispatcher::handleAbort()
{
    masterMotionManager->handleAbort();
}

void DummyDispatcher::handleReset()
{
    masterMotionManager->handleReset();
}

void DummyDispatcher::preHome()
{
    MotionElementContainer::getIns()->getItem<SCAxis>(AxisName::PAZ)->home();
}

void DummyDispatcher::postHome() {}
