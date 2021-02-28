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
