#ifndef SMACAXISCONFIG_H
#define SMACAXISCONFIG_H
#include "configManager/configobject.h"
#include "serialPortWrapper/scserialport.h"

#include "BasicElement/axisconfig.h"
class SMACConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(SerialPortConfig *serialPortConfig READ serialPortConfig )

public:
    SMACConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_serialPortConfig = new SerialPortConfig(this);
        init();
    }

    SerialPortConfig * serialPortConfig() const
    {
        return m_serialPortConfig;
    }

private:
    SerialPortConfig * m_serialPortConfig;
};

#endif    // SMACAXISCONFIG_H
