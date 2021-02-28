#ifndef DISTATEMONITOR_H
#define DISTATEMONITOR_H

#include "BasicElement/scdi.h"
#include "motionelementcontainer.h"
#include "scmotion_global.h"
#include <QMap>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QThread>

class SCMOTIONSHARED_EXPORT DiStateMonitor : public QThread
{
    Q_OBJECT

private:
    explicit DiStateMonitor(QObject *parent = nullptr);

public:
    static DiStateMonitor &getIns()
    {
        static DiStateMonitor instance;
        return instance;
    }

    ~DiStateMonitor() override;

    void registerDi(QString diName);
    void unRegisterDi(QString diName);
    void startMonitor();
    void stopMonitor();

signals:
    void diStateChanged(QString diName, bool state);

    // QThread interface
protected:
    virtual void run() override;

private:
    bool isMonitor = false;
    QMap<SCDI *, bool> diStates;
    QMutex locker;
};

class SCMOTIONSHARED_EXPORT AutoUnregisterDi
{
public:
    AutoUnregisterDi(const QString& diName);
    ~AutoUnregisterDi();

private:
    QString diName;
};

#endif    // DISTATEMONITOR_H
