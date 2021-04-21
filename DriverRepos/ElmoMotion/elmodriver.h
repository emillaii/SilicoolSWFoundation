#ifndef ELMODRIVER_H
#define ELMODRIVER_H

#include "BasicElement/scaxis.h"
#include "ElmoCommunicationCppWrapper.h"
#include "elmodriverconfig.h"

class ElmoDriver : public SCAxis
{
    Q_OBJECT

public:
    explicit ElmoDriver(QString name, QObject *parent = nullptr);
    ~ElmoDriver() override;

    Q_INVOKABLE void setCurrent(double current);
    Q_INVOKABLE void elmoMoveTo(double targetPos);

    // SCAxis interface
public:
    virtual double getCurrentOutputPos() noexcept override;
    virtual bool isEnable() noexcept override;
    virtual bool hasAlarm() noexcept override;
    virtual bool isReady() noexcept override;
    virtual bool isInPos() noexcept override;
    virtual bool isRunning() noexcept override;
    virtual double getCurrentVel() noexcept override;

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override;
    virtual QString homeErrorMsg() override;
    virtual void operationAfterHome() override;
    virtual void stopImpl() noexcept override;
    virtual void stopHome() override;
    virtual void enableImpl() override;
    virtual void disableImpl() override;
    virtual void clearErrorImpl() override;
    virtual void moveToImpl(double targetPos) override;
    virtual void velocityMoveImpl(Direction dir, double vel, double acc) override;
    virtual double getFeedbackPosImpl() noexcept override;
    virtual bool bindToMasterAxisImpl(bool bind) override;
    virtual void scaleMaxAccImpl(double ratio) override;
    virtual void scaleMaxVelImpl(double ratio) override;
    virtual void clearPosImpl() override;
    virtual void syncProfilerAndFeedbackPos() override;

protected:
    virtual void softLandDownImpl(double vel, double targetPos, double force, double margin) override;
    virtual void softLandUpImpl() override;
    virtual bool isSoftLandDownFinished() noexcept override;
    virtual bool isSoftLandUpFinished() noexcept override;

private:
    void sendCommand(QString cmd);
    bool getResponse(QString cmd, QString &response);
    int getMotionStatus();
    double getCurrentInLimitRange(double current);

private:
    ElmoDriverConfig *elmoConfig = nullptr;
    QByteArray nameBuf;
    double posBeforeSoftlanding = 0;
    char errMsg[1024] = {};

    const int CacheInvalidTime = 1;
    int msCache = -1;
    qint64 lastUpdateMsTime = 0;
};

#endif    // ELMODRIVER_H
