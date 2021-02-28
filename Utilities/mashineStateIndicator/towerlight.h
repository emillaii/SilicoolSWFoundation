#ifndef TOWERLIGHT_H
#define TOWERLIGHT_H

#include "buzzer.h"
#include <QObject>

class UTILITIESSHARED_EXPORT TowerLight : public QObject
{
    Q_OBJECT
public:
    enum TLState
    {
        Blinking,
        Solid,
    };
    Q_ENUM(TLState)

    enum TLColor
    {
        Red,
        Green,
        Yellow
    };
    Q_ENUM(TLColor)
    Q_DECLARE_FLAGS(TLColors, TLColor)

    TowerLight(QObject *parent = nullptr) : QObject(parent) {}

    virtual void set(TLState state, TLColors colors) = 0;

    void setBlinkConfig(BlinkConfig *blinkConfig)
    {
        this->blinkConfig = blinkConfig;
    }

    static const EnumHelper<TLColor> &TLColorEnumInfo()
    {
        static const EnumHelper<TLColor> enumInfo(staticMetaObject, "TLColor");
        return enumInfo;
    }

protected:
    BlinkConfig *blinkConfig = nullptr;
};

#endif    // TOWERLIGHT_H
