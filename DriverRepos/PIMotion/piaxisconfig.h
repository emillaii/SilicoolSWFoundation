#ifndef PIAXISCONFIG_H
#define PIAXISCONFIG_H

#include "BasicElement/axisconfig.h"

class PIAxisConfig : public AxisConfig
{
    Q_OBJECT
    Q_PROPERTY(int refrenceOverTime READ refrenceOverTime WRITE setRefrenceOverTime NOTIFY refrenceOverTimeChanged)
    Q_PROPERTY(int moveOverTime READ moveOverTime WRITE setMoveOverTime NOTIFY moveOverTimeChanged)
    Q_PROPERTY(QString piAxisName READ piAxisName WRITE setPiAxisName NOTIFY piAxisNameChanged)
public:
    Q_INVOKABLE PIAxisConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        init();
    }

    int refrenceOverTime() const
    {
        return m_refrenceOverTime;
    }

    int moveOverTime() const
    {
        return m_moveOverTime;
    }

    QString piAxisName() const
    {
        return m_piAxisName;
    }

public slots:

    void setRefrenceOverTime(int refrenceOverTime)
    {
        if (m_refrenceOverTime == refrenceOverTime)
            return;

        m_refrenceOverTime = refrenceOverTime;
        emit refrenceOverTimeChanged(m_refrenceOverTime);
    }

    void setMoveOverTime(int moveOverTime)
    {
        if (m_moveOverTime == moveOverTime)
            return;

        m_moveOverTime = moveOverTime;
        emit moveOverTimeChanged(m_moveOverTime);
    }

    void setPiAxisName(QString piAxisName)
    {
        if (m_piAxisName == piAxisName)
            return;

        m_piAxisName = piAxisName;
        emit piAxisNameChanged(m_piAxisName);
    }

signals:

    void refrenceOverTimeChanged(int refrenceOverTime);

    void moveOverTimeChanged(int moveOverTime);

    void piAxisNameChanged(QString piAxisName);

private:
    int m_refrenceOverTime = 50000;
    int m_moveOverTime = 50000;
    QString m_piAxisName = "";
};

#endif    // PIAXISCONFIG_H
