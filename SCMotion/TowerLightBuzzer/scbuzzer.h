#ifndef SCBUZZER_H
#define SCBUZZER_H

#include "BasicElement/scdo.h"
#include "buzzerconfig.h"
#include "mashineStateIndicator/buzzer.h"
#include "motionutility.h"
#include "scmotion_global.h"
#include "singletowerlight.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT SCBuzzer : public Buzzer
{
    Q_OBJECT
public:
    explicit SCBuzzer(QObject *parent = nullptr) : Buzzer(parent) {}

    // Buzzer interface
public:
    void set(BuzzerState state) override;

public:
    void init();

private:
    bool isInit = false;
    SCDO *m_buzzerOutput = nullptr;
    SingleTowerLightAndBuzzer *m_buzzer;
};

#endif    // SCBUZZER_H
