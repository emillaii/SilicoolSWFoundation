#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#include "motionutility.h"
#include "scmotion_global.h"
#include <QElapsedTimer>
#include <QThread>

class SCMOTIONSHARED_EXPORT GlobalState
{
private:
    GlobalState();

public:
    static GlobalState &getIns();

    bool getIsPaused();
    void setIsPaused(bool value);
    void setIsAborted(bool value);

    void waitNotPaused(QString name);

private:
    bool isPaused;
    bool isAborted;
};

#endif    // GLOBALSTATE_H
