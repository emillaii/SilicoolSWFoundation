#ifndef SCVACUUM_H
#define SCVACUUM_H

#include "BasicElement/scdi.h"
#include "BasicElement/scdo.h"
#include "MotionManager/motionelementcontainer.h"
#include "extendedqtimer.h"
#include "motionutility.h"
#include "scmotion_global.h"
#include "singletonthread.h"
#include "vacuumconfig.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT SCVacuum : public QObject
{
    Q_OBJECT
public:
    explicit SCVacuum(QString name, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::Vacuum;
    }

    QString name() const
    {
        return objectName();
    }

    VacuumConfig *config() const;

    void setConfig(VacuumConfig *config);

    void init();

    void disableSetAndWait()
    {
        isDisableSetAndWait = true;
    }
    void enableSetAndWait()
    {
        isDisableSetAndWait = false;
    }
    void disableCheck()
    {
        isDisableCheck = true;
    }
    void enableCheck()
    {
        isDisableCheck = false;
    }
    void disableAll();
    void enableAll();

    Q_INVOKABLE bool get()
    {
        return feedbackInput->get();
    }

    Q_INVOKABLE void set(bool state, bool waitDone = true);

    ///
    /// \brief wait
    /// 若在配置的超时时间内，真空达到给定的状态，函数返回，否则报错并请求用户响应
    /// \param state
    ///
    void wait(bool state);

    ///
    /// \brief waitUntilTimeout
    /// 根据配置的超时时间，等待真空达到给定的状态
    /// \param state
    /// \return
    ///
    bool waitUntilTimeout(bool state);

    bool waitUntilTimeout(bool state, int timeout);

    bool checkFeedbackSignal(bool state);

    // just for VacuumDebugger UI element calling
    QVariantMap getDetailState();

    Q_INVOKABLE void setBlow(bool state);

private slots:
    void closeBlowOutput()
    {
        blowOutput->set(false);
    }

private:
    VacuumConfig *m_config = nullptr;
    SCDI *feedbackInput = nullptr;
    SCDO *vacuumizeOutput = nullptr;
    SCDO *blowOutput = nullptr;
    bool hasBlowOutput;

    bool isDisableSetAndWait = false;
    bool isDisableCheck = false;
    ExtendedQTimer schedulingTimer;
};

#endif    // SCVACUUM_H
