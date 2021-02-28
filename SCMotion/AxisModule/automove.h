#ifndef AUTOMOVE_H
#define AUTOMOVE_H

#include "axismodule.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT AutoMove
{
public:
    AutoMove(AxisModule *targetModule, QString targetPos, bool waitDone = true);
    void disable();
    ~AutoMove();

private:
    AxisModule *targetModule;
    QString targetPos;
    bool waitDone;
    bool isDisable = false;
};

#endif    // AUTOMOVE_H
