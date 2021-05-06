#ifndef MASHINESTATEINDICATOR_H
#define MASHINESTATEINDICATOR_H

#include "buzzer.h"
#include "commonmethod.h"
#include "configManager/configarray.h"
#include "configManager/configobjectarray.h"
#include "enumhelper.h"
#include "systemCore/statemashinedefinition.h"
#include "towerlight.h"
#include <QObject>

class UTILITIESSHARED_EXPORT StateIndicatorConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(SMD::State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(Buzzer::BuzzerState buzzerState READ buzzerState WRITE setBuzzerState NOTIFY buzzerStateChanged)
    Q_PROPERTY(TowerLight::TLState towerLightState READ towerLightState WRITE setTowerLightState NOTIFY towerLightStateChanged)
    Q_PROPERTY(bool red READ red WRITE setRed NOTIFY redChanged)
    Q_PROPERTY(bool green READ green WRITE setGreen NOTIFY greenChanged)
    Q_PROPERTY(bool yellow READ yellow WRITE setYellow NOTIFY yellowChanged)

public:
    Q_INVOKABLE StateIndicatorConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        QVariantList tlColors = toVariantList(TowerLight::TLColorEnumInfo().names());
        if (tlColors.count() == 0)
        {
            qFatal("Did not find Enum: TowerLight::TLColor");
        }
        QList<bool> colorEnabled;
        for (int i = 0; i < tlColors.count(); i++)
        {
            colorEnabled.append(false);
        }
        setIdentityProperty("state");
        init();
    }

    SMD::State state() const
    {
        return m_state;
    }

    Buzzer::BuzzerState buzzerState() const
    {
        return m_buzzerState;
    }

    TowerLight::TLState towerLightState() const
    {
        return m_towerLightState;
    }

    bool red() const
    {
        return m_red;
    }

    bool green() const
    {
        return m_green;
    }

    bool yellow() const
    {
        return m_yellow;
    }

public slots:
    void setState(SMD::State state)
    {
        if (m_state == state)
            return;

        m_state = state;
        emit stateChanged(m_state);
    }

    void setBuzzerState(Buzzer::BuzzerState buzzerState)
    {
        if (m_buzzerState == buzzerState)
            return;

        m_buzzerState = buzzerState;
        emit buzzerStateChanged(m_buzzerState);
    }

    void setTowerLightState(TowerLight::TLState towerLightState)
    {
        if (m_towerLightState == towerLightState)
            return;

        m_towerLightState = towerLightState;
        emit towerLightStateChanged(m_towerLightState);
    }

    void setRed(bool red)
    {
        if (m_red == red)
            return;

        m_red = red;
        emit redChanged(m_red);
    }

    void setGreen(bool green)
    {
        if (m_green == green)
            return;

        m_green = green;
        emit greenChanged(m_green);
    }

    void setYellow(bool yellow)
    {
        if (m_yellow == yellow)
            return;

        m_yellow = yellow;
        emit yellowChanged(m_yellow);
    }

signals:
    void stateChanged(SMD::State state);

    void buzzerStateChanged(Buzzer::BuzzerState buzzerState);

    void towerLightStateChanged(TowerLight::TLState towerLightState);

    void redChanged(bool red);

    void greenChanged(bool green);

    void yellowChanged(bool yellow);

private:
    SMD::State m_state;
    Buzzer::BuzzerState m_buzzerState;
    TowerLight::TLState m_towerLightState;
    bool m_red = false;
    bool m_green = false;
    bool m_yellow = false;
};

class UTILITIESSHARED_EXPORT MachineStateIndicatorConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(BlinkConfig *buzzerBlinkConfig READ buzzerBlinkConfig)
    Q_PROPERTY(BlinkConfig *towerLightBlinkConfig READ towerLightBlinkConfig)
    Q_PROPERTY(ConfigObjectArray *stateIndicatorConfigs READ stateIndicatorConfigs)

public:
    MachineStateIndicatorConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_buzzerBlinkConfig = new BlinkConfig;
        m_towerLightBlinkConfig = new BlinkConfig;

        m_stateIndicatorConfigs = new ConfigObjectArray(&StateIndicatorConfig::staticMetaObject, this);
        init();
    }

    BlinkConfig *buzzerBlinkConfig() const
    {
        return m_buzzerBlinkConfig;
    }

    BlinkConfig *towerLightBlinkConfig() const
    {
        return m_towerLightBlinkConfig;
    }

    ConfigObjectArray *stateIndicatorConfigs() const
    {
        return m_stateIndicatorConfigs;
    }

private:
    BlinkConfig *m_buzzerBlinkConfig;
    BlinkConfig *m_towerLightBlinkConfig;
    ConfigObjectArray *m_stateIndicatorConfigs;
};

class UTILITIESSHARED_EXPORT MachineStateIndicator : public QObject
{
    Q_OBJECT
public:
    explicit MachineStateIndicator(Buzzer *buzzer, TowerLight *towerLight, MachineStateIndicatorConfig *config, QObject *parent = nullptr)
        : QObject(parent), buzzer(buzzer), towerLight(towerLight), config(config)
    {
        buzzer->setBlinkConfig(config->buzzerBlinkConfig());
        towerLight->setBlinkConfig(config->towerLightBlinkConfig());
        mapStateIndicator();
        connect(config->stateIndicatorConfigs(), &ConfigObjectArray::configInserted, this, &MachineStateIndicator::onStateConfigInserted);
        connect(config->stateIndicatorConfigs(), &ConfigObjectArray::configRemoved, this, &MachineStateIndicator::onStateConfigRemoved);
    }

public slots:
    void onStateChanged(SMD::State state)
    {
        if (stateIndicatorConfigs.contains(state))
        {
            auto config = stateIndicatorConfigs[state];
            buzzer->set(config->buzzerState());
            TowerLight::TLColors colors;
            if (config->red())
            {
                colors |= TowerLight::Red;
            }
            if (config->green())
            {
                colors |= TowerLight::Green;
            }
            if (config->yellow())
            {
                colors |= TowerLight::Yellow;
            }
            towerLight->set(config->towerLightState(), colors);
        }
        else
        {
            qCritical() << "Did not find StateIndicatorConfig:" << state;
        }
    }

private slots:
    void onStateConfigInserted(int index, ConfigObject *configObj)
    {
        Q_UNUSED(index)
        Q_UNUSED(configObj)
        mapStateIndicator();
    }

    void onStateConfigRemoved(int index)
    {
        Q_UNUSED(index)
        mapStateIndicator();
    }

private:
    void mapStateIndicator()
    {
        for (int i = 0; i < config->stateIndicatorConfigs()->count(); i++)
        {
            StateIndicatorConfig *stateIndocatorConfig = qobject_cast<StateIndicatorConfig *>(config->stateIndicatorConfigs()->at(i));
            stateIndicatorConfigs[stateIndocatorConfig->state()] = stateIndocatorConfig;
        }
    }

private:
    Buzzer *buzzer;
    TowerLight *towerLight;
    MachineStateIndicatorConfig *config;
    QMap<SMD::State, StateIndicatorConfig *> stateIndicatorConfigs;
};

#endif    // MASHINESTATEINDICATOR_H
