#ifndef DEHONGLIGHTSOURCECONTROLLER_H
#define DEHONGLIGHTSOURCECONTROLLER_H

#include "configManager/configobject.h"
#include "lightsourcecontroller.h"
#include "serialPortWrapper/scserialport.h"
#include "singletonthread.h"
#include <QObject>

class DehongLSCConfig : public SerialPortConfig
{
    Q_OBJECT

public:
    Q_PROPERTY(QString lscName READ lscName WRITE setLscName NOTIFY lscNameChanged)

    Q_INVOKABLE DehongLSCConfig(QObject *parent = nullptr) : SerialPortConfig(parent)
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

class DehongLightSourceController : public QObject, public LightSourceController
{
    Q_OBJECT

    struct CommandStruct
    {
        uint8_t head;
        uint8_t channel;
        uint8_t data;
        uint8_t suffix;

        CommandStruct()
        {
            head = static_cast<uint8_t>('S');
            suffix = static_cast<uint8_t>('#');
        }
    };

public:
    explicit DehongLightSourceController(SerialPortConfig *config, QObject *parent = nullptr);

    // LightSourceController interface
public:
    virtual void init() override;
    virtual void setBrightness(int channel, int brightness) override;

private:
    SCSerialPort *serialPort;
};

#endif    // DehongLIGHTSOURCECONTROLLER_H
