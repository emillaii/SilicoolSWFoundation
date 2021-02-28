#ifndef MOTIONSTATEGETTER_H
#define MOTIONSTATEGETTER_H

#include <QObject>

class MotionStateGetter : public QObject
{
    Q_OBJECT

private:
    explicit MotionStateGetter(QObject *parent = nullptr) : QObject(parent) {}

public:
    static MotionStateGetter &getIns()
    {
        static MotionStateGetter instance;
        return instance;
    }

signals:
    double reqGetAxisFeedbackPos(QString axisName, int cacheInvalidationTime = 2);
    double reqGetAxisTargetPos(QString axisName, int cacheInvalidationTime = 2);
    int reqGetCylinderCurrentState(QString cylName, int cacheInvalidationTime = 2);
    bool reqGetDiCurrentState(QString diName, int cacheInvalidationTime = 2);
};

#endif    // MOTIONSTATEGETTER_H
