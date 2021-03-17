#ifndef SCAXIS_H
#define SCAXIS_H

#include "MotionManager/motionelementcontainer.h"
#include "MoveProtection/moveprotection.h"
#include "TaskEngine/instruction.h"
#include "axisconfig.h"
#include "errorHandling/scassert.h"
#include "errorHandling/silicolerror.h"
#include "globalstate.h"
#include "loghelper.h"
#include "motionutility.h"
#include "scdi.h"
#include "scdo.h"
#include "scmotion_global.h"
#include "softlandingpos.h"
#include <QThread>
#include <math.h>

class SCMOTIONSHARED_EXPORT SCAxis : public QObject
{
    Q_OBJECT
public:
    enum Direction
    {
        Positive,
        Negative
    };
    Q_ENUM(Direction)

    static Direction oppositeDir(Direction dir);

    SCAxis(QString name, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::Axis;
    }

    QString name() const
    {
        return objectName();
    }

    AxisConfig *config() const
    {
        return m_config;
    }

    virtual void setConfig(AxisConfig *config)
    {
        m_config = config;
    }

    void setMoveProtection(MoveProtection *moveProtection)
    {
        this->moveProtection = moveProtection;
    }

    bool isInit() const
    {
        return m_isInit;
    }

    virtual bool isEnable() noexcept
    {
        return m_isEnable;
    }

    bool hasHome() const
    {
        if (!m_config->advancedAxisConfig()->needHome())
        {
            return true;
        }
        return m_hasHome;
    }

    bool hasStop() const
    {
        return m_hasStop;
    }
    void clearHasStop()
    {
        m_hasStop = false;
    }

    double velocityRatio() const
    {
        return m_config->velocityRatio();
    }

    QVariant invoke(const QString &cmd, const QVariantList &args = QVariantList());

    ///
    /// \brief needReHome
    /// Set hasHome to false
    ///
    void needReHome();

    void init();

    Q_INVOKABLE void home(bool waitDone = true);
    void waitHomeDone();

    ///
    /// \brief getFeedbackPos
    /// Get feedback pos from cache.
    /// Software will push the feedback pos to cache, and give it
    /// a timestamp. If the elapsed time is greater than the given
    /// 'cacheInvalidationTime', software will update cache first.
    /// If you want to get the realtime feedback pos, give 'cacheInvalidationTime'
    /// with -1.
    /// \param cacheInvalidationTime ms
    /// \return
    ///
    double getFeedbackPos(int cacheInvalidationTime = 2);
    ///
    /// \brief getCurrentOutputPos get profiler pos
    /// \return
    ///
    virtual double getCurrentOutputPos() noexcept = 0;
    double getCurrentTargetPos() const
    {
        return currentTargetPos;
    }

    Q_INVOKABLE void enable();
    Q_INVOKABLE void disable();

    virtual bool hasAlarm() noexcept = 0;
    virtual bool isReady() noexcept = 0;
    ///
    /// \brief isInPos 反馈是否到位，一般规划正常停止，反馈位置与规划位置的误差小于设定误差带，且保持了一定时间后认为反馈到位
    /// \return
    ///
    virtual bool isInPos() noexcept = 0;
    ///
    /// \brief isRunning   规划器是否在运行
    /// \return
    ///
    virtual bool isRunning() noexcept = 0;
    virtual bool getNegativeLimitStatus() noexcept
    {
        return false;
    }
    virtual bool getPositiveLimitStatus() noexcept
    {
        return false;
    }

    Q_INVOKABLE void clearAlarm();

    void stop();
    Q_INVOKABLE void stopAndResetState();

    virtual double positiveLimit() const
    {
        return m_config->positiveLimit();
    }
    virtual double negativeLimit() const
    {
        return m_config->negativeLimit();
    }
    virtual double maxVelocity() const
    {
        return m_config->maxVel();
    }
    virtual double maxAcc() const
    {
        return m_config->maxAcc();
    }

    Q_INVOKABLE void absMoveWithLimit(double targetPos);
    Q_INVOKABLE void relMoveWithLimit(double step);

    bool isInLimitRange(double targetPos) const;
    bool isProfilerAtPos(double pos, double precision)
    {
        return !isRunning() && qAbs(getCurrentOutputPos() - pos) <= precision;
    }
    Q_INVOKABLE void absMove(double targetPos, bool waitDone = true, double precision = 0.1, int timeout = 3000);
    Q_INVOKABLE void relMove(double step, bool waitDone = true, double precision = 0.1, int timeout = 3000);
    ///
    /// \brief stepMove
    /// Be different with relMove, stepMove would not check hasHome, isInLimitRange and would ignore moveProtection,
    /// So it should be use carefully.
    ///
    Q_INVOKABLE void stepMove(double step, bool waitDone = true, double precision = 0.1, int timeout = 30000);

