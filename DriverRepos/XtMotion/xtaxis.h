#ifndef XTAXIS_H
#define XTAXIS_H

#include "BasicElement/scaxis.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XT_MotionControler_Client_Lib.h"
#include "xtdi.h"
#include "xtdo.h"
#include <QObject>

using namespace XT_Controler_Extend;
using namespace XT_Controler;

class XtAxisConfig : public AxisConfig
{
    Q_OBJECT

    Q_PROPERTY(bool reverseAlarmInput READ reverseAlarmInput WRITE setReverseAlarmInput NOTIFY reverseAlarmInputChanged)
    Q_PROPERTY(bool reverseReadyInput READ reverseReadyInput WRITE setReverseReadyInput NOTIFY reverseReadyInputChanged)
    Q_PROPERTY(bool reverseInposInput READ reverseInposInput WRITE setReverseInposInput NOTIFY reverseInposInputChanged)
    Q_PROPERTY(double maxJerk READ maxJerk WRITE setMaxJerk NOTIFY maxJerkChanged)

public:
    Q_INVOKABLE XtAxisConfig(QObject *parent = nullptr) : AxisConfig(parent)
    {
        init();
    }

    bool reverseAlarmInput() const
    {
        return m_reverseAlarmInput;
    }

    bool reverseReadyInput() const
    {
        return m_reverseReadyInput;
    }

    bool reverseInposInput() const
    {
        return m_reverseInposInput;
    }

    double maxJerk() const
    {
        return m_maxJerk;
    }

public slots:
    void setReverseAlarmInput(bool reverseAlarmInput)
    {
        if (m_reverseAlarmInput == reverseAlarmInput)
            return;

        m_reverseAlarmInput = reverseAlarmInput;
        emit reverseAlarmInputChanged(m_reverseAlarmInput);
    }

    void setReverseReadyInput(bool reverseReadyInput)
    {
        if (m_reverseReadyInput == reverseReadyInput)
            return;

        m_reverseReadyInput = reverseReadyInput;
        emit reverseReadyInputChanged(m_reverseReadyInput);
    }

    void setReverseInposInput(bool reverseInposInput)
    {
        if (m_reverseInposInput == reverseInposInput)
            return;

        m_reverseInposInput = reverseInposInput;
        emit reverseInposInputChanged(m_reverseInposInput);
    }

    void setMaxJerk(double maxJerk)
    {
        if (qFuzzyCompare(m_maxJerk, maxJerk))
            return;

        m_maxJerk = maxJerk;
        emit maxJerkChanged(m_maxJerk);
    }

signals:
    void reverseAlarmInputChanged(bool reverseAlarmInput);

    void reverseReadyInputChanged(bool reverseReadyInput);

    void reverseInposInputChanged(bool reverseInposInput);

    void maxJerkChanged(double maxJerk);

private:
    bool m_reverseAlarmInput = false;
    bool m_reverseReadyInput = false;
    bool m_reverseInposInput = false;
    double m_maxJerk = 10000;
};

class XtAxis : public SCAxis
{
    Q_OBJECT
public:
    explicit XtAxis(QString name, QObject *parent = nullptr);

    static int generateNewAxisID();
    static int generateNewThreadID();
    static int generateNewCalcSlotID();

    int axisId() const
    {
        return masterAxisId;
    }

    double maxJerk() const
    {
        return xtAxisConfig->maxJerk();
    }

    // SCAxis interface
public:
    virtual double getCurrentOutputPos() noexcept override;
    virtual bool hasAlarm() noexcept override;
    virtual bool isReady() noexcept override;
    virtual bool isInPos() noexcept override;
    virtual bool isRunning() noexcept override;
    virtual void stopImpl() noexcept override;
    virtual double getCurrentVel() noexcept override;

protected:
    virtual void initImpl() override;
    virtual void homeImpl() override;
    virtual bool isHomeDone() noexcept override;
    virtual QString homeErrorMsg() override;
    virtual void operationAfterHome() override;
    virtual void stopHome() override;
    virtual void enableImpl() override;
    virtual void disableImpl() override;
    virtual void clearErrorImpl() override;
    virtual void moveToImpl(double targetPos) override;
    virtual void clearPosImpl() override;
    virtual void syncProfilerAndFeedbackPos() override;
    virtual void velocityMoveImpl(Direction dir, double vel, double acc) override;
    virtual double getFeedbackPosImpl() noexcept override;
    virtual void scaleMaxAccImpl(double ratio) override;
    virtual void scaleMaxVelImpl(double ratio) override;
    virtual bool bindToMasterAxisImpl(bool bind) override;

protected:
    int masterAxisId = -1;
    int slaverAxisId = -1;
    int threadId = -1;

private:
    XtDO *enableDO = nullptr;
    XtDO *clearErrorDO = nullptr;
    XtDI *inposDI = nullptr;
    XtDI *alarmDI = nullptr;
    XtDI *readyDI = nullptr;
    XtAxisConfig *xtAxisConfig = nullptr;
    static int axisCount;
    static int threadResource;
    static int calculationSlotResource;
};

#endif    // XTAXIS_H
