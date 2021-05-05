#include "worker.h"

QString Worker::workerConfigDir = "";
QMap<QString, Worker *> Worker::workers;

Worker::Worker(QString workerName, QObject *parent)
    : QThread(parent), m_workerName(workerName), workerNameBytes(workerName.toUtf8()), m_logCate(workerNameBytes), m_currentOrLastEvent("Invalid")
{
    Logger::getIns()->registerCategory(&m_logCate);
    m_currentState = SMD::Idle;
    workers[workerName] = this;
}

void Worker::handleDebugEvent(QString eventName, QVariantList args)
{
    if (!m_isInit)
    {
        UIOperation::getIns()->showError(tr("Worker %1 is not inited!").arg(m_workerName));
        return;
    }
    if (hasUnhandledEvent())
    {
        QString msg = tr("%1 is buzy! Are you sure to push event '%2' to %1?\r\n"
                         "If yes, event '%2' will be handled after all other events being handled!")
                          .arg(workerName())
                          .arg(eventName);
        auto rsp = UIOperation::getIns()->getUIResponse("Warn", msg, MsgBoxIcon::Warning, YesNoBtns);
        if (rsp != YesBtn)
        {
            return;
        }
    }
    pushEvent(WorkerEvent(eventName, args, 1));
}

void Worker::pushEvent(const WorkerEvent &event)
{
    if (event.isFrameworkEvent)
    {
        eventQueue.enqueue(event);
        startRun();
    }
    else
    {
        if (m_currentState == SMD::Aborted)
        {
            qCCritical(logCate()) << tr("Worker %1 is aborted! Event will be lost. Event:").arg(workerName()) << event;
            return;
        }
        eventQueue.enqueue(event);
        qCDebug(m_logCate) << tr("Push event to queue:") << event;
        if (m_currentState != SMD::Paused)
        {
            startRun();
        }
    }
}

void Worker::processEvent(const WorkerEvent &event)
{
    processEvent(event, false);
}

const WorkerEvent &Worker::currentOrLastEvent() const
{
    return m_currentOrLastEvent;
}

void Worker::checkIsInit()
{
    if (!m_isInit)
    {
        throw SilicolAbort(tr("Worker %1 is not inited!").arg(m_workerName));
    }
}

bool Worker::hasAbortEvent()
{
    if (lastEventIsAbort)
    {
        return true;
    }
    WorkerEvent head;
    if (eventQueue.tryGetHead(head))
    {
        return head.isFrameworkEvent && head.eventName == "Abort";
    }
    else
    {
        return false;
    }
}

bool Worker::hasAutoStopEvent()
{
    if (lastEventIsAutoStop)
    {
        return true;
    }
    WorkerEvent head;
    if (eventQueue.tryGetHead(head))
    {
        return head.isFrameworkEvent && head.eventName == "__AutoStop";
    }
    else
    {
        return false;
    }
}

bool Worker::hasAbortOrAutoStopEvent()
{
    return hasAbortEvent() || hasAutoStopEvent();
}

bool Worker::hasUnhandledEvent()
{
    return !eventQueue.isEmpty();
}

void Worker::stopEventHandled()
{
    m_currentState = SMD::Ready;
    emit workerEventHandled(m_workerName, SMD::Stop, {});
}

void Worker::purgeEventHandled()
{
    m_currentState = SMD::Ready;
    emit workerEventHandled(m_workerName, SMD::Purge, {});
}

Worker *Worker::getWorker(QString workerName) const
{
    if (workers.contains(workerName))
    {
        return workers[workerName];
    }
    else
    {
        qCWarning(logCate()) << "Undefined worker:" << workerName;
        return nullptr;
    }
}

void Worker::waitWorkerThreadStopped(QStringList workerNames)
{
    QList<Worker *> t_workers;
    foreach (auto workerName, workerNames)
    {
        auto worker = getWorker(workerName);
        if (worker != nullptr)
        {
            t_workers.append(worker);
        }
    }

    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (timer.elapsed() > 5000)
        {
            qCWarning(logCate()) << "waitWorkerThreadStopped..." << workerNames;
            timer.restart();
        }
        bool t_isRun = false;
        foreach (auto worker, t_workers)
        {
            t_isRun |= worker->isRunning();
        }
        if (!t_isRun)
        {
            return;
        }
        QThread::msleep(10);
    }
}

void Worker::abortEventHandled()
{
    m_currentState = SMD::Aborted;
    emit workerEventHandled(m_workerName, SMD::Abort, {});
}

