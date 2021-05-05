#ifndef WORKER_H
#define WORKER_H

#include "TaskEngine/instruction.h"
#include "errorHandling/errorreporter.h"
#include "errorHandling/silicolerror.h"
#include "errorHandling/stoppederror.h"
#include "priorityqueue.h"
#include "statemashinedefinition.h"
#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include "workerevent.h"
#include <QLoggingCategory>
#include <QObject>
#include <QThread>

class UTILITIESSHARED_EXPORT Worker : public QThread
{
    Q_OBJECT

public:
    explicit Worker(QString workerName, QObject *parent = nullptr);

    Q_INVOKABLE void handleDebugEvent(QString eventName, QVariantList args = QVariantList());

signals:
    ///
    /// \brief postEvent
    /// Push event to target worker queue
    ///
    void postEvent(QString workerName, const WorkerEvent &event);

    ///
    /// \brief sendEvent
    /// Call target worker processing event in self thread
    ///
    void sendEvent(QString workerName, const WorkerEvent &event);

    void sendEventToSM(SMD::Event event);

    void workerEventHandled(QString workerName, SMD::Event event, QPrivateSignal);

    void homeDone(QString workerName, bool homeResult, QPrivateSignal);

public:
    ///
    /// \brief pushEvent
    /// Push event to queue
    ///
    void pushEvent(const WorkerEvent &event);

    ///
    /// \brief processEvent
    /// Process event in calling thread immediately
    ///
    void processEvent(const WorkerEvent &event);

    const QLoggingCategory &logCate() const
    {
        return m_logCate;
    }

    const QString workerName() const
    {
        return m_workerName;
    }

    const WorkerEvent &currentOrLastEvent() const;

    bool isInit() const
    {
        return m_isInit;
    }
    void checkIsInit();
    bool hasAbortEvent();
    bool hasAutoStopEvent();
    bool hasAbortOrAutoStopEvent();
    bool hasUnhandledEvent();

    void stopEventHandled();
    void purgeEventHandled();

    Worker *getWorker(QString workerName) const;
    ///
    /// \brief waitWorkerThreadStopped
    /// Caution dead lock! Do not call this method in method which run on event queue.
    ///
    void waitWorkerThreadStopped(QStringList workerNames);

    static QString getWorkerConfigDir();    // DUT Related
    static void setWorkerConfigDir(QString value);
    static QString constWorkerConfigDir();    // Non DUT related

    virtual void handleWorkerConfigDirChanged() {}

protected:
    void clearEvent();

protected:
    virtual void run() override;

protected:
    virtual void initEventHandle() = 0;
    virtual void startEventHandle(const WorkerEvent &event) = 0;
    virtual void stopEventHandle();
    virtual void allWorkerStopped();
    virtual void purgeEventHandle();
    virtual void resetEventHandle() = 0;
    virtual void homeWorker(bool moveToReadyPosAfterHome) = 0;

    ///
    /// \brief customEventHandle
    /// 如果Override方法能处理该事件，就处理然后返回。否则一定要调用子类方法处理该事件
    /// \param event
    /// \return
    ///
    virtual bool customEventHandle(const WorkerEvent &event);

private:
    void startRun();
    void abortEventHandled();
    void processEvent(const WorkerEvent &event, bool fromEventQueue);

private:
    static QString workerConfigDir;
    static QMap<QString, Worker *> workers;

    SMD::State m_currentState;
    PriorityQueue<WorkerEvent> eventQueue;
    bool isRun = false;
    bool m_isInit = false;
    QString m_workerName;
    QByteArray workerNameBytes;
    QLoggingCategory m_logCate;
    bool lastEventIsAbort = false;
    bool lastEventIsAutoStop = false;
    WorkerEvent m_currentOrLastEvent;
};

#endif    // WORKER_H
