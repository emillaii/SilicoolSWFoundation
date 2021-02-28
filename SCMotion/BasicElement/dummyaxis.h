#ifndef DUMMYAXIS_H
#define DUMMYAXIS_H

#include "scaxis.h"
#include "scmotion_global.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT DummyAxis : public SCAxis
{
    Q_OBJECT
public:
    explicit DummyAxis(QString name, QObject *parent = nullptr) : SCAxis(name, parent) {}

    // SCAxis interface
public:
    virtual double getCurrentOutputPos() noexcept override
    {
        return m_currentPos;
    }
    virtual bool hasAlarm() noexcept override
    {
        return false;
    }
    virtual bool isReady() noexcept override
    {
        return true;
    }
    virtual bool isInPos() noexcept override
    {
        return true;
    }
    virtual bool isRunning() noexcept override
    {
        return false;
    }
    virtual double positiveLimit() const override
    {
        return INT_MAX;
    }
    virtual double negativeLimit() const override
    {
        return INT_MIN;
    }
    virtual double maxVelocity() const override
    {
        return 100;
    }
    virtual double getCurrentVel() noexcept override
    {
        return 0;
    }

protected:
    virtual void initImpl() override
    {
        qCInfo(motionCate()) << QString("Dummy axis %1 init...").arg(name());
    }
    virtual void homeImpl() override
    {
        qCInfo(motionCate()) << QString("Dummy axis %1 home...").arg(name());
    }
    virtual bool isHomeDone() noexcept override
    {
        return true;
    }
    virtual QString homeErrorMsg() override
    {
        return "";
    }
    virtual void operationAfterHome() override
    {
        m_currentPos = 0;
    }
    virtual void stopImpl() noexcept override {}
    virtual void stopHome() override {}
    virtual void enableImpl() override {}
    virtual void disableImpl() override {}
    virtual void clearErrorImpl() override {}
    virtual void moveToImpl(double targetPos) override
    {
        m_currentPos = targetPos;
        qCInfo(motionCate()) << QString("Dummy axis %1 move to %2...").arg(name()).arg(targetPos);
    }
    virtual void velocityMoveImpl(Direction dir, double vel, double acc) override
    {
        qCInfo(motionCate()) << QString("Dummy axis %1 move to %2 direction, Velocity %3, Acc %4...").arg(name()).arg(dir).arg(vel).arg(acc);
    }
    virtual double getFeedbackPosImpl() noexcept override
    {
        return m_currentPos;
    }
    virtual bool bindToMasterAxisImpl(bool bind) override
    {
        Q_UNUSED(bind)
        return true;
    }
    virtual void scaleMaxAccImpl(double ratio) override
    {
        Q_UNUSED(ratio)
    }
    virtual void clearPosImpl() override
    {
        m_currentPos = 0;
    }
    virtual void syncProfilerAndFeedbackPos() override {}

protected slots:
    virtual void scaleMaxVelImpl(double ratio) override
    {
        Q_UNUSED(ratio)
    }

public:
    virtual void fastSoftLanding(double vel, double targetPos) override
    {
        Q_UNUSED(vel)
        m_currentPos = targetPos;
        qCInfo(motionCate()) << QString("Dummy axis %1 fast soft landing... Target pos: %2").arg(name()).arg(targetPos);
    }

protected:
    virtual void softLandDownImpl(double vel, double targetPos, double force, double margin) override
    {
        Q_UNUSED(vel)
        Q_UNUSED(force)
        Q_UNUSED(margin)
        posBeforeSoftlandingDown = m_currentPos;
        m_currentPos = targetPos;
        qCInfo(motionCate()) << QString("Dummy axis %1 soft landing down... Target pos: %2").arg(name()).arg(targetPos);
    }
    virtual void softLandUpImpl() override
    {
        m_currentPos = posBeforeSoftlandingDown;
        qCInfo(motionCate()) << QString("Dummy axis %1 soft landing up...").arg(name());
    }
    virtual bool isSoftLandDownFinished() noexcept override
    {
        return true;
    }
    virtual bool isSoftLandUpFinished() noexcept override
    {
        return true;
    }

private:
    double m_currentPos = 0;
    double posBeforeSoftlandingDown = 0;
};

#endif    // DUMMYAXIS_H
