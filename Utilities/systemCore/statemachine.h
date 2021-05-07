#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "alarmhelper.h"
#include "dispatcher.h"
#include "errorHandling/silicolerror.h"
#include "loging/Logger.h"
#include "statemashinedefinition.h"
#include <QObject>
#include <QThread>

class StateMachine;

class SilicoolState : public QObject
{
    Q_OBJECT
public:
    SilicoolState(StateMachine *stateMachine, SMD::State state);

    SMD::State state() const
    {
        return m_state;
    }

    void addTransition(SMD::Event event, SilicoolState *targetState)
    {
        transitions[event] = targetState;
    }

    bool canTransite(SMD::Event event)
    {
        return transitions.contains(event);
    }

    void transite(SMD::Event event);

    void active();

signals:
    void entered();

private:
    friend class StateMachine;

    StateMachine *m_stateMachine;
    SMD::State m_state;
    QMap<SMD::Event, SilicoolState *> transitions;
};

class UTILITIESSHARED_EXPORT StateMachine : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(SMD::State currentState READ currentState WRITE setCurrentState NOTIFY currentStateChanged)
    Q_PROPERTY(QString stateStringDescription READ stateStringDescription WRITE setStateStringDescription NOTIFY stateStringDescriptionChanged)
    Q_PROPERTY(QString stateColorDescription READ stateColorDescription WRITE setStateColorDescription NOTIFY stateColorDescriptionChanged)

    Q_PROPERTY(bool initEnabled READ initEnabled WRITE setInitEnabled NOTIFY initEnabledChanged)
    Q_PROPERTY(bool purgeEnabled READ purgeEnabled WRITE setPurgeEnabled NOTIFY purgeEnabledChanged)
    Q_PROPERTY(bool startEnabled READ startEnabled WRITE setStartEnabled NOTIFY startEnabledChanged)
    Q_PROPERTY(bool pauseEnabled READ pauseEnabled WRITE setPauseEnabled NOTIFY pauseEnabledChanged)
    Q_PROPERTY(bool continueEnabled READ continueEnabled WRITE setContinueEnabled NOTIFY continueEnabledChanged)
    Q_PROPERTY(bool stopEnabled READ stopEnabled WRITE setStopEnabled NOTIFY stopEnabledChanged)
    Q_PROPERTY(bool resetEnabled READ resetEnabled WRITE setResetEnabled NOTIFY resetEnabledChanged)

    StateMachine(Dispatcher *dispatcher, QObject *parent = nullptr);

    ~StateMachine() override;

    MySqlTableModel *alarmModel() const
    {
        return alarmHelper.alarmModel();
    }

    SMD::State currentState() const
    {
        return m_currentState;
    }

    bool initEnabled() const
    {
        return m_initEnabled;
    }

    bool purgeEnabled() const
    {
        return m_purgeEnabled;
    }

    bool startEnabled() const
    {
        return m_startEnabled;
    }

    bool pauseEnabled() const
    {
        return m_pauseEnabled;
    }

    bool stopEnabled() const
    {
        return m_stopEnabled;
    }

    bool continueEnabled() const
    {
        return m_continueEnabled;
    }

    bool resetEnabled() const
    {
        return m_resetEnabled;
    }

    QString stateStringDescription() const
    {
        return m_stateStringDescription;
    }

    QString stateColorDescription() const
    {
        return m_stateColorDescription;
    }

public slots:
    void postEvent(SMD::Event event)
    {
        currentSilicoolState->transite(event);
    }

    void setRunningParameter(QVariantMap param)
    {
        dispatcher->setRunningParameter(param);
    }

