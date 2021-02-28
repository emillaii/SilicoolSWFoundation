#ifndef VACUUM_H
#define VACUUM_H

#include "actionerror.h"
#include "common.h"
#include "loghelper.h"
#include <QThread>

class Vacuum
{
public:
    void set(bool state)
    {
        qDebug(FUNC_ARG(state));
    }

    bool checkState(bool targetState)
    {
        return myRand(3) == 0;
    }

private:
    QString name;
};

#endif    // VACUUM_H
