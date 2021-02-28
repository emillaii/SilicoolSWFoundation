#include "statemachine.h"

SILICOOL_CREATE_LOGGING_CATEGORY(smCate, "StateMachine")

SilicoolState::SilicoolState(StateMachine *stateMachine, SMD::State state) : QObject(stateMachine), m_stateMachine(stateMachine), m_state(state) {}

void SilicoolState::transite(SMD::Event event)
{
    if (!canTransite(event))
    {
        qFatal("Can not process event %d on current state %d", event, m_state);
    }
    SilicoolState *targetState = transitions[event];
    targetState->active();
}

void SilicoolState::active()
{
    m_stateMachine->setCurrentState(this);
    emit entered();
}

StateMachine::StateMachine(Dispatcher *dispatcher, QObject *parent) : QObject(parent), dispatcher(dispatcher)
{
    init();
}

StateMachine::~StateMachine() {}

void StateMachine::errorOccured(QString errorId, QString processName, QString module, QString msg)
{
    qCCritical(smCate()) << tr("Error occur: id: %1, process: %2, module: %3, msg: %4").arg(errorId).arg(processName).arg(module).arg(msg);
    if (currentSilicoolState != errorState)
    {
        if (currentSilicoolState == purgingState || currentSilicoolState == runningState || currentSilicoolState == stoppingState
            || currentSilicoolState == pausingState)
        {
            lastStateBeforeError = currentSilicoolState;
            errorState->active();
        }
        else if (currentSilicoolState == abortingState)
        {
        }
        else
        {
            qCritical(smCate()) << tr("Error occur while current state is %1").arg(stateStringDescription());
        }
    }
    errors.append(errorId);
}

void StateMachine::errorHandled(QString errorId)
{
    if (errors.contains(errorId))
    {
        errors.removeOne(errorId);
    }
    else
    {
        qCCritical(smCate()) << tr("Unknown error id:") << errorId;
    }

    if (errors.count() == 0)
    {
        if (currentSilicoolState == errorState)
        {
            setCurrentState(lastStateBeforeError);
        }
    }
}

void StateMachine::requestAbort()
{
    if (currentSilicoolState != abortingState && currentSilicoolState != abortedState)
    {
        abortingState->active();
    }
}

void StateMachine::forceAbort()
{
    abortingState->active();
}

void StateMachine::onRunning()
{
    if (lastSilicoolState == pausedState)
    {
        emit continueSignal();
    }
    else if (lastSilicoolState == readyState)
    {
        emit startSignal();
    }
    else
    {
        qCCritical(smCate()) << tr("Error transition from %1 to running!").arg(lastSilicoolState->state());
    }
}

void StateMachine::onInited(bool result)
{
    if (result)
    {
        errors.clear();
        initializedState->active();
    }
    else
    {
        idleState->active();
    }
}

void StateMachine::onPaused()
{
    pausedState->active();
}

void StateMachine::onPurged()
{
    readyState->active();
}

void StateMachine::onStopped()
{
    readyState->active();
}

void StateMachine::onAborted()
{
    abortedState->active();
}

void StateMachine::onReseted()
{
    errors.clear();
    idleState->active();
}

void StateMachine::onReqHandleEvent(SMD::Event event)
{
    if (currentSilicoolState->canTransite(event))
    {
        currentSilicoolState->transite(event);
    }
    else
    {
        qCCritical(smCate())
            << QString("Unknown event '%1' base on current state: '%2'!").arg(SMD::EventEnumInfo().enumToName(event)).arg(stateStringDescription());
    }
}

void StateMachine::setCurrentState(SilicoolState *currentState)
{
    if (currentState == currentSilicoolState)
    {
        return;
    }

    lastSilicoolState = currentSilicoolState;
    currentSilicoolState = currentState;

    setCurrentState(currentSilicoolState->state());

    setStateStringDescription(SMD::StateEnumInfo().enumToName(m_currentState));
    setStateColorDescription(state2ColorDescription[m_currentState]);

    QStringList eventEnumNames = SMD::EventEnumInfo().names();
    if (eventEnumNames.contains("Abort"))
    {
        eventEnumNames.removeOne("Abort");
    }
    foreach (QString eventName, eventEnumNames)
    {
        QString eventEnabled = eventName.toLower() + QString("Enabled");
        bool enabled = currentSilicoolState->canTransite(SMD::EventEnumInfo().nameToEnum(eventName));
        if (!setProperty(eventEnabled.toUtf8(), enabled))
        {
            qFatal("Set property %s to %d failed!", eventEnabled.toUtf8().data(), enabled);
        }
    }
}

