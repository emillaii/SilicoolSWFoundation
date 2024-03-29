#ifndef LASERCONFIG_H
#define LASERCONFIG_H

#include "configManager/configobject.h"
#include "enumhelper.h"
#include "serialPortWrapper/scserialport.h"

class LaserStationConfig : public ConfigObject
{
    Q_OBJECT

public:
    enum SamplePeriod
    {
        SP200Us,
        SP500Us,
        SP1Ms,
        SP2Ms
    };
    Q_ENUM(SamplePeriod)

    Q_PROPERTY(SamplePeriod samplePeriod READ samplePeriod WRITE setSamplePeriod NOTIFY samplePeriodChanged)
    Q_PROPERTY(int stationNum READ stationNum WRITE setStationNum NOTIFY stationNumChanged)
    Q_PROPERTY(bool logTime READ logTime WRITE setLogTime NOTIFY logTimeChanged)

public:
    LaserStationConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    SamplePeriod samplePeriod() const
    {
        return m_samplePeriod;
    }

    int stationNum() const
    {
        return m_stationNum;
    }

    bool logTime() const
    {
        return m_logTime;
    }

public slots:
    void setSamplePeriod(SamplePeriod samplePeriod)
    {
        if (m_samplePeriod == samplePeriod)
            return;

        m_samplePeriod = samplePeriod;
        emit samplePeriodChanged(m_samplePeriod);
    }

    void setStationNum(int stationNum)
    {
        if (m_stationNum == stationNum)
            return;

        m_stationNum = stationNum;
        emit stationNumChanged(m_stationNum);
    }

    void setLogTime(bool logTime)
    {
        if (m_logTime == logTime)
            return;

        m_logTime = logTime;
        emit logTimeChanged(m_logTime);
    }

signals:
    void samplePeriodChanged(SamplePeriod samplePeriod);

    void stationNumChanged(int stationNum);

    void logTimeChanged(bool logTime);

private:
    SamplePeriod m_samplePeriod{ SP200Us };
    int m_stationNum = 0;
    bool m_logTime = false;
};

class LaserConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(SerialPortConfig *serialPortConfig READ serialPortConfig)
    Q_PROPERTY(LaserStationConfig *auxLaserConfig READ auxLaserConfig)
    Q_PROPERTY(LaserStationConfig *leftLaserConfig READ leftLaserConfig)
    Q_PROPERTY(LaserStationConfig *rightLaserConfig READ rightLaserConfig)

public:
    LaserConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_serialPortConfig = new SerialPortConfig(this);
        m_auxLaserConfig = new LaserStationConfig(this);
        m_leftLaserConfig = new LaserStationConfig(this);
        m_rightLaserConfig = new LaserStationConfig(this);
        m_serialPortConfig->setObjectName("LaserSerialPortConfig");
        m_auxLaserConfig->setObjectName("AuxLaserStationConfig");
        m_leftLaserConfig->setObjectName("LeftLaserStationConfig");
        m_rightLaserConfig->setObjectName("RightLaserStationConfig");
        init();
    }

    SerialPortConfig *serialPortConfig() const
    {
        return m_serialPortConfig;
    }

    LaserStationConfig *leftLaserConfig() const
    {
        return m_leftLaserConfig;
    }

    LaserStationConfig *rightLaserConfig() const
    {
        return m_rightLaserConfig;
    }

    LaserStationConfig *auxLaserConfig() const
    {
        return m_auxLaserConfig;
    }

private:
    SerialPortConfig *m_serialPortConfig;
    LaserStationConfig *m_leftLaserConfig;
    LaserStationConfig *m_rightLaserConfig;
    LaserStationConfig *m_auxLaserConfig;
};

#endif    // LASERCONFIG_H