private slots:
    void setInitEnabled(bool initEnabled)
    {
        if (m_initEnabled == initEnabled)
            return;

        m_initEnabled = initEnabled;
        emit initEnabledChanged(m_initEnabled);
    }

    void setPurgeEnabled(bool purgeEnabled)
    {
        if (m_purgeEnabled == purgeEnabled)
            return;

        m_purgeEnabled = purgeEnabled;
        emit purgeEnabledChanged(m_purgeEnabled);
    }

    void setStartEnabled(bool startEnabled)
    {
        if (m_startEnabled == startEnabled)
            return;

        m_startEnabled = startEnabled;
        emit startEnabledChanged(m_startEnabled);
    }

    void setPauseEnabled(bool pauseEnabled)
    {
        if (m_pauseEnabled == pauseEnabled)
            return;

        m_pauseEnabled = pauseEnabled;
        emit pauseEnabledChanged(m_pauseEnabled);
    }

    void setStopEnabled(bool stopEnabled)
    {
        if (m_stopEnabled == stopEnabled)
            return;

        m_stopEnabled = stopEnabled;
        emit stopEnabledChanged(m_stopEnabled);
    }

    void setCurrentState(SMD::State currentState)
    {
        if (m_currentState == currentState)
            return;

        m_currentState = currentState;
        emit currentStateChanged(m_currentState);
    }

    void setStateStringDescription(QString stateStringDescription)
    {
        if (m_stateStringDescription == stateStringDescription)
            return;

        m_stateStringDescription = stateStringDescription;
        emit stateStringDescriptionChanged(m_stateStringDescription);
    }

    void setStateColorDescription(QString stateColorDescription)
    {
        if (m_stateColorDescription == stateColorDescription)
            return;

        m_stateColorDescription = stateColorDescription;
        emit stateColorDescriptionChanged(m_stateColorDescription);
    }

    void setResetEnabled(bool resetEnabled)
    {
        if (m_resetEnabled == resetEnabled)
            return;

        m_resetEnabled = resetEnabled;
        emit resetEnabledChanged(m_resetEnabled);
    }

    void setContinueEnabled(bool continueEnabled)
    {
        if (m_continueEnabled == continueEnabled)
            return;

        m_continueEnabled = continueEnabled;
        emit continueEnabledChanged(m_continueEnabled);
    }

public slots:
    void errorOccured(QString errorId, QString processName, QString module, QString msg);
    void errorHandled(QString errorId);
    void requestAbort();
    void forceAbort();

private slots:
    void onRunning();
    void onInited(bool result);
    void onPaused();
    void onPurged();
    void onStopped();
    void onAborted();
    void onReseted();
    void onReqHandleEvent(SMD::Event event);

signals:
    void startSignal();
    void continueSignal();
    void reqShowBuzyStatus(QString buzyContent);
    void reqHideBuzyStatus();

signals:
    void currentStateChanged(SMD::State currentState);

    void initEnabledChanged(bool InitEnabled);

    void purgeEnabledChanged(bool PurgeEnabled);

    void startEnabledChanged(bool StartEnabled);

    void pauseEnabledChanged(bool PauseEnabled);

    void stopEnabledChanged(bool StopEnabled);

    void stateStringDescriptionChanged(QString stateStringDescription);

    void stateColorDescriptionChanged(QString stateColorDescription);

    void resetEnabledChanged(bool resetEnabled);

    void continueEnabledChanged(bool continueEnabled);

private:
    void setCurrentState(SilicoolState *currentState);
    void init();

private:
    friend class SilicoolState;
    friend class Dispatcher;

    Dispatcher *dispatcher;

    bool m_initEnabled = false;
    bool m_purgeEnabled = false;
    bool m_startEnabled = false;
    bool m_pauseEnabled = false;
    bool m_stopEnabled = false;
    bool m_resetEnabled = false;
    bool m_continueEnabled = false;
    SMD::State m_currentState;
    QString m_stateStringDescription;
    QString m_stateColorDescription;

    SilicoolState *currentSilicoolState = nullptr;
    SilicoolState *lastSilicoolState = nullptr;
    SilicoolState *lastStateBeforeError = nullptr;

    SilicoolState *idleState;
    SilicoolState *initializingState;
    SilicoolState *initializedState;
    SilicoolState *purgingState;
    SilicoolState *readyState;
    SilicoolState *runningState;
    SilicoolState *pausingState;
    SilicoolState *pausedState;
    SilicoolState *stoppingState;
    SilicoolState *errorState;
    SilicoolState *abortingState;
    SilicoolState *abortedState;
    SilicoolState *resetingState;

    QList<QString> errors;

    QMap<SMD::State, QString> state2ColorDescription;

    AlarmHelper alarmHelper;
};

#endif    // STATEMACHINE_H
