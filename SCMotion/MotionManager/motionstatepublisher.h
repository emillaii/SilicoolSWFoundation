#ifndef MOTIONSTATEPUBLISHER_H
#define MOTIONSTATEPUBLISHER_H

#include "motionmanager.h"
#include <QMutex>
#include <QMutexLocker>
#include <QThread>

#define SUB_ELEMENT(name, elementType, definedNames, nameContainer)                                                                                  \
    QMutexLocker t(&locker);                                                                                                                         \
    if (!definedNames.contains(name))                                                                                                                \
    {                                                                                                                                                \
        qCCritical(motionCate()) << QString("Undefined %1: %2").arg(elementType).arg(name);                                                          \
        return;                                                                                                                                      \
    }                                                                                                                                                \
    if (!nameContainer.contains(name))                                                                                                               \
    {                                                                                                                                                \
        nameContainer.append(name);                                                                                                                  \
        tryStart();                                                                                                                                  \
    }

#define UNSUB_ELEMENT(name, nameContainer)                                                                                                           \
    QMutexLocker t(&locker);                                                                                                                         \
    if (nameContainer.contains(name))                                                                                                                \
    {                                                                                                                                                \
        nameContainer.removeAll(name);                                                                                                               \
        if (subscribedVacuums.count() == 0 && subscribedCylinders.count() == 0 && subscribedDis.count() == 0 && subscribedDos.count() == 0)          \
        {                                                                                                                                            \
            isRun = false;                                                                                                                           \
        }                                                                                                                                            \
    }

class MotionStatePublisher : public QThread
{
    Q_OBJECT
public:
    explicit MotionStatePublisher(const QList<QString> &definedCylinders,
                                  const QList<QString> &definedVacuums,
                                  const QList<QString> &definedDis,
                                  const QList<QString> &definedDos,
                                  QObject *parent = nullptr);

    ~MotionStatePublisher() override;

signals:
    void cylStatesPublished(QVariantMap state);
    void vacuumStatePublished(QVariantMap state);
    void diStatePublished(QVariantMap state);
    void doStatePublished(QVariantMap state);

public slots:
    void subscribeCylinderState(QString cylName);
    void subscribeVacuumState(QString vacuumName);
    void subscribeDIState(QString diName);
    void subscribeDOState(QString doName);

    void unSubscribeCylinderState(QString cylName);
    void unSubscribeVacuumState(QString vacuumName);
    void unSubscribeDIState(QString diName);
    void unSubscribeDOState(QString doName);

    void dispose();

    // QThread interface
protected:
    virtual void run() override;

private:
    void tryStart();

private:
    bool isRun = false;
    bool isMotionInited = false;

    QMutex locker;

    QList<QString> definedCylinders;
    QList<QString> definedVacuums;
    QList<QString> definedDis;
    QList<QString> definedDos;
    QList<QString> subscribedCylinders;
    QList<QString> subscribedVacuums;
    QList<QString> subscribedDis;
    QList<QString> subscribedDos;
};

#endif    // MOTIONSTATEPUBLISHER_H
