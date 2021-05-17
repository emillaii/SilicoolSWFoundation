#include "singletowerlight.h"
#include "commonmethod.h"
#include "qtconcurrentrun.h"

SingleTowerLightAndBuzzer::SingleTowerLightAndBuzzer(SCDO *output, BlinkConfig *blinkConfig, QObject *parent) : QObject(parent)
{
    m_output = output;
    m_blinkConfig = blinkConfig;
    timer.setSingleShot(true);
    timer.callOnTimeout(this, &SingleTowerLightAndBuzzer::onTimeout, Qt::DirectConnection);
    timer.moveToThread(&SingletonThd::timerThread());
}

void SingleTowerLightAndBuzzer::close()
{
    timer.reqStop();

    m_output->set(false);
}

void SingleTowerLightAndBuzzer::onTimeout()
{
    m_output->set(nextBlinkingState);
    nextBlinkingState = !nextBlinkingState;
    timer.reqStart(nextBlinkingState ? m_blinkConfig->closedtime() : m_blinkConfig->openedtime());
}

void SingleTowerLightAndBuzzer::open()
{
    timer.reqStop();

    m_output->set(true);
}

void SingleTowerLightAndBuzzer::blinking()
{
    if (timer.isActive())
    {
        return;
    }
    nextBlinkingState = true;
    timer.reqStart(0);
}
