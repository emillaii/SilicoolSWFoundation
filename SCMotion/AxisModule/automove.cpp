#include "automove.h"

AutoMove::AutoMove(AxisModule *targetModule, QString targetPos, bool waitDone) : targetModule(targetModule), targetPos(targetPos), waitDone(waitDone)
{
}

void AutoMove::disable()
{
    isDisable = true;
}

AutoMove::~AutoMove()
{
    if (!isDisable)
    {
        targetModule->moveTo(targetPos, waitDone);
    }
}

AutoStop::AutoStop(SCAxis *targetAxis, bool waitStopped) : targetAxis(targetAxis), waitStopped(waitStopped) {}

void AutoStop::disable()
{
    isDisable = true;
}

AutoStop::~AutoStop()
{
    if (!isDisable)
    {
        targetAxis->stop(waitStopped);
    }
}
