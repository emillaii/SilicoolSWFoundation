#include "MotionCore.h"

#define CTRL_PI 3.14159265358979


MotionCore::MotionCore(QObject *parent) : Worker("MotionCore", parent)
{
}

double MotionCore::calculateProfileTime(double dist, double maxVel, double maxAcc, double maxDec)
{
    double T1, T2, T3, Dmv, Dcv;
    T2  = 0;
    T1 = (maxVel * CTRL_PI) / (2 * maxAcc);
    T1 = floor(T1 + 0.5);
    T3 = (maxVel * CTRL_PI) / (2 * maxDec);
    T3 = floor(T3 + 0.5);
    Dmv = maxVel * T1 / 2 + maxVel * T3 / 2;
    Dcv = dist - Dmv;

    // in case acc or vel too large, then fix acc adj autoadj vel and recal T1 (scale time profile by ratio)
    if (Dcv < 0)
    {
        double T3factor = T3 / (T1 + T3);
        double ScaledD1 = dist * (1 - T3factor);
        T1 = floor(sqrt(ScaledD1 * CTRL_PI / maxAcc)) + 1;
        double ScaledD3 = dist * T3factor;
        T3 = floor((sqrt(4 * (maxDec / CTRL_PI) * ScaledD3)) / (2 * (maxDec / CTRL_PI))) + 1;
    }
    else if (Dcv > 0)
    {
        T2 = Dcv / maxVel;
        T2 = floor(T2 + 0.5);
    }
    double motionTime = T1+T2+T3+1;

    QString informationMessage;
    informationMessage.append(tr("Motion time: ")).append(QString::number(motionTime)).append("ms");
    UIOperation::getIns()->okCancelConfirm(informationMessage);

    return motionTime;
}

