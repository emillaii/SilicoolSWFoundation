#ifndef IOCONFIG_H
#define IOCONFIG_H

#include "configManager/configobject.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT IOConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool reversal READ reversal WRITE setReversal NOTIFY reversalChanged)

public:
    Q_INVOKABLE IOConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("name");
        init();
    }

    virtual ~IOConfig() {}

    bool reversal() const
    {
        return m_reversal;
    }

    QString name() const
    {
        return m_name;
    }

public slots:
    void setReversal(bool reversal)
    {
        if (m_reversal == reversal)
            return;

        m_reversal = reversal;
        emit reversalChanged(m_reversal);
    }

    void setName(QString name)
    {
        if (m_name == name)
            return;

        m_name = name;
        emit nameChanged(m_name);
    }

signals:
    void reversalChanged(bool reversal);

    void nameChanged(QString name);

private:
    bool m_reversal = false;
    QString m_name = "";
};

#endif    // IOCONFIG_H
