#ifndef PIAXISCONFIG_H
#define PIAXISCONFIG_H

#include "BasicElement/axisconfig.h"

class PIAxisConfig : public AxisConfig
{
    Q_OBJECT
    Q_PROPERTY(int refrenceOverTime READ refrenceOverTime WRITE setRefrenceOverTime NOTIFY refrenceOverTimeChanged)
    Q_PROPERTY(int moveOverTime READ moveOverTime WRITE setMoveOverTime NOTIFY moveOverTimeChanged)

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

signals:

    void refrenceOverTimeChanged(int refrenceOverTime);

    void moveOverTimeChanged(int moveOverTime);

private:
    int m_refrenceOverTime = 50000;
    int m_moveOverTime = 50000;
};

#endif    // PIAXISCONFIG_H
