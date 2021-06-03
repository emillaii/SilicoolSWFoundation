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
    Q_PROPERTY(QString coordName READ coordName WRITE setCoordName NOTIFY coordNameChanged)
    Q_PROPERTY(QString coord READ coord WRITE setCoord NOTIFY coordChanged)

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

    QString coordName() const
    {
        return m_coordName;
    }

    QString coord() const
    {
        return m_coord;
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

    void setCoordName(QString coordName)
    {
        if (m_coordName == coordName)
            return;

        m_coordName = coordName;
        emit coordNameChanged(m_coordName);
    }

    void setCoord(QString coord)
    {
        if (m_coord == coord)
            return;

        m_coord = coord;
        emit coordChanged(m_coord);
    }

signals:

    void ipChanged(QString ip);

    void portChanged(int port);

    void controllerIDChanged(int controllerID);

    void coordNameChanged(QString coordName);

    void coordChanged(QString coord);

private:

    QString m_ip = "";
    int m_port = -1;
    int m_controllerID =-1;
    QString m_coordName ="UOACOORD";
    QString m_coord = "0,0,0,0,0,0";
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

    ConfigObjectArray *getPICoreCfgArry() const
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
