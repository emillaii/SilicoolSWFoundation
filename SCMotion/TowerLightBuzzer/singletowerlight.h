#ifndef SINGLETOWERLIGHT_H
#define SINGLETOWERLIGHT_H

#include "BasicElement/scdo.h"
#include "mashineStateIndicator/buzzer.h"
#include "motionutility.h"
#include <QObject>
#include <QTimer>

class SingleTowerLightAndBuzzer : public QObject
{
    Q_OBJECT
public:
    explicit SingleTowerLightAndBuzzer(SCDO *output, BlinkConfig *blinkConfig, QObject *parent = nullptr);

    void open();
    void blinking();
    void close();

private:
    void onTimeout();

private:
    SCDO *m_output;
    BlinkConfig *m_blinkConfig;
    QTimer timer;
    bool nextBlinkingState = true;
};

#endif    // SINGLETOWERLIGHT_H
