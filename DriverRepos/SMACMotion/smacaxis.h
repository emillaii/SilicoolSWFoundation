#ifndef SMACAXIS_H
#define SMACAXIS_H

#include "BasicElement/scaxis.h"
#include "smacaxisconfig.h"
#include "smacdriver.h"

// TAMAGAWA motor to be tested, may not use SMAC for AOA gripper
class SmacAxis : public QObject
{
    Q_OBJECT
    explicit SmacAxis(SMACDriver *driver, QObject *parent = nullptr);

    void init();
    void enableMotion();
    void disableMotion();
    void home();
    void setHome();
    void softClip();
    double getPos(int axis);
};

#endif    // SMACAXIS_H
