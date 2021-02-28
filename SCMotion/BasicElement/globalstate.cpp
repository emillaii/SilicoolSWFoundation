#include "globalstate.h"

GlobalState::GlobalState()
{
    isPaused = false;
    isAborted = false;
}

GlobalState &GlobalState::getIns()
{
    static GlobalState instance;
    return instance;
}

bool GlobalState::getIsPaused()
{
    return isPaused;
}

void GlobalState::setIsPaused(bool value)
{
    isPaused = value;
}

void GlobalState::setIsAborted(bool value)
{
    isAborted = value;
}

void GlobalState::waitNotPaused(QString name)
{
    if (isPaused)
    {
        QElapsedTimer timer;
        timer.start();
        while (isPaused)
        {
            if (isAborted)
            {
                throw SilicolAbort("Abort while paused!");
            }
            if (timer.elapsed() > 30000)
            {
                qCDebug(motionCate()) << QString("Machine is paused. %1 is suspended...").arg(name);
                timer.restart();
            }
            QThread::msleep(5);
        }
    }
}
