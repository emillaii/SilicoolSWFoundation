#ifndef GTAXIS_H
#define GTAXIS_H

#include "BasicElement/scaxis.h"
#include "gtaxisconfig.h"
#include "gtresulthandler.h"
#include "gts.h"

#define VelCoeff (1000)
#define AccCoeff (1000000)
#define JerkCoeff (1000000000.0)

#define CheckGTAxisResult(res, errMsg)                                                                                                               \
    if (res != CMD_SUCCESS)                                                                                                                          \
    {                                                                                                                                                \
        throw ActionError(name(), QString("%1. Error code: %2").arg(errMsg).arg(res));                                                               \
    }

class GTAxis : public SCAxis
{
    Q_OBJECT

public:
    enum MoveMode
    {
        Unknown,
        Pos,
        Jog,
        Gear
    };

    GTAxis(QString name, QObject *parent = nullptr);
    GTAxisConfig *getGtAxisConfig() const
    {
        return qobject_cast<GTAxisConfig *>(config());
    }
    void setMoveMode(MoveMode mode, bool checkCurrentMode = true);
    void followMasterAxis(GTAxis *masterAxis, long masterEven, long slaveEven);

    // SCAxis interface
public:
    virtual double getCurrentOutputPos() noexcept override;
    virtual bool hasAlarm() noexcept override;
    virtual bool isReady() noexcept override
    {
        return true;
    }
    virtual bool isInPos() noexcept override;
    virtual bool isRunning() noexcept override;
    virtual bool getNegativeLimitStatus() noexcept override;
    virtual bool getPositiveLimitStatus() noexcept override;
    virtual double getCurrentVel() noexcept override;

    virtual void clearStatus() override;

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

private slots:
    void onInposBandParamChanged(int param);
    void onSmoothParamChanged(double v);

private:
    bool refreshLimitStatus(bool isPositiveLimit);
    void leaveLimitPos(bool isPositiveLimit);
    long getAxisStatus(bool checkCache = true);

private:
    MoveMode currentMode = Unknown;
    GTAxisConfig *gtAxisConfig = nullptr;
    double lastAccRatio = 1;
    double lastVelRatio = 1;
    short coreNo;
    short index;
    THomeStatus homeStatus;

    bool isConnectGtParamChangedSig = false;

    const int AxisStatusCacheInvalidTime = 1;
    long axisStatusCache = 0;
    qint64 lastUpdateAxisStatusCacheTime = 0;
};

#endif    // GTAXIS_H