void StateMachine::init()
{
    idleState = new SilicoolState(this, SMD::Idle);
    initializingState = new SilicoolState(this, SMD::Initializing);
    initializedState = new SilicoolState(this, SMD::Initialized);
    purgingState = new SilicoolState(this, SMD::Purging);
    readyState = new SilicoolState(this, SMD::Ready);
    runningState = new SilicoolState(this, SMD::Running);
    pausingState = new SilicoolState(this, SMD::Pausing);
    pausedState = new SilicoolState(this, SMD::Paused);
    stoppingState = new SilicoolState(this, SMD::Stopping);
    errorState = new SilicoolState(this, SMD::Error);
    abortingState = new SilicoolState(this, SMD::Aborting);
    abortedState = new SilicoolState(this, SMD::Aborted);
    resetingState = new SilicoolState(this, SMD::Reseting);

    state2ColorDescription[SMD::Idle] = "Gray";
    state2ColorDescription[SMD::Initializing] = "Yellow";
    state2ColorDescription[SMD::Initialized] = "LimeGreen";
    state2ColorDescription[SMD::Purging] = "Yellow";
    state2ColorDescription[SMD::Ready] = "LimeGreen";
    state2ColorDescription[SMD::Running] = "Yellow";
    state2ColorDescription[SMD::Pausing] = "Yellow";
    state2ColorDescription[SMD::Paused] = "LimeGreen";
    state2ColorDescription[SMD::Stopping] = "Yellow";
    state2ColorDescription[SMD::Error] = "Red";
    state2ColorDescription[SMD::Aborting] = "Red";
    state2ColorDescription[SMD::Aborted] = "Red";
    state2ColorDescription[SMD::Reseting] = "Yellow";

    connect(dispatcher, &Dispatcher::reqShowBuzyStatus, this, &StateMachine::reqShowBuzyStatus);
    connect(dispatcher, &Dispatcher::reqHideBuzyStatus, this, &StateMachine::reqHideBuzyStatus);
    connect(
        dispatcher, &Dispatcher::reqPublishCurrentState, this, [this] { emit currentStateChanged(m_currentState); }, Qt::DirectConnection);
    connect(dispatcher, &Dispatcher::sendEventToSM, this, &StateMachine::onReqHandleEvent, Qt::DirectConnection);

    idleState->addTransition(SMD::Init, initializingState);
    connect(initializingState, &SilicoolState::entered, dispatcher, &Dispatcher::onInit);
    connect(dispatcher, &Dispatcher::inited, this, &StateMachine::onInited, Qt::DirectConnection);
    initializedState->addTransition(SMD::Purge, purgingState);
    connect(initializedState, &SilicoolState::entered, this, [this] { postEvent(SMD::Purge); });    // Auto purge
    connect(purgingState, &SilicoolState::entered, dispatcher, &Dispatcher::onPurge);
    connect(dispatcher, &Dispatcher::purged, this, &StateMachine::onPurged, Qt::DirectConnection);
    readyState->addTransition(SMD::Start, runningState);
    connect(runningState, &SilicoolState::entered, this, &StateMachine::onRunning);
    connect(this, &StateMachine::startSignal, dispatcher, &Dispatcher::onStart);
    connect(this, &StateMachine::continueSignal, dispatcher, &Dispatcher::onContinue);
    runningState->addTransition(SMD::Stop, stoppingState);
    connect(stoppingState, &SilicoolState::entered, dispatcher, &Dispatcher::onStop);
    connect(dispatcher, &Dispatcher::stopped, this, &StateMachine::onStopped, Qt::DirectConnection);
    runningState->addTransition(SMD::Pause, pausingState);
    connect(pausingState, &SilicoolState::entered, dispatcher, &Dispatcher::onPause);
    connect(dispatcher, &Dispatcher::paused, this, &StateMachine::onPaused, Qt::DirectConnection);
    pausedState->addTransition(SMD::Continue, runningState);
    connect(abortingState, &SilicoolState::entered, dispatcher, &Dispatcher::onAbort, Qt::DirectConnection);
    connect(dispatcher, &Dispatcher::aborted, this, &StateMachine::onAborted, Qt::DirectConnection);
    abortedState->addTransition(SMD::Reset, resetingState);
    connect(resetingState, &SilicoolState::entered, dispatcher, &Dispatcher::onReset);
    connect(dispatcher, &Dispatcher::reseted, this, &StateMachine::onReseted, Qt::DirectConnection);

    idleState->active();
}
