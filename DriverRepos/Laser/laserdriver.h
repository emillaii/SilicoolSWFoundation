#ifndef LASERDRIVER_H
#define LASERDRIVER_H

#include "errorHandling/silicolerrorhandler.h"
#include "highpricisiontimer.h"
#include "laserconfig.h"
#include "serialPortWrapper/scserialport.h"
#include "singletonthread.h"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

class LaserDriver : public QObject
{
    Q_OBJECT

public:
    explicit LaserDriver(SerialPortConfig *serialPortConfig, QObject *parent = nullptr);

    void init(LaserStationConfig::SamplePeriod samplePeriod, int stationNum);

    void dispose();

    ///
    /// \brief readHeight
    /// \return return +999.99 while error occur
    ///
    double readHeight(int stationNum);

    double readHeightNoErrHandling(int stationNum);

private:
    void executeCommand(QString command, QString data, int stationNum);

    QByteArray createComand(QString cmdName, QString data, int stationNum);

    QString extractResponseData(QString response, QString targetCmd, int targetStation);

private:
    SCSerialPort *serialPort;
    QMap<LaserStationConfig::SamplePeriod, QString> sp2Data;
    QMutex locker;
};

#endif    // LASERDRIVER_H
