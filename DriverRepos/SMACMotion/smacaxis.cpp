#include "smacaxis.h"


SmacAxis::SmacAxis(SMACDriver *driver, SMACAxisConfig *smacAxisConfig, QObject *parent):smacDriver(driver),axisConfig(smacAxisConfig)
{

}

void SmacAxis::init()
{
    smacDriver->sendComand(axisConfig->initCom());
}

void SmacAxis::enableMotion()
{
    smacDriver->sendComand("0MN");
}

void SmacAxis::disableMotion()
{
    smacDriver->sendComand("0MF");
}

void SmacAxis::home()
{
    smacDriver->sendComand("0PM,MN,SV2000,SA200,GH");
}

void SmacAxis::moveToZeroPos()
{
    smacDriver->sendComand(axisConfig->extentedCom());
}

void SmacAxis::softClip()
{
    smacDriver->sendComand(axisConfig->clipCom());
}

double SmacAxis::getPos(int axis)
{
    smacDriver->clearBuffer();
    smacDriver->sendComand(QString("%1TP").arg(axis));
    QString data = smacDriver->readData();
    return data.toDouble();
}
