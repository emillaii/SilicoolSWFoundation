#ifndef MOVEANDPRCONFIG_H
#define MOVEANDPRCONFIG_H

#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class AxisMoveConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(double pos1 READ pos1 WRITE setPos1 NOTIFY pos1Changed)
    Q_PROPERTY(double pos2 READ pos2 WRITE setPos2 NOTIFY pos2Changed)
    Q_PROPERTY(double prPos READ prPos WRITE setPrPos NOTIFY prPosChanged)

public:
    Q_INVOKABLE AxisMoveConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    QString axisName() const
    {
        return m_axisName;
    }

    double pos1() const
    {
        return m_pos1;
    }

    double pos2() const
    {
        return m_pos2;
    }

    double prPos() const
    {
        return m_prPos;
    }

public slots:
    void setAxisName(QString axisName)
    {
        if (m_axisName == axisName)
            return;

        m_axisName = axisName;
        emit axisNameChanged(m_axisName);
    }

    void setPos1(double pos1)
    {
        if (qFuzzyCompare(m_pos1, pos1))
            return;

        m_pos1 = pos1;
        emit pos1Changed(m_pos1);
    }

    void setPos2(double pos2)
    {
        if (qFuzzyCompare(m_pos2, pos2))
            return;

        m_pos2 = pos2;
        emit pos2Changed(m_pos2);
    }

    void setPrPos(double prPos)
    {
        if (qFuzzyCompare(m_prPos, prPos))
            return;

        m_prPos = prPos;
        emit prPosChanged(m_prPos);
    }

signals:
    void axisNameChanged(QString axisName);

    void pos1Changed(double pos1);

    void pos2Changed(double pos2);

    void prPosChanged(double prPos);

private:
    QString m_axisName;
    double m_pos1 = -1;
    double m_pos2 = 1;
    double m_prPos = 0;
};

class MoveAndPrConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigObjectArray *axisMoveConfigs READ axisMoveConfigs)
    Q_PROPERTY(
        QString visionLocationName READ visionLocationName WRITE setVisionLocationName NOTIFY visionLocationNameChanged)
    Q_PROPERTY(int times READ times WRITE setTimes NOTIFY timesChanged)

public:
    MoveAndPrConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_axisMoveConfigs = new ConfigObjectArray(&AxisMoveConfig::staticMetaObject, this);
        init();
    }

    ConfigObjectArray *axisMoveConfigs() const
    {
        return m_axisMoveConfigs;
    }

    QString visionLocationName() const
    {
        return m_visionLocationName;
    }

    int times() const
    {
        return m_times;
    }

public slots:
    void setVisionLocationName(QString visionLocationName)
    {
        if (m_visionLocationName == visionLocationName)
            return;

        m_visionLocationName = visionLocationName;
        emit visionLocationNameChanged(m_visionLocationName);
    }

    void setTimes(int times)
    {
        if (m_times == times)
            return;

        m_times = times;
        emit timesChanged(m_times);
    }

signals:
    void visionLocationNameChanged(QString visionLocationName);

    void timesChanged(int times);

private:
    ConfigObjectArray *m_axisMoveConfigs;
    QString m_visionLocationName;
    int m_times = 30;
};

#endif    // MOVEANDPRCONFIG_H
