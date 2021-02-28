#include "singletowerlight.h"
#include "commonmethod.h"
#include "qtconcurrentrun.h"

SingleTowerLightAndBuzzer::SingleTowerLightAndBuzzer(SCDO *output, BlinkConfig *blinkConfig, QObject *parent)
    : QObject(parent)
{
    m_output = output;
    m_blinkConfig = blinkConfig;
    timer.setSingleShot(true);
    timer.callOnTimeout(this, &SingleTowerLightAndBuzzer::onTimeout);
}

void SingleTowerLightAndBuzzer::close()
{
    timer.stop();

    m_output->set(false);
}

void SingleTowerLightAndBuzzer::onTimeout()
{
    m_output->set(nextBlinkingState);
    nextBlinkingState = !nextBlinkingState;
    timer.start(nextBlinkingState ? m_blinkConfig->closedtime() : m_blinkConfig->openedtime());
}

void SingleTowerLightAndBuzzer::open()
{
    timer.stop();

    m_output->set(true);
}

void SingleTowerLightAndBuzzer::blinking()
{
    if(timer.isActive())
    {
        return;
    }
    nextBlinkingState = true;
    timer.start(0);
}
