#ifndef ACSAXIS_H
#define ACSAXIS_H

#include "BasicElement/scaxis.h"
#include "acsaxisconfig.h"
#include "ACSC.h"
#include "acscardconfig.h"

#define CheckACSCResult(res, errMsg)                                                                                                               \
    if (res == 0)                                                                                                                          \
    {                                                                                                                                                \
        throw ActionError(name(), QString("%1. Error code: %2").arg(errMsg).arg(res));                                                               \
    }

class ACSAxis : public SCAxis
{
    Q_OBJECT

public:

    ACSAxis(QString name, QObject *parent = nullptr);
    ACSAxisConfig *getPiAxisConfig() const
    {
        return qobject_cast<ACSAxisConfig *>(config());
    }

    void kill() noexcept;

    // SCAxis interface
public:
    virtual double getCurrentOutputPos() noexcept override
    {
        return getFeedbackPosImpl();
    }
    virtual bool hasAlarm() noexcept override;
    virtual bool isReady() noexcept override
    {
        return true;
    }
    virtual bool isInPos() noexcept override;
    virtual bool isRunning() noexcept override;
    virtual double getCurrentVel() noexcept override;

    virtual void clearStatus() override{}

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override{return 0;}
    virtual QString homeErrorMsg() override{return "";}
    virtual void operationAfterHome() override{}
    virtual void stopImpl() noexcept override;
    virtual void stopHome() override;
    virtual void enableImpl() override;
    virtual void disableImpl() override;
    virtual void clearErrorImpl() override;
    virtual void moveToImpl(double targetPos) override;
    virtual double getFeedbackPosImpl() noexcept override;
    virtual bool bindToMasterAxisImpl(bool bind) override
    {
        // Todo
        qCritical() << "UnImplemented function...";
        return false;
    }
    virtual void scaleMaxAccImpl(double ratio) override{
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void clearPosImpl()
    {
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void syncProfilerAndFeedbackPos()
    {
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void velocityMoveImpl(Direction dir, double vel, double acc)
    {
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void scaleMaxVelImpl(double ratio)
    {
        // Todo
        qCritical() << "UnImplemented function...";
    }

private:
    ACSAxisConfig *acsAxisConfig = nullptr;
    HANDLE hComm;
};
#endif    // ACSAXIS_H
