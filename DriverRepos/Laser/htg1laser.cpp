#include "htg1laser.h"

HTG1Laser::HTG1Laser(LaserDriver *laserDriver, LaserStationConfig *laserStationConfig, QObject *parent)
    : QThread(parent), laserDriver(laserDriver), laserStationConfig(laserStationConfig)
{
}

void HTG1Laser::init()
{
    laserDriver->init(laserStationConfig->samplePeriod(), laserStationConfig->stationNum());
}

double HTG1Laser::readValidHeight()
{
    return ErrorHandler::tryToHandleGeneralError<double>([this] {
        double height = readHeight();
        if (height > 900)
        {
            throw GeneralError("Laser", tr("Error laser height %1").arg(height));
        }
        else
        {
            return height;
        }
    });
}

double HTG1Laser::readHeight()
{
    return laserDriver->readHeight(laserStationConfig->stationNum());
}

double HTG1Laser::readHeightNoErrorHandling()
{
    SCTimer t("readHeight", laserStationConfig->logTime());
    return laserDriver->readHeightNoErrHandling(laserStationConfig->stationNum());
}

void HTG1Laser::startContinuousReadHeight(int samplePeriod)
{
    if (isContinuousReadHeight)
    {
        return;
    }
    this->samplePeriod = samplePeriod;
    isContinuousReadHeight = true;
    start();
}

void HTG1Laser::stopContinuousReadHeight()
{
    isContinuousReadHeight = false;
}

void HTG1Laser::run()
{
    try
    {
        while (isContinuousReadHeight)
        {
            double height = readHeightNoErrorHandling();
            setCurrentHeight(height);
            QThread::msleep(samplePeriod);
        }
    }
    catch (SilicoolException &se)
    {
        qCritical() << se.what();
    }
    isContinuousReadHeight = false;
}
