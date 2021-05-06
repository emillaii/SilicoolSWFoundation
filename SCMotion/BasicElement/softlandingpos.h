#ifndef SOFTLANDINGPOS_H
#define SOFTLANDINGPOS_H

#include "configManager/configobjectarray.h"
#include "errorHandling/silicolerror.h"
#include "motionelementdefinition.h"

class SCMOTIONSHARED_EXPORT SoftLandingPos : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString axisName READ axisName WRITE setAxisName NOTIFY axisNameChanged)
    Q_PROPERTY(QString posName READ posName WRITE setPosName NOTIFY posNameChanged)
    Q_PROPERTY(double targetPos READ targetPos WRITE setTargetPos NOTIFY targetPosChanged)
    Q_PROPERTY(double margin READ margin WRITE setMargin NOTIFY marginChanged)
    Q_PROPERTY(double velocity READ velocity WRITE setVelocity NOTIFY velocityChanged)
    Q_PROPERTY(double force READ force WRITE setForce NOTIFY forceChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(int measureHeightHoldTime READ measureHeightHoldTime WRITE setMeasureHeightHoldTime NOTIFY measureHeightHoldTimeChanged)

public:
    Q_INVOKABLE SoftLandingPos(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("axisName");
        setReadOnlyProperty("posName");
        setIdentityProperty("posName");
        addEvent("MeasureHeight");
        init();
    }

    QString axisName() const
    {
        return m_axisName;
    }

    double targetPos() const
    {
        return m_targetPos;
    }

    double margin() const
    {
        return m_margin;
    }

    double velocity() const
    {
        return m_velocity;
    }

    double force() const
    {
        return m_force;
    }

    int timeout() const
    {
        return m_timeout;
    }

    QString posName() const
    {
        return m_posName;
    }

    int measureHeightHoldTime() const
    {
        return m_measureHeightHoldTime;
    }

public slots:
    void setTargetPos(double targetPos)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_targetPos, targetPos))
            return;

        m_targetPos = targetPos;
        emit targetPosChanged(m_targetPos);
    }

    void setMargin(double margin)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_margin, margin))
            return;

        m_margin = margin;
        emit marginChanged(m_margin);
    }

    void setVelocity(double velocity)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_velocity, velocity))
            return;

        m_velocity = velocity;
        emit velocityChanged(m_velocity);
    }

    void setForce(double force)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_force, force))
            return;

        m_force = force;
        emit forceChanged(m_force);
    }

    void setTimeout(int timeout)
    {
        if (m_timeout == timeout)
            return;

        m_timeout = timeout;
        emit timeoutChanged(m_timeout);
    }

    void setPosName(QString posName)
    {
        if (m_posName == posName)
            return;

        m_posName = posName;
        emit posNameChanged(m_posName);
    }

    void setMeasureHeightHoldTime(int measureHeightHoldTime)
    {
        if (m_measureHeightHoldTime == measureHeightHoldTime)
            return;

        m_measureHeightHoldTime = measureHeightHoldTime;
        emit measureHeightHoldTimeChanged(m_measureHeightHoldTime);
    }

    void setAxisName(QString axisName)
    {
        if (m_axisName == axisName)
            return;

        m_axisName = axisName;
        emit axisNameChanged(m_axisName);
    }

signals:
    void targetPosChanged(double targetPos);

    void marginChanged(double margin);

    void velocityChanged(double velocity);

    void forceChanged(double force);

    void timeoutChanged(int timeout);

    void posNameChanged(QString posName);

    void measureHeightHoldTimeChanged(int measureHeightHoldTime);

    void axisNameChanged(QString axisName);

signals:
    void measureHeightReq(QString axisName, QString softLandingPosName, double vel, double force, int holdTime);

    // ConfigObject interface
public:
    virtual void handleEvent(QString event) override
    {
        if (event == "MeasureHeight")
        {
            emit measureHeightReq(axisName(), posName(), velocity(), force(), measureHeightHoldTime());
        }
        else
        {
            ConfigObject::handleEvent(event);
        }
    }

private:
    double m_targetPos = 0;
    double m_margin = 3;
    double m_velocity = 10;
    double m_force = 0.1;
    int m_timeout = 30000;
    QString m_posName;
    int m_measureHeightHoldTime = 1000;
    QString m_axisName;
};

#endif    // SOFTLANDINGPOS_H
