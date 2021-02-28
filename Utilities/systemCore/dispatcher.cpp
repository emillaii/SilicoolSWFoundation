#include "dispatcher.h"

ShowBuzyStatusHelper::ShowBuzyStatusHelper(Dispatcher *dispatcher, QString buzyContent) : dispatcher(dispatcher)
{
    emit dispatcher->reqShowBuzyStatus(buzyContent);
}

ShowBuzyStatusHelper::~ShowBuzyStatusHelper()
{
    emit dispatcher->reqHideBuzyStatus();
}

Dispatcher::Dispatcher(QObject *parent) : QObject(parent)
{
    this->moveToThread(&dispatcherThd);
    dispatcherThd.start();
}

Dispatcher::~Dispatcher()
{
    dispatcherThd.quit();
    dispatcherThd.wait();
}

void Dispatcher::setRunningParameter(const QVariantMap &param)
{
    runningParameter = param;
}

void Dispatcher::registerWorker(Worker *worker)
{
    if (workers.contains(worker->workerName()))
    {
        if (worker == workers[worker->workerName()])
        {
            return;
        }
        else
        {
            qFatal("Detect the same worker name: %s", worker->workerName().toUtf8().data());
        }
    }
    else
    {
        connect(worker, &Worker::sendEventToSM, this, &Dispatcher::sendEventToSM, Qt::DirectConnection);
        connect(worker, &Worker::workerEventHandled, this, &Dispatcher::onWorkerEventHandled, Qt::DirectConnection);
        connect(worker, &Worker::postEvent, this, &Dispatcher::onWorkerPostEvent, Qt::DirectConnection);
        connect(worker, &Worker::sendEvent, this, &Dispatcher::onWorkerSendEvent, Qt::DirectConnection);
        connect(worker, &Worker::homeDone, this, &Dispatcher::onWorkerHomeDone, Qt::DirectConnection);
        workers[worker->workerName()] = worker;
    }
}

void Dispatcher::onInit()
{
    try
    {
        initDevice(false);
        home(true, false);
        emit inited(true);
    }
    catch (SilicoolException &se)
    {
        qCritical(smCate()) << se.what();
        emit inited(false);
    }
}

void Dispatcher::onPause()
{
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Pause));
    handlePause();
    emit paused();
}

void Dispatcher::onContinue()
{
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Continue));
    handleContinue();
}

void Dispatcher::onStart()
{
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Start, runningParameter));
    handleStart();
}

void Dispatcher::onStop()
{
    setExpectedHandledEvent(SMD::Stop);
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Stop));
    checkAllWorkerHandledEvent();
}

void Dispatcher::onPurge()
{
    setExpectedHandledEvent(SMD::Purge);
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Purge));
    checkAllWorkerHandledEvent();
}

void Dispatcher::onAbort()
{
    setExpectedHandledEvent(SMD::Abort);
    pushEventToAllWorker(WorkerEvent(SMD::Abort));
    handleAbort();
    checkAllWorkerHandledEvent();
}

void Dispatcher::onReset()
{
    makeAllWorkerProcessEvent(WorkerEvent(SMD::Reset));
    handleReset();
    emit reseted();
}

void Dispatcher::initDevice(bool showTip)
{
    {
        ShowBuzyStatusHelper t(this, tr("Initializing..."));
        handleInit();
        makeAllWorkerProcessEvent(WorkerEvent(SMD::Init));
        emit reqPublishCurrentState();
    }
    if (showTip)
    {
        UIOperation::getIns()->showTip(tr("Init device successful!"));
    }
}

void Dispatcher::home(bool moveToReadyPosAfterHome, bool showTip)
{
    ShowBuzyStatusHelper t(this, tr("Homing..."));

    preHome();
    QStringList workerNames = workers.keys();
    foreach (auto worker, workerNames)
    {
        workersHomeDone[worker] = false;
        workersHomeResult[worker] = false;
    }
    WorkerEvent homeEvent("__Home", moveToReadyPosAfterHome, INT_MAX - 1);
    homeEvent.isFrameworkEvent = true;
    pushEventToAllWorker(homeEvent);

    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (timer.elapsed() > 15000)
        {
            QStringList notHomeDoneWorkers;
            foreach (auto worker, workerNames)
            {
                if (!workersHomeDone[worker])
                {
                    notHomeDoneWorkers.append(worker);
                }
            }
            qCInfo(smCate()) << "Waiting workers home done..." << combineString(",", notHomeDoneWorkers);
            timer.restart();
        }
        bool allWorkerHomeDone = true;
        foreach (auto worker, workerNames)
        {
            allWorkerHomeDone &= workersHomeDone[worker];
        }
        if (allWorkerHomeDone)
        {
            break;
        }
        QThread::msleep(10);
    }

    bool allWorkerHomeResult = true;
    QStringList homeFailedWorkers;
    foreach (auto worker, workerNames)
    {
        allWorkerHomeResult &= workersHomeResult[worker];
        if (!workersHomeResult[worker])
        {
            homeFailedWorkers.append(worker);
        }
    }
    if (!allWorkerHomeResult)
    {
        throw SilicolAbort(tr("Not all workers home successful!") + QString("\r\n(%1)").arg(combineString(",", homeFailedWorkers)));
    }

    postHome();
    if (showTip)
    {
        UIOperation::getIns()->showTip(tr("Home all motor successful!"));
    }
}

