#ifndef HTG1LASER_H
#define HTG1LASER_H

#include "configManager/configobject.h"
#include "errorHandling/silicolerrorhandler.h"
#include "highpricisiontimer.h"
#include "laserdriver.h"
#include "loghelper.h"
#include "serialPortWrapper/scserialport.h"
#include <QObject>

class HTG1Laser : public QThread
{
    Q_OBJECT

    Q_PROPERTY(double currentHeight READ currentHeight WRITE setCurrentHeight NOTIFY currentHeightChanged)

public:
    explicit HTG1Laser(LaserDriver *laserDriver, LaserStationConfig *laserStationConfig, QObject *parent = nullptr);

    Q_INVOKABLE void init();

    double readValidHeight();

    double readHeight();

    double readHeightNoErrorHandling();

    Q_INVOKABLE void startContinuousReadHeight(int samplePeriod);

    Q_INVOKABLE void stopContinuousReadHeight();

    double currentHeight() const
    {
        return m_currentHeight;
    }

public slots:
    void setCurrentHeight(double currentHeight)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_currentHeight, currentHeight))
            return;

        m_currentHeight = currentHeight;
        emit currentHeightChanged(m_currentHeight);
    }

signals:
    void currentHeightChanged(double currentHeight);

    // QThread interface
protected:
    virtual void run() override;

private:
    LaserDriver *laserDriver;
    LaserStationConfig *laserStationConfig;
    double m_currentHeight = 0;
    bool isContinuousReadHeight = false;
    int samplePeriod = 0;
};

#endif    // HTG1LASER_H
