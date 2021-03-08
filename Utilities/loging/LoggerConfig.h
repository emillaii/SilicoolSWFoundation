#ifndef LOGPARAMETER_H
#define LOGPARAMETER_H

#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT LogCategoryConfig : public ConfigObject
{
    Q_OBJECT

    int m_logLevel = 0;
    QString m_category;

public:
    Q_INVOKABLE explicit LogCategoryConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        disableTranslate();
        init();
    }

    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(int logLevel READ logLevel WRITE setLogLevel NOTIFY logLevelChanged)

    int logLevel() const
    {
        return m_logLevel;
    }

    QString category() const
    {
        return m_category;
    }

public slots:
    void setLogLevel(int logLevel)
    {
        if (m_logLevel == logLevel)
            return;

        m_logLevel = logLevel;
        emit logLevelChanged(m_logLevel);
    }

    void setCategory(QString category)
    {
        if (m_category == category)
            return;

        m_category = category;
        emit categoryChanged(m_category);
    }

signals:
    void logLevelChanged(int logLevel);
    void categoryChanged(QString category);
};

class UTILITIESSHARED_EXPORT LoggerConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(ConfigObjectArray *categoryConfigs READ categoryConfigs)
    Q_PROPERTY(QString logPublishAddr READ logPublishAddr WRITE setLogPublishAddr NOTIFY logPublishAddrChanged)
    Q_PROPERTY(QString msgSubscribeAddr READ msgSubscribeAddr WRITE setMsgSubscribeAddr NOTIFY msgSubscribeAddrChanged)

public:
    LoggerConfig()
    {
        m_categoryConfigs = new ConfigObjectArray(&LogCategoryConfig::staticMetaObject, this);
        disableTranslate();
        init();
    }

    ConfigObjectArray *categoryConfigs() const
    {
        return m_categoryConfigs;
    }

    QString logPublishAddr() const
    {
        return m_logPublishAddr;
    }

    QString msgSubscribeAddr() const
    {
        return m_msgSubscribeAddr;
    }

public slots:

    void setLogPublishAddr(QString logPublishAddr)
    {
        if (m_logPublishAddr == logPublishAddr)
            return;

        m_logPublishAddr = logPublishAddr;
        emit logPublishAddrChanged(m_logPublishAddr);
    }

    void setMsgSubscribeAddr(QString msgSubscribeAddr)
    {
        if (m_msgSubscribeAddr == msgSubscribeAddr)
            return;

        m_msgSubscribeAddr = msgSubscribeAddr;
        emit msgSubscribeAddrChanged(m_msgSubscribeAddr);
    }

signals:
    void logPublishAddrChanged(QString logPublishAddr);
    void msgSubscribeAddrChanged(QString msgSubscribeAddr);

private:
    ConfigObjectArray *m_categoryConfigs;
    QString m_logPublishAddr = "tcp://127.0.0.1:60010";
    QString m_msgSubscribeAddr = "tcp://127.0.0.1:60001";
};

#endif    // LOGPARAMETER_H