void Worker::processEvent(const WorkerEvent &event, bool fromEventQueue)
{
    qCDebug(m_logCate) << tr("Process event:") << event << tr("FromEventQueue:") << fromEventQueue;

    lastEventIsAbort = (event.isFrameworkEvent && event.eventName == "Abort");
    lastEventIsAutoStop = (event.isFrameworkEvent && event.eventName == "__AutoStop");
    m_currentOrLastEvent = event;
    if (event.isFrameworkEvent)
    {
        if (event.eventName == "Init")
        {
            initEventHandle();
            m_isInit = true;
            m_currentState = SMD::Initialized;
        }
        else if (event.eventName == "__Home")
        {
            try
            {
                checkIsInit();
                if (event.eventArgs.type() != QVariant::Bool)
                {
                    throw SilicolAbort(tr("Unmatched parameter type! Expected type: bool, given type: %1").arg(event.eventArgs.typeName()),
                                       EX_LOCATION);
                }
                homeWorker(event.eventArgs.toBool());
                emit homeDone(workerName(), true, {});
            }
            catch (SilicoolException &se)
            {
                qCCritical(logCate()) << se.what();
                emit homeDone(workerName(), false, {});
            }
        }
        else if (event.eventName == "__AllWorkerStopped")
        {
            allWorkerStopped();
        }
        else if (event.eventName == "Start")
        {
            m_currentState = SMD::Running;
            startEventHandle(event);
        }
        else if (event.eventName == "Stop")
        {
            m_currentState = SMD::Stopping;
            stopEventHandle();
        }
        else if (event.eventName == "__AutoStop")
        {
            clearEvent();
            stopEventHandled();
        }
        else if (event.eventName == "Pause")
        {
            m_currentState = SMD::Paused;
            isRun = false;
        }
        else if (event.eventName == "Continue")
        {
            m_currentState = SMD::Running;
            isRun = true;
            start();
        }
        else if (event.eventName == "Purge")
        {
            m_currentState = SMD::Purging;
            purgeEventHandle();
        }
        else if (event.eventName == "Abort")
        {
            isRun = false;
            eventQueue.clear();
            abortEventHandled();
        }
        else if (event.eventName == "Reset")
        {
            eventQueue.clear();
            if (m_isInit)
            {
                resetEventHandle();
            }
            else
            {
                qCWarning(logCate()) << QString("Worker %1 is not inited, Reset event is ignored...").arg(workerName());
            }
            m_currentState = SMD::Idle;
        }
        else
        {
            throw SilicolAbort(tr("Unhandled framework event: %1").arg(event.eventName), EX_LOCATION);
        }
    }
    else
    {
        if (!customEventHandle(event))
        {
            throw SilicolAbort(tr("Unhandled custom event: %1").arg(event.eventName), EX_LOCATION);
        }
    }
}

void Worker::run()
{
    WorkerEvent event;
    while (isRun && eventQueue.tryDequeue(event))
    {
        try
        {
            processEvent(event, true);
        }
        catch (StoppedError &se)
        {
            se.what();
        }
        catch (SilicoolException &se)
        {
            qCCritical(m_logCate) << se.what();
            processEvent(WorkerEvent(SMD::Abort));
            ErrorReporter::getIns().requestAbort();
        }
    }
}

void Worker::stopEventHandle()
{
    WorkerEvent autoStop("__AutoStop", QVariant(), INT_MAX - 1);
    autoStop.isFrameworkEvent = true;
    pushEvent(autoStop);
}

void Worker::allWorkerStopped() {}

void Worker::purgeEventHandle()
{
    purgeEventHandled();
}

bool Worker::customEventHandle(const WorkerEvent &event)
{
    Instruction ins(this, event.eventName, event.eventArgs.toList());
    ins.execute();
    return true;
}

void Worker::startRun()
{
    if (!isRunning())
    {
        isRun = true;
        start();
    }
}

QString Worker::getWorkerConfigDir()
{
    return workerConfigDir;
}

void Worker::setWorkerConfigDir(QString value)
{
    workerConfigDir = value;
}

QString Worker::constWorkerConfigDir()
{
    return "./config/workers";
}

void Worker::clearEvent()
{
    WorkerEvent event;
    while (eventQueue.tryDequeue(event))
    {
        qCWarning(logCate()) << "Event was cleared:" << event.eventName;
    }
}
