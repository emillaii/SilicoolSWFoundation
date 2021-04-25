#include "dummydispatcher.h"

DummyDispatcher::DummyDispatcher(MasterMotionManager *masterMotionManager, QObject *parent)
    : Dispatcher(parent), masterMotionManager(masterMotionManager)
{
}

void DummyDispatcher::setContextProperty(QQmlApplicationEngine &engine) {}

void DummyDispatcher::handleInit()
{
    VisionManager::getIns().postInit(false);
    masterMotionManager->initMotionManagers();
}

void DummyDispatcher::handleStart()
{
    VisionManager::getIns().stopAllCameraShowRealtimeImage();
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

void DummyDispatcher::preHome() {}

void DummyDispatcher::postHome() {}
