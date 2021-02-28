#include "sctowerlight.h"

void SCTowerLight::init()
{
    if (!isInit)
    {
        redLightOutput = MotionElementContainer::getIns()->getItem<SCDO>(TowerLightRedDoName);
        greenLightOutput = MotionElementContainer::getIns()->getItem<SCDO>(TowerLightGreenDoName);
        yellowLightOutput = MotionElementContainer::getIns()->getItem<SCDO>(TowerLightYellowDoName);

        singleLightMap[TLColor::Red] = new SingleTowerLightAndBuzzer(redLightOutput, blinkConfig);
        singleLightMap[TLColor::Green] = new SingleTowerLightAndBuzzer(greenLightOutput, blinkConfig);
        singleLightMap[TLColor::Yellow] = new SingleTowerLightAndBuzzer(yellowLightOutput, blinkConfig);
        isInit = true;
    }
}

void SCTowerLight::open(TLColors colors)
{
    closeAll();

    if (colors.testFlag(TLColor::Red))
    {
        singleLightMap[TLColor::Red]->open();
    }
    if (colors.testFlag(TLColor::Green))
    {
        singleLightMap[TLColor::Green]->open();
    }
    if (colors.testFlag(TLColor::Yellow))
    {
        singleLightMap[TLColor::Yellow]->open();
    }
}

void SCTowerLight::blinking(TLColors colors)
{
    closeAll();

    if (colors.testFlag(TLColor::Red))
    {
        singleLightMap[TLColor::Red]->blinking();
    }
    if (colors.testFlag(TLColor::Green))
    {
        singleLightMap[TLColor::Green]->blinking();
    }
    if (colors.testFlag(TLColor::Yellow))
    {
        singleLightMap[TLColor::Yellow]->blinking();
    }
}

void SCTowerLight::closeAll()
{
    singleLightMap[TLColor::Red]->close();
    singleLightMap[TLColor::Green]->close();
    singleLightMap[TLColor::Yellow]->close();
}

void SCTowerLight::set(TLState state, TLColors colors)
{
    if (!isInit)
    {
        return;
    }
    switch (state)
    {
        case TLState::Solid:
            open(colors);
            return;
        case TLState::Blinking:
            blinking(colors);
            return;
    }
}
