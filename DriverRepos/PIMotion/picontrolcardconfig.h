#ifndef PICONTROLCARDCONFIG_H
#define PICONTROLCARDCONFIG_H

#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"
#include "errorHandling/silicolerror.h"

class PIControlCardConfig : public ConfigObject
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(int controllerID READ controllerID WRITE setControllerID NOTIFY controllerIDChanged)

public:
    Q_INVOKABLE PIControlCardConfig(QObject *parent = nullptr) : ConfigObject(parent)
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

    int controllerID() const
    {
        return m_controllerID;
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

    void setControllerID(int controllerID)
    {
        if (m_controllerID == controllerID)
            return;

        m_controllerID = controllerID;
        emit controllerIDChanged(m_controllerID);
    }

signals:

    void ipChanged(QString ip);

    void portChanged(int port);

    void controllerIDChanged(int controllerID);

private:

    QString m_ip = "";
    int m_port = -1;
    int m_controllerID =-1;
};

class PICardConfigManager
{
private:
    PICardConfigManager()
    {
        piCoreCfg = new ConfigObjectArray( &PIControlCardConfig::staticMetaObject);
        piCardCfgFile = new ConfigFile("PICoreConfig", piCoreCfg, "./config/motionConfig/PICoreConfig.json");
        piCardCfgFile->populate(true);
    }
public:
    static PICardConfigManager &getIns()
    {
        static PICardConfigManager instance;
        return  instance;
    }

    ConfigObjectArray *getPICoreCfgArr() const
    {
        return piCoreCfg;
    }
    PIControlCardConfig *getPICoreCfg() const
    {
        if(piCoreCfg->count() <=0)
        {
            throw SilicolAbort("PI core config is null!", EX_LOCATION);
        }

        PIControlCardConfig *coreCfg = piCoreCfg->getConfig<PIControlCardConfig>(0);
        return coreCfg;
    }
private:
    ConfigFile *piCardCfgFile;
    ConfigObjectArray *piCoreCfg;
};

#endif // PICONTROLCARDCONFIG_H
