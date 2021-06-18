#ifndef PIAXIS_H
#define PIAXIS_H

#include "BasicElement/scaxis.h"
#include "PI_GCS2_DLL.h"
#include "piaxisconfig.h"
#include "picontrolcardconfig.h"

class PIAxis : public SCAxis
{
    Q_OBJECT

public:
    PIAxis(QString name, QObject *parent = nullptr);
    PIAxisConfig *getPiAxisConfig() const
    {
        return qobject_cast<PIAxisConfig *>(config());
    }

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

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override;
    virtual QString homeErrorMsg() override
    {
        return "";
    }
    virtual void operationAfterHome() override {}
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
    virtual void scaleMaxAccImpl(double ratio) override {
        // Todo
        qCritical() << "UnImplemented function...";
    }
    virtual void scaleMaxVelImpl(double ratio) override ;
    void clearPosImpl(){
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void syncProfilerAndFeedbackPos(){
        // Todo
        qCritical() << "UnImplemented function...";
    }
    void velocityMoveImpl(Direction dir, double vel, double acc){
        // Todo
        qCritical() << "UnImplemented function...";
    }

private:
    void refrenceIfNeed();
    void moveToPosition(double *positons);
    int getAxisIndexByName(QString axisName);

private:
    PIAxisConfig *piAxisConfig = nullptr;
    int m_controllerID = -1;
};

#endif    // PIAXIS_H
