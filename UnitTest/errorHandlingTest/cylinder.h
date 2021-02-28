#ifndef CYLINDER_H
#define CYLINDER_H

#include "actionerror.h"
#include "common.h"
#include "loghelper.h"
#include <QThread>

class Cylinder
{
public:
    Cylinder(QString name) : name(name) {}

    void set(bool state)
    {
        qDebug(FUNC_ARG(state));
        aeh.tryToHandleError([&] {
            if (myRand(3) == 0)
            {
                throw SilicolError("Cylinder", QString("%1 set to %2 state failed!").arg(name).arg(state));
            }
            QThread::msleep(myRand(200, 500));
        });
    }

private:
    QString name;
    ActionErrorHandler aeh;
};
#endif    // CYLINDER_H