QStringList Dispatcher::getWorkerNames() const
{
    return workers.keys();
}

QVariantList Dispatcher::getWorkerRunStatus() const
{
    QVariantList qv;
    auto workerNames = workers.keys();
    foreach (auto workerName, workerNames)
    {
        qv << workers[workerName]->isRunning();
    }
    return qv;
}

void Dispatcher::pushEventToAllWorker(const WorkerEvent &event)
{
    foreach (auto worker, workers.values())
    {
        worker->pushEvent(event);
    }
}

void Dispatcher::makeAllWorkerProcessEvent(const WorkerEvent &event)
{
    foreach (auto worker, workers.values())
    {
        worker->processEvent(event);
    }
}

void Dispatcher::setExpectedHandledEvent(SMD::Event event)
{
    expectedHandledEvent = event;
    foreach (auto key, workers.keys())
    {
        workersEventHandled[key] = false;
    }
}

void Dispatcher::checkAllWorkerHandledEvent()
{
    bool allWorkersHandledEvent = true;
    QStringList workers = workersEventHandled.keys();
    QStringList remaindWorkers;
    foreach (auto worker, workers)
    {
        allWorkersHandledEvent &= workersEventHandled[worker];
        if (!workersEventHandled[worker])
        {
            remaindWorkers.append(worker);
        }
    }
    if (!remaindWorkers.isEmpty())
    {
        qCDebug(smCate()) << tr("以下模块还未处理完该事件:") << combineString(",", remaindWorkers);
    }
    if (allWorkersHandledEvent)
    {
        switch (expectedHandledEvent)
        {
            case SMD::Stop:
            {
                WorkerEvent allWorkerStoppedEvent("__AllWorkerStopped", QVariant(), INT_MAX - 1);
                allWorkerStoppedEvent.isFrameworkEvent = true;
                makeAllWorkerProcessEvent(allWorkerStoppedEvent);
                emit stopped();
                break;
            }
            case SMD::Purge:
            {
                emit purged();
                break;
            }
            case SMD::Abort:
            {
                emit aborted();
                break;
            }
            default:
            {
                qCCritical(smCate()) << tr("Unknown expected handled event:") << expectedHandledEvent;
            }
        }
    }
}

void Dispatcher::onWorkerEventHandled(QString workerName, SMD::Event event)
{
    if (!workersEventHandled.contains(workerName))
    {
        qCCritical(smCate()) << tr("Unknown worker name:") << workerName;
        return;
    }

    qCDebug(smCate()) << workerName << "handled event:" << SMD::EventEnumInfo().enumToName(event);
    if (event == expectedHandledEvent)
    {
        workersEventHandled[workerName] = true;
        checkAllWorkerHandledEvent();
    }
}

void Dispatcher::onWorkerPostEvent(QString targetWorkerName, const WorkerEvent &event)
{
    if (!workers.contains(targetWorkerName))
    {
        qCCritical(smCate()) << tr("Unknown worker name:") << targetWorkerName;
        return;
    }
    workers[targetWorkerName]->pushEvent(event);
}

void Dispatcher::onWorkerSendEvent(QString targetWorkerName, const WorkerEvent &event)
{
    if (!workers.contains(targetWorkerName))
    {
        qCCritical(smCate()) << tr("Unknown worker name:") << targetWorkerName;
        return;
    }
    workers[targetWorkerName]->processEvent(event);
}

void Dispatcher::onWorkerHomeDone(QString workerName, bool homeResult)
{
    qCDebug(smCate()) << workerName << "home done:" << homeResult;

    workersHomeResult[workerName] = homeResult;
    workersHomeDone[workerName] = true;
}
