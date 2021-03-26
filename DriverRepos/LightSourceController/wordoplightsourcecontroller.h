#ifndef WORDOPLIGHTSOURCECONTROLLER_H
#define WORDOPLIGHTSOURCECONTROLLER_H

#include "configManager/configobject.h"
#include "lightsourcecontroller.h"
#include "serialPortWrapper/scserialport.h"
#include "singletonthread.h"
#include <QObject>

class WordopLSCConfig : public SerialPortConfig
{
    Q_OBJECT

public:
    Q_PROPERTY(QString lscName READ lscName WRITE setLscName NOTIFY lscNameChanged)

    Q_INVOKABLE WordopLSCConfig(QObject *parent = nullptr) : SerialPortConfig(parent)
    {
        setReadOnlyProperty("lscName");
        init();
    }

    QString lscName() const
    {
        return m_lscName;
    }

public slots:
    void setLscName(QString lscName)
    {
        if (m_lscName == lscName)
            return;

        m_lscName = lscName;
        emit lscNameChanged(m_lscName);
    }

signals:
    void lscNameChanged(QString lscName);

private:
    QString m_lscName;
};

class WordopLightSourceController : public QObject, public LightSourceController
{
    Q_OBJECT

    struct CommandStruct
    {
        uint8_t head;
        uint8_t len;
        uint8_t devCode;
        uint8_t devId;
        uint8_t cmd;
        uint8_t channel;
        uint8_t data;
        uint8_t checkSum;

    public:
        void calcCheckSum()
        {
            uint8_t *p = reinterpret_cast<uint8_t *>(this);
            checkSum = 0;
            for (int i = 0; i < 7; i++)
            {
                checkSum += p[i];
            }
        }
    };

public:
    explicit WordopLightSourceController(SerialPortConfig *config, QObject *parent = nullptr);
    ~WordopLightSourceController() override;

    // LightSourceController interface
public:
    virtual void init() override;
    virtual void setBrightness(int channel, int brightness) override;
    virtual void open(int channel) override;
    virtual void close(int channel) override;
    virtual bool getBrightness(int channel, int &brightness) override;
    virtual void dispose() override;

private:
    void setOnOffImpl(int channel, bool isOn);

private:
    const static uint8_t SetBrightnessCmd = 0x1a;
    const static uint8_t GetBrightnessCmd = 0x31;
    const static uint8_t SetTriggerCmd = 0x22;
    const static uint8_t SetOnOffCmd = 0x2a;
    const static uint8_t DevCode = 0x01;

    SCSerialPort *serialPort;

    int devId = 0x00;
};

#endif    // WORDOPLIGHTSOURCECONTROLLER_H
