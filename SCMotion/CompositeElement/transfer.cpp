#include "transfer.h"

Transfer::Transfer(QString name, LoadConfig *loadConfig, UnloadConfig *unloadConfig, QObject *parent)
    : QObject(parent), name(name), loadConfig(loadConfig), unloadConfig(unloadConfig)
{
}

void Transfer::init(SCAxis *belt, SCDI *sensor)
{
    this->belt = belt;
    this->sensor = sensor;
    SC_ASSERT(loadConfig != nullptr || unloadConfig != nullptr);
}

void Transfer::loadTray()
{
    SC_ASSERT(loadConfig != nullptr);
    SC_ASSERT(belt != nullptr && sensor != nullptr);

    belt->velocityMove(loadConfig->beltRunDirection());
    ErrorHandler::tryToHandleGeneralError<void>(
        [this] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (sensor->checkState(true))
                {
                    if (loadConfig->delayAfterSNRGotSignal() > 0)
                    {
                        QThread::msleep(loadConfig->delayAfterSNRGotSignal());
                    }
                    belt->stop();
                    break;
                }
                if (timer.elapsed() > loadConfig->loadTimeout())
                {
                    belt->stop();
                    throw GeneralError(name, tr("Load tray timeout!"));
                }
                QThread::msleep(1);
            }
        },
        [this] { belt->velocityMove(loadConfig->beltRunDirection()); });
}

bool Transfer::moveToDetectHasTray(int detectTrayMaxTime)
{
    SC_ASSERT(belt != nullptr);

    SCAxis::Direction dir(SCAxis::Positive);
    if (loadConfig != nullptr)
    {
        dir = loadConfig->beltRunDirection();
    }
    else if (unloadConfig != nullptr)
    {
        dir = unloadConfig->beltRunDirection();
    }
    belt->velocityMove(dir);
    QElapsedTimer timer;
    timer.start();
    while (timer.elapsed() < detectTrayMaxTime)
    {
        if (sensor->get(-1))
        {
            break;
        }
        QThread::msleep(1);
    }
    belt->stop();
    return sensor->get(-1);
}

void Transfer::unloadTray()
{
    SC_ASSERT(unloadConfig != nullptr);
    SC_ASSERT(belt != nullptr && sensor != nullptr);

    belt->velocityMove(unloadConfig->beltRunDirection());
    ErrorHandler::tryToHandleGeneralError<void>(
        [this] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (sensor->checkState(false))
                {
                    if (unloadConfig->delayAfterSNRLostSignal() > 0)
                    {
                        QThread::msleep(unloadConfig->delayAfterSNRLostSignal());
                    }
                    belt->stop();
                    break;
                }
                if (timer.elapsed() > unloadConfig->unloadTimeout())
                {
                    belt->stop();
                    throw GeneralError(name, tr("Unload tray timeout!"));
                }
                QThread::msleep(1);
            }
        },
        [this] { belt->velocityMove(unloadConfig->beltRunDirection()); });
}

void Transfer::returnbackTray(SCDI *entranceSNR)
{
    SC_ASSERT(loadConfig != nullptr);
    SC_ASSERT(belt != nullptr);

    belt->velocityMove(SCAxis::oppositeDir(loadConfig->beltRunDirection()));
    ErrorHandler::tryToHandleGeneralError<void>(
        [this, entranceSNR] {
            QElapsedTimer timer;
            timer.start();
            while (true)
            {
                if (entranceSNR->get(-1))
                {
                    belt->stop();
                    break;
                }
                if (timer.elapsed() > loadConfig->loadTimeout())
                {
                    belt->stop();
                    throw GeneralError(name, tr("Return back tray timeout!"));
                }
                QThread::msleep(1);
            }
        },
        [this] { belt->velocityMove(SCAxis::oppositeDir(loadConfig->beltRunDirection())); });
}
