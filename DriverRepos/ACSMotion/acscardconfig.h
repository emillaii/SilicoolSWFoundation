#ifndef ACSCAEDCONFIG_H
#define ACSCAEDCONFIG_H

#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"
#include "errorHandling/silicolerror.h"

class ACSCardConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int handle READ handle WRITE setHandle NOTIFY handleChanged)

public:
    Q_INVOKABLE ACSCardConfig(QObject *parent = nullptr)
    {
        setSelectFileProperty("setupFileName", false);
        init();
    }

    QString ip() const
    {
        return m_ip;
    }

    int port() const
    {
        return m_port;
    }

    int handle() const
    {
        return m_handle;
    }

public slots:
    void setIp(QString ip)
    {
        if (m_ip == ip)
            return;

        m_ip = ip;
        emit ipChanged(m_ip);
    }

    void setPort(int port)
    {
        if (m_port == port)
            return;

        m_port = port;
        emit portChanged(m_port);
    }

    void setHandle(int handle)
    {
        if (m_handle == handle)
            return;

        m_handle = handle;
        emit handleChanged(m_handle);
    }

signals:
    void ipChanged(QString ip);

    void portChanged(int port);

    void handleChanged(int handle);

private:
    QString m_ip = "";
    int m_port = -1;
    int m_handle = -1;
};

class ACSCardConfigManage
{
private:
    ACSCardConfigManage()
    {
        acsCardCfg = new ConfigObjectArray(&ACSCardConfig::staticMetaObject);
        acsCardCfgFile = new ConfigFile("ACSCardConfig", acsCardCfg, "./config/motionConfig/ACSCardConfig.json");
        acsCardCfgFile->populate();
    }

public:
    static ACSCardConfigManage &getIns()
    {
        static ACSCardConfigManage instance;
        return instance;
    }

    ConfigObjectArray *getACSCardCfgArray() const
    {
        return acsCardCfg;
    }
    ACSCardConfig *getACSCardCfg() const
    {
        if(acsCardCfg->count() <= 0)
        {
            throw SilicolAbort("ACSC core config is null!", EX_LOCATION);
        }
        ACSCardConfig *acsCoreCfg = acsCardCfg->getConfig<ACSCardConfig>(0);
        return acsCoreCfg;
    }
private:
    ConfigFile *acsCardCfgFile;
    ConfigObjectArray *acsCardCfg;
};

#endif // ACSCAEDCONFIG_H
