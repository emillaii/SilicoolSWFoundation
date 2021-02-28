#ifndef XTVCM_H
#define XTVCM_H

#include "C_SoftLanding_dll.h"
#include "VCM_init_Struct.h"
#include "xtaxis.h"
#include "xtdi.h"
#include "xtvcmconfig.h"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

using namespace XT_Controler_Extend;

class XtVcm : public XtAxis
{
    Q_OBJECT

public:
    explicit XtVcm(QString name, QObject *parent = nullptr);

    static void initAllVcm();
    static void releaseAllVcm();

    Q_INVOKABLE void showZeroIoState(bool isShow);

private:
    void createVcmResource();
    void setMoveDirection(bool beforeHome);
    void setSoftLandingParameter(double vel, double targetPos, double force, double margin);
    void checkHomeRes(int res);

    // SCAxis interface
public:
    virtual void setConfig(AxisConfig *config) override;
    virtual bool hasAlarm() noexcept override;
    virtual void clearErrorImpl() override;
    virtual bool isInPos() noexcept override;
    virtual double positiveLimit() const override
    {
        return vcmConfig->positiveLimit();
    }
    virtual double negativeLimit() const override
    {
        return vcmConfig->negativeLimit();
    }
    virtual double maxVelocity() const override
    {
        return vcmConfig->maxVel();
    }
    virtual double maxAcc() const override
    {
        return vcmConfig->maxAcc();
    }

public:
    virtual void fastSoftLanding(double vel, double targetPos) override;

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override;
    virtual void operationAfterHome() override;
    virtual void stopImpl() noexcept override;
    virtual void enableImpl() override;
    virtual void disableImpl() override;
    virtual double getFeedbackPosImpl() noexcept override;
    virtual void moveToImpl(double targetPos) override;
    virtual void scaleMaxAccImpl(double ratio) override;
    virtual void scaleMaxVelImpl(double ratio) override;
    virtual void clearPosImpl() override;
    virtual void syncProfilerAndFeedbackPos() override;

    // For VCM
protected:
    virtual void softLandDownImpl(double vel, double targetPos, double force, double margin) override;
    virtual void softLandUpImpl() override;
    virtual bool isSoftLandDownFinished() noexcept override;
    virtual bool isSoftLandUpFinished() noexcept override;

private:
    static int vcmCount;
    static QList<VCM_Resource_struct> vcmResources;
    static QList<XtVcm *> xtVcms;
    static bool isInitAllVcm;
    static QMutex getNowPosLocker;
    static QMutex setSoftLandingParamLocker;

    int vcmId;
    int originId;
    int origin2Id;
    int softlandingSlotId;
    XtVcmConfig *vcmConfig = nullptr;
    bool isDirectionOpposite = false;
    bool gotIndexIO = false;
    bool isShowZeroIOState = false;
};

#endif    // XTVCM_H
