#include "scbuzzer.h"
#include "commonmethod.h"
#include "qtconcurrentrun.h"

void SCBuzzer::init()
{
    if (!isInit)
    {
        m_buzzerOutput = MotionElementContainer::getIns()->getItem<SCDO>(BuzzerDoName);
        m_buzzer = new SingleTowerLightAndBuzzer(m_buzzerOutput, blinkConfig);
        isInit = true;
    }
}

void SCBuzzer::set(BuzzerState state)
{
    if (!isInit)
    {
        return;
    }
    switch (state)
    {
        case BuzzerState::Closed:
            m_buzzer->close();
            return;
        case BuzzerState::Opened:
            m_buzzer->open();
            return;
        case BuzzerState::Blinking:
            m_buzzer->blinking();
            return;
    }
}
