#ifndef ACSAXIS_H
#define ACSAXIS_H

#include "BasicElement/scaxis.h"
#include "acsaxisconfig.h"
#include "ACSC.h"
#include "acscardconfig.h"
#include "acserrorhandle.h"

#define VelCoeff (1)
#define AccCoeff (1)

class ACSAxis : public SCAxis
{
    Q_OBJECT

public:

    ACSAxis(QString name, QObject *parent = nullptr);
    ACSAxisConfig *getACSAxisConfig() const
    {
        return qobject_cast<ACSAxisConfig *>(config());
    }

    void kill() ;

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
    virtual void clearStatus() override;

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override;
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
        // do nothing
        return true;
    }
    virtual void scaleMaxAccImpl(double ratio) override;

    void clearPosImpl() override
    {

    }
    void syncProfilerAndFeedbackPos() override
    {

    }
    void velocityMoveImpl(Direction dir, double vel, double acc)override;
    void scaleMaxVelImpl(double ratio);

private:
    ACSAxisConfig *acsAxisConfig = nullptr;
    HANDLE hComm;
};
#endif    // ACSAXIS_H
