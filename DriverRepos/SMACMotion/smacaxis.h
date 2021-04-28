#ifndef SMACAXIS_H
#define SMACAXIS_H

#include "BasicElement/scaxis.h"
#include "smacaxisconfig.h"
#include "smacdriver.h"
#include "errorHandling/silicolerrorhandler.h"
#include <QObject>

// TAMAGAWA motor to be tested, may not use SMAC for AOA gripper
class SmacAxis : public QObject
{
    Q_OBJECT
public:
    explicit SmacAxis(SMACDriver *driver, SMACAxisConfig *smacAxisConfig, QObject *parent = nullptr);

    void init();
    void enableMotion();
    void disableMotion();
    void home();
    void moveToZeroPos();
    void softClip();
    double getPos(int axis);
private:
    SMACDriver *smacDriver;
    SMACAxisConfig *axisConfig;
};

#endif    // SMACAXIS_H
