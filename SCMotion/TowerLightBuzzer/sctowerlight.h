#ifndef SCTOWERLIGHT_H
#define SCTOWERLIGHT_H

#include "../BasicElement/scdo.h"
#include "../motionutility.h"
#include "buzzerconfig.h"
#include "mashineStateIndicator/towerlight.h"
#include "scmotion_global.h"
#include "singletowerlight.h"
#include <QMap>
#include <QObject>

class SCMOTIONSHARED_EXPORT SCTowerLight : public TowerLight
{
    Q_OBJECT
public:
    explicit SCTowerLight(QObject *parent = nullptr) : TowerLight(parent) {}

    // TowerLight interface
public:
    void set(TLState state, TLColors colors) override;

public:
    void init();

private:
    void open(TLColors colors);
    void blinking(TLColors colors);
    void closeAll();

private:
    bool isInit = false;

    SCDO *redLightOutput = nullptr;
    SCDO *greenLightOutput = nullptr;
    SCDO *yellowLightOutput = nullptr;

    QMap<TLColor, SingleTowerLightAndBuzzer *> singleLightMap;
};

#endif    // SCTOWERLIGHT_H
