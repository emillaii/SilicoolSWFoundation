#include "distatemonitor.h"

DiStateMonitor::DiStateMonitor(QObject *parent) : QThread(parent) {}

DiStateMonitor::~DiStateMonitor()
{
    stopMonitor();
}

void DiStateMonitor::registerDi(QString diName)
{
    SCDI *di = MotionElementContainer::getIns()->getItem<SCDI>(diName);
    QMutexLocker tmpLocker(&locker);
    if (!diStates.contains(di))
    {
        if (di->isInit())
        {
            diStates[di] = di->get();
        }
        else
        {
            diStates[di] = false;
        }
    }
    if (!isRunning())
    {
        start();
    }
}

void DiStateMonitor::unRegisterDi(QString diName)
{
    SCDI *di = MotionElementContainer::getIns()->getItem<SCDI>(diName);
    QMutexLocker tmpLocker(&locker);
    if (diStates.contains(di))
    {
        diStates.remove(di);
    }
}

void DiStateMonitor::startMonitor()
{
    isMonitor = true;
    start();
}

void DiStateMonitor::stopMonitor()
{
    isMonitor = false;
    wait();
}

void DiStateMonitor::run()
{
    while (isMonitor)
    {
        {
            QMutexLocker tmpLocker(&locker);
            if (diStates.count() == 0)
            {
                return;
            }
            foreach (auto di, diStates.keys())
            {
                bool newState = di->get();
                if (diStates[di] != newState)
                {
                    diStates[di] = newState;
                    emit diStateChanged(di->name(), newState);
                }
            }
        }
        QThread::msleep(5);
    }
}

AutoUnregisterDi::AutoUnregisterDi(const QString &diName) : diName(diName) {}

AutoUnregisterDi::~AutoUnregisterDi()
{
    DiStateMonitor::getIns().unRegisterDi(diName);
}
