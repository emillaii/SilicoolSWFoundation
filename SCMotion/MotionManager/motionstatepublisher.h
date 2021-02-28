#ifndef MOTIONSTATEPUBLISHER_H
#define MOTIONSTATEPUBLISHER_H

#include "motionmanager.h"
#include <QThread>

#define SUB_ELEMENT(name, elementType, definedNames, nameContainer)                                                    \
    if (!definedNames.contains(name))                                                                                  \
    {                                                                                                                  \
        qCCritical(motionCate()) << QString("Undefined %1: %2").arg(elementType).arg(name);                            \
        return;                                                                                                        \
    }                                                                                                                  \
    if (!nameContainer.contains(name))                                                                                 \
    {                                                                                                                  \
        nameContainer.append(name);                                                                                    \
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

    // QThread interface
protected:
    virtual void run() override;

private:
    bool isRun = false;

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