    void tryToMove(double targetPos, uint checkInterval = 10, int stepCount = 10, double precision = 0.1, int timeout = 60000);

    // 等待规划停止，检查规划是否停在目标位置
    void waitProfilerStopped(double targetPos);
    void waitProfilerStopped();
    // 等待反馈到位，且反馈位置与规划位置误差小于precision
    void waitInPos(double targetPos, double precision = 0.1, int timeout = 3000);
    void waitInPos(double precision = 0.1, int timeout = 3000);
    // 等待反馈位置变化量，在配置的window内小于precision
    void waitSettling(double precision = 0.01, int timeout = 3000);
    void waitSettling(int window, double precision, int timeout);    // No error handling
    // 先等待规划停止，若配置了settlingByAppLayer，则waitSettling，否则waitInPos
    void waitArrivedPos(double targetPos, double precision = 0.1, int timeout = 3000);
    void waitArrivedPos(double precision = 0.1, int timeout = 3000);
    // 等待反馈位置大于/小于/接近某一位置
    void waitGreaterThanPos(double targetPos, double precision = 1, int timeout = 30000);
    void waitLessThanPos(double targetPos, double precision = 1, int timeout = 30000);
    void waitApproachPos(double targetPos, double precision = 1, int timeout = 30000);

    ///
    /// \brief absForceMove
    /// Not check axis is running!
    /// Force move will never wait move done!
    /// \param targetPos
    ///
    Q_INVOKABLE void absForceMove(double targetPos);
    Q_INVOKABLE void relForceMove(double step);
    Q_INVOKABLE void relForceMoveWithLimit(double step);

    ///
    /// \brief scaleMaxVel
    /// set velocity ratio, will not change the ratio in config
    /// \param ratio
    ///
    void scaleMaxVel(double ratio);
    void scaleMaxAcc(double ratio);
    Q_INVOKABLE void resetMaxAcc();
    ///
    /// \brief resetMaxVel
    /// set velocity ratio with the ratio in config
    ///
    Q_INVOKABLE void resetMaxVel();
    ///
    /// \brief setNextMoveVelAccRatio
    /// vel ratio and acc ratio would be applied next absMove or relMove
    /// \param velRatio value less than or equal to 0 means use vel ratio in config
    /// \param accRatio value less than or equal to 0 means 1
    ///
    void setNextMoveVelAccRatio(double velRatio, double accRatio);
    Q_INVOKABLE void setNextMoveVelAcc(double vel, double acc);

    virtual double getCurrentVel() noexcept = 0;

    ///
    /// \brief jogMove
    /// Internal function. Just for corresponding UI.
    /// \param direction    0: Negative  1: Positive
    /// \param velocityLimit    velocity ratio limit
    ///
    Q_INVOKABLE void jogMove(int direction);
    ///
    /// \brief jogMoveWithVel
    /// Internal function. Just for corresponding UI.
    /// \param direction    0: Negative  1: Positive
    /// \param velocity     mm/s
    ///
    Q_INVOKABLE void jogMoveWithVel(int direction, double velocity);
    void moveToPositiveLimit(bool waitDone = true, double velocityRatioLimit = 1);
    void moveToNegativeLimit(bool waitDone = true, double velocityRatioLimit = 1);
    void slowMoveTo(double targetPos, bool waitDone = true, double velocityRatioLimit = 1);
    void velocityMove(Direction dir, double vel = -1, double acc = -1);

    Q_INVOKABLE void startReciprocate(double pos1, int delay1, double pos2, int delay2, int times);
    Q_INVOKABLE void stopReciprocate();

    void setMasterAxis(SCAxis *masterAxis);
    SCAxis *masterAxis() const;
    Q_INVOKABLE bool hasMasterAxis();
    Q_INVOKABLE bool hasBindedToMaterAxis();
    Q_INVOKABLE bool bindToMasterAxis();
    Q_INVOKABLE bool unBindToMasterAxis();

    Q_INVOKABLE virtual void clearStatus() {}

protected:
    void checkState(bool checkHasHome = true);
    void checkIsRunning();
    void checkInLimitRange(double targetPos) const;
    void setHasHome(bool value);

    ///
    /// \brief initImpl
    /// You can throw SilicoolAbort if any error occur.
    ///
    virtual void initImpl() = 0;

