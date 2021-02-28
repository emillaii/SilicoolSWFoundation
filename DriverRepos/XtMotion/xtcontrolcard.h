#ifndef XTCONTROLCARD_H
#define XTCONTROLCARD_H

#include "BasicElement/controlcard.h"
#include "XT_MotionControlerExtend_Client_Lib.h"
#include "XT_MotionControler_Client_Lib.h"
#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "errorHandling/silicolerror.h"
#include "loghelper.h"
#include "loging/Logger.h"
#include "motionutility.h"
#include "processkiller.h"
#include "xtvcm.h"
#include <QtConcurrent>

class XtControlCardConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString controllerIP READ controllerIP WRITE setControllerIP NOTIFY controllerIPChanged)
    Q_PROPERTY(bool startControllerServer READ startControllerServer WRITE setStartControllerServer NOTIFY
                   startControllerServerChanged)
    Q_PROPERTY(QString controllerServerDir READ controllerServerDir WRITE setControllerServerDir NOTIFY
                   controllerServerDirChanged)
    Q_PROPERTY(QString profileName READ profileName WRITE setProfileName NOTIFY profileNameChanged)

public:
    XtControlCardConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        init();
    }

    QString controllerIP() const
    {
        return m_controllerIP;
    }

    bool startControllerServer() const
    {
        return m_startControllerServer;
    }

    QString controllerServerDir() const
    {
        return m_controllerServerDir;
    }

    QString profileName() const
    {
        return m_profileName;
    }

public slots:
    void setControllerIP(QString controllerIP)
    {
        if (m_controllerIP == controllerIP)
            return;

        m_controllerIP = controllerIP;
        emit controllerIPChanged(m_controllerIP);
    }

    void setStartControllerServer(bool startControllerServer)
    {
        if (m_startControllerServer == startControllerServer)
            return;

        m_startControllerServer = startControllerServer;
        emit startControllerServerChanged(m_startControllerServer);
    }

    void setControllerServerDir(QString controllerServerDir)
    {
        if (m_controllerServerDir == controllerServerDir)
            return;

        m_controllerServerDir = controllerServerDir;
        emit controllerServerDirChanged(m_controllerServerDir);
    }

    void setProfileName(QString profileName)
    {
        if (m_profileName == profileName)
            return;

        m_profileName = profileName;
        emit profileNameChanged(m_profileName);
    }

signals:
    void controllerIPChanged(QString controllerIP);

    void startControllerServerChanged(bool startControllerServer);

    void controllerServerDirChanged(QString controllerServerDir);

    void profileNameChanged(QString profileName);

private:
    QString m_controllerIP = "127.0.0.1";
    bool m_startControllerServer = true;
    QString m_controllerServerDir;
    QString m_profileName;
};

class XtControlCard : public ControlCard
{
    Q_OBJECT
public:
    XtControlCard();

    ~XtControlCard() override;

    // ControlCard interface
public:
    virtual void preInit() override;

    virtual void postInit() override;

    virtual void dispose() override;

private:
    void postInitImpl();

private:
    bool isInit = false;
    XtControlCardConfig *controlCardConfig;
    ConfigFile *controlCardConfigFile;
    const QString controlCardConfigFileName = "./config/motionConfig/XtControlCardConfig.json";
};

#endif    // XTCONTROLCARD_H
