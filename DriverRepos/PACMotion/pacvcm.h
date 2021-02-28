#ifndef PACVCM_H
#define PACVCM_H

#include "BasicElement/scaxis.h"
#include "PAC.h"
#include "pacvcmconfig.h"
#include <QMap>

class PACVcm : public SCAxis
{
    Q_OBJECT

public:
    PACVcm(QString name, QObject *parent = nullptr);

    ~PACVcm() override;

    void keepForce(int current);

    // SCAxis interface
public:
    virtual void setConfig(AxisConfig *config) override;
    virtual double getCurrentOutputPos() noexcept override;
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
    virtual void stopImpl() noexcept override
    {
        // TMP
    }
    virtual void stopHome() override;
    virtual void enableImpl() override;
    virtual void disableImpl() override;
    virtual void clearErrorImpl() override;
    virtual void moveToImpl(double targetPos) override;
    virtual void clearPosImpl() override;
    virtual void syncProfilerAndFeedbackPos() override {}
    virtual void velocityMoveImpl(Direction dir, double vel, double acc) override;
    virtual double getFeedbackPosImpl() noexcept override;
    virtual bool bindToMasterAxisImpl(bool bind) override;
    virtual void scaleMaxAccImpl(double ratio) override;
    virtual void scaleMaxVelImpl(double ratio) override;

protected:
    virtual void softLandDownImpl(double vel, double targetPos, double force, double margin) override;
    virtual void softLandUpImpl() override;
    virtual bool isSoftLandDownFinished() noexcept override;
    virtual bool isSoftLandUpFinished() noexcept override;

private:
    void setServoMode(BYTE ucMode, bool checkMode = true);
    void startSoftLanding(double vel, BYTE dir, double force);
    void waitActionDone(int timeout);

private:
    static QMap<int, QString> errCode2Str;
    static bool hasFillErrCode2Str;
    static int pacVcmCount;

    PACVcmConfig *m_vcmConfig = nullptr;
    BYTE vcmAddress = 1;
    int currentServoMode = -1;
    double startSoftLandDownPos = 0;
};

#endif    // PACVCM_H