    virtual void homeImpl() = 0;
    virtual bool isHomeDone() noexcept = 0;
    virtual QString homeErrorMsg() = 0;
    ///
    /// \brief operationAfterHome
    /// You may clear current pos and feedback pos after home.
    ///
    virtual void operationAfterHome() = 0;
    virtual void stopImpl() noexcept = 0;
    virtual void stopHome() = 0;

    ///
    /// \brief clearPosImpl clear profiler pos and feedback pos
    ///
    virtual void clearPosImpl() = 0;
    ///
    /// \brief clearPosImpl set profiler pos with feedback pos
    ///
    virtual void syncProfilerAndFeedbackPos() = 0;

    virtual void enableImpl() = 0;
    virtual void disableImpl() = 0;

    virtual void clearErrorImpl() = 0;

    //    virtual void emergencyStopImpl() = 0;

    virtual void moveToImpl(double targetPos) = 0;

    virtual void velocityMoveImpl(Direction dir, double vel, double acc) = 0;

    virtual double getFeedbackPosImpl() noexcept = 0;

    virtual bool bindToMasterAxisImpl(bool bind) = 0;

    virtual void scaleMaxAccImpl(double ratio) = 0;

    virtual void scaleMaxVelImpl(double ratio) = 0;

private slots:
    void onConfigVelChanged(double v);
    void onConfigAccChanged(double v);

private:
    void startHome();
    void absMoveImpl(double targetPos, bool checkRunning = true);
    bool isArrivedPos(double targetPos, double currentPos, double precision);
    void getPosInLimitRange(double &targetPos);
    double getProfilerPrecision() const;
    void waitControllerHomeDone();
    void waitDriverHomeDone();
    void waitSettlingAfterHome();

    // -------------- For Softlanding --------------
public:
    virtual void fastSoftLanding(double vel, double targetPos)
    {
        Q_UNUSED(vel)
        Q_UNUSED(targetPos)
        throw SilicolAbort("Unimplemented function: fastSoftLanding");
    }
    void softLandPosDown(QString softLandingPos, bool waitDone = true);
    void softLandDown(double vel, double targetPos, double force, double margin, bool waitDone = true, int timeout = 30000);
    void waitSoftLandDownFinished(double vel, double targetPos, double force, double margin, int timeout = 30000);
    void waitSoftLandUpFinished(int timeout = 30000);
    void softLandUp(bool waitDone = true, int timeout = 30000);
    void setIsSoftlandingDown(bool value)    // SCAxis will auto set 'm_isSoftLandDown' while you calling 'softLandDown' or 'softLandUp'
    {
        m_isSoftLandDown = value;
    }
    bool isSoftlandingDown() const
    {
        return m_isSoftLandDown;
    }
    Q_INVOKABLE double measureHeight(double vel, double force, int holdTime);

protected:
    virtual void softLandDownImpl(double vel, double targetPos, double force, double margin)
    {
        Q_UNUSED(vel)
        Q_UNUSED(targetPos)
        Q_UNUSED(force)
        Q_UNUSED(margin)
        throw SilicolAbort("Unimplemented function: softLandDownImpl");
    }

    virtual void softLandUpImpl()
    {
        throw SilicolAbort("Unimplemented function: softLandUpImpl");
    }

    virtual bool isSoftLandDownFinished() noexcept
    {
        return false;
    }

    virtual bool isSoftLandUpFinished() noexcept
    {
        return false;
    }

private:
    void startSoftLandDown(double vel, double targetPos, double force, double margin);
    void startSoftLandUp();

private:
    const double MinStep = 0.00001;

    bool isConnectConfigChangedSignal = false;

    MoveProtection *moveProtection = nullptr;
    bool isReciprocating = false;
    AxisConfig *m_config = nullptr;
    bool m_isInit = false;
    bool m_isEnable = false;
    bool m_hasHome = false;
    double currentTargetPos = 0;
    double lastFeedbackPos = 0;
    qint64 lastUpdateFeedbackPosTimeStamp = 0;
    bool m_hasStop = false;
    bool m_hasBindToMasterAxis = false;
    SCAxis *m_masterAxis = nullptr;

    AdvancedAxisConfig::HomeExecutor homeExecutor;
    SCDI *homeDoneDi = nullptr;
    SCDO *startHomeDo = nullptr;

    double nextMoveVelRatio = -1;
    double nextMoveAccRatio = -1;
    double currentVelRatio = -1;
    double currentAccRatio = -1;

    HighPrecisionTimer axisMoveTimer;

    bool m_isSoftLandDown = false;
};

#endif    // SCAXIS_H
