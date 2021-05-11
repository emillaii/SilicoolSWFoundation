#ifndef TRANSFER_H
#define TRANSFER_H

#include "BasicElement/scdi.h"
#include "MotionManager/distatemonitor.h"
#include "errorHandling/scassert.h"
#include "scmotion_global.h"
#include "transferconfig.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT Transfer : public QObject
{
    Q_OBJECT

public:
    explicit Transfer(QString name, LoadConfig *loadConfig = nullptr, UnloadConfig *unloadConfig = nullptr, QObject *parent = nullptr);
    void init(SCAxis *belt, SCDI *sensor);

    void disableUnload();
    void enableUnload();

    void loadTray();
    bool moveToDetectHasTray(int detectTrayMaxTime);
    void unloadTray();
    void returnbackTray(SCDI *entranceSNR);

private slots:
    void onDiStateChanged(QString diName, bool state);

private:
    bool trayUnloaded();

private:
    QString name;
    LoadConfig *loadConfig = nullptr;
    UnloadConfig *unloadConfig = nullptr;
    SCAxis *belt = nullptr;
    SCDI *sensor = nullptr;

    bool isDisableUnload = false;
    bool isConnSig = false;

    bool sensorLostSignal = false;
    qint64 sensorLostSignalTime = 0;
};

#endif    // TRANSFER_H
