#ifndef MOTIONSTATEREPORTER_H
#define MOTIONSTATEREPORTER_H

#include "BasicElement/scaxis.h"
#include "CompositeElement/sccylinder.h"
#include "CompositeElement/scvacuum.h"
#include "motionelementdefinition.h"
#include <QDataStream>
#include <QMap>
#include <QObject>

class MotionStateReporter : public QThread
{
    Q_OBJECT
public:
    explicit MotionStateReporter(int reportInterval = 250, QObject *parent = nullptr);

public:
    void setElementToBeWatched(QList<SCAxis *> axes,
                               QList<SCDI *> scdis,
                               QList<SCDO *> scdos,
                               QList<SCCylinder *> cylinders,
                               QList<SCVacuum *> vacuums);

    void startReport();

    void stopReport();

signals:
    void reportMotionState(QVariant state);

    // QThread interface
protected:
    virtual void run() override;

private:
    int reportInterval;    // ms
    bool isReporting = false;

    QList<SCAxis *> axes;
    QList<SCDI *> scdis;
    QList<SCDO *> scdos;
    QList<SCCylinder *> cylinders;
    QList<SCVacuum *> vacuums;
};

#endif    // MOTIONSTATEREPORTER_H
