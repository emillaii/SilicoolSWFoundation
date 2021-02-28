#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "errorHandling/silicolerror.h"
#include "loging/Logger.h"
#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include "worker.h"
#include <QObject>
#include <QThread>

SILICOOL_DECLARE_LOGGING_CATEGORY(smCate, )

class Dispatcher;

class ShowBuzyStatusHelper
{
public:
    ShowBuzyStatusHelper(Dispatcher *dispatcher, QString buzyContent);
    ~ShowBuzyStatusHelper();

private:
    Dispatcher *dispatcher;
};

class UTILITIESSHARED_EXPORT Dispatcher : public QObject
{
    Q_OBJECT
public:
    Dispatcher(QObject *parent = nullptr);

    virtual ~Dispatcher() override;

    void setRunningParameter(const QVariantMap &param);

    void registerWorker(Worker *worker);

public slots:
    void onInit();

    void onPause();

    void onContinue();

    void onStart();

    void onStop();

    void onPurge();

    void onAbort();

    void onReset();

    void initDevice(bool showTip);

    void home(bool moveToReadyPosAfterHome, bool showTip);

    QStringList getWorkerNames() const;
    QVariantList getWorkerRunStatus() const;

private:
    void pushEventToAllWorker(const WorkerEvent &event);

    void makeAllWorkerProcessEvent(const WorkerEvent &event);

    void setExpectedHandledEvent(SMD::Event event);

    void checkAllWorkerHandledEvent();

private slots:
    void onWorkerEventHandled(QString workerName, SMD::Event event);
    void onWorkerPostEvent(QString targetWorkerName, const WorkerEvent &event);
    void onWorkerSendEvent(QString targetWorkerName, const WorkerEvent &event);
    void onWorkerHomeDone(QString workerName, bool homeResult);

protected:
    ///
    /// \brief initImpl
    /// If any error occur, please throw SilicoolAbort
    ///
    virtual void handleInit() = 0;
    virtual void handleStart() = 0;
    virtual void handlePause() = 0;
    virtual void handleContinue() = 0;
    virtual void handleAbort() = 0;
    virtual void handleReset() = 0;
    virtual void preHome() = 0;
    virtual void postHome() = 0;

signals:
    void inited(bool result);
    void paused();
    void purged();
    void stopped();
    void aborted();
    void reseted();
    void reqPublishCurrentState();
    void sendEventToSM(SMD::Event event);
    void reqShowBuzyStatus(QString buzyContent);
    void reqHideBuzyStatus();

private:
    QThread dispatcherThd;
    QVariantMap runningParameter;
    QMap<QString, Worker *> workers;
    QMap<QString, bool> workersEventHandled;
    QMap<QString, bool> workersHomeDone;
    QMap<QString, bool> workersHomeResult;
    SMD::Event expectedHandledEvent{};
};

#endif    // DISPATCHER_H
