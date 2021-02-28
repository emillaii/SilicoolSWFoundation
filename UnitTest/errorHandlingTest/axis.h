#ifndef AXIS_H
#define AXIS_H

#include "actionerror.h"
#include "common.h"
#include "loghelper.h"
#include <QThread>

class Axis
{
public:
    Axis(QString name) : name(name) {}

    void moveTo(double targetPos)
    {
        qDebug() << "Axis" << name << "moveTo" << targetPos;
        aeh.tryToHandleError([&] {
            if (myRand(5) == 0)
            {
                throw ActionError("Axis", QString("%1 move to %2 failed!").arg(name).arg(targetPos), "You can retry");
            }
            QThread::msleep(myRand(200, 500));
            currentPos = targetPos;
        });
    }

    void stepMove(double step)
    {
        moveTo(currentPos + step);
    }

    double getCurrentPos()
    {
        return currentPos;
    }

private:
    double currentPos = 0;
    QString name;
    ActionErrorHandler aeh;
};

#endif    // AXIS_H
