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

class SMACAxisConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(QString clipCom READ clipCom WRITE setClipCom NOTIFY clipComChanged)
    Q_PROPERTY(QString extentedCom READ extentedCom WRITE setExtentedCom NOTIFY extentedComChanged)
public:
    Q_INVOKABLE SMACAxisConfig(QObject *parent = nullptr): ConfigObject(parent)
    {
        init();
    }
    QString clipCom() const
    {
        return m_clipCom;
    }

    QString extentedCom() const
    {
        return m_extentedCom;
    }

public slots:
    void setClipCom(QString clipCom)
    {
        if (m_clipCom == clipCom)
            return;

        m_clipCom = clipCom;
        emit clipComChanged(m_clipCom);
    }

    void setExtentedCom(QString extentedCom)
    {
        if (m_extentedCom == extentedCom)
            return;

        m_extentedCom = extentedCom;
        emit extentedComChanged(m_extentedCom);
    }

signals:
    void clipComChanged(QString clipCom);

    void extentedComChanged(QString extentedCom);

private:
    QString m_clipCom;
    QString m_extentedCom;
};

#endif    // SMACAXISCONFIG_H
