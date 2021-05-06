#ifndef GTCONTROLCARDCONFIG_H
#define GTCONTROLCARDCONFIG_H

#include "configManager/configfile.h"
#include "configManager/configobject.h"
#include "configManager/configobjectarray.h"

class GTCoreConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString setupFileName READ setupFileName WRITE setSetupFileName NOTIFY setupFileNameChanged)
    Q_PROPERTY(int axisCount READ axisCount WRITE setAxisCount NOTIFY axisCountChanged)
    Q_PROPERTY(int diCount READ diCount WRITE setDiCount NOTIFY diCountChanged)
    Q_PROPERTY(int doCount READ doCount WRITE setDoCount NOTIFY doCountChanged)

public:
    Q_INVOKABLE GTCoreConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setSelectFileProperty("setupFileName", false);
        QStringList args;
        args << "axisCount"
             << "diCount"
             << "doCount";
        setEngineerAuthorities(args);
        init();
    }

    QString setupFileName() const
    {
        return m_setupFileName;
    }

    int axisCount() const
    {
        return m_axisCount;
    }

    int diCount() const
    {
        return m_diCount;
    }

    int doCount() const
    {
        return m_doCount;
    }

public slots:
    void setSetupFileName(QString setupFileName)
    {
        if (m_setupFileName == setupFileName)
            return;

        m_setupFileName = setupFileName;
        emit setupFileNameChanged(m_setupFileName);
    }

    void setAxisCount(int axisCount)
    {
        if (m_axisCount == axisCount)
            return;

        m_axisCount = axisCount;
        emit axisCountChanged(m_axisCount);
    }

    void setDiCount(int diCount)
    {
        if (m_diCount == diCount)
            return;

        m_diCount = diCount;
        emit diCountChanged(m_diCount);
    }

    void setDoCount(int doCount)
    {
        if (m_doCount == doCount)
            return;

        m_doCount = doCount;
        emit doCountChanged(m_doCount);
    }

signals:
    void setupFileNameChanged(QString setupFileName);

    void axisCountChanged(int axisCount);

    void diCountChanged(int diCount);

    void doCountChanged(int doCount);

private:
    QString m_setupFileName;
    int m_axisCount = 8;
    int m_diCount = 16;
    int m_doCount = 16;
};

class GTCardConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int cardOpenMode READ cardOpenMode WRITE setCardOpenMode NOTIFY cardOpenModeChanged)
    Q_PROPERTY(ConfigObjectArray *gtCoreCfgs READ gtCoreCfgs)

public:
    GTCardConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_gtCoreCfgs = new ConfigObjectArray(&GTCoreConfig::staticMetaObject, this);
        setEngineerAuthority("cardOpenMode");
        m_gtCoreCfgs->setEngineerAuthority();
        init();
    }

    int cardOpenMode() const
    {
        return m_cardOpenMode;
    }

    ConfigObjectArray *gtCoreCfgs() const
    {
        return m_gtCoreCfgs;
    }

public slots:
    void setCardOpenMode(int cardOpenMode)
    {
        if (m_cardOpenMode == cardOpenMode)
            return;

        m_cardOpenMode = cardOpenMode;
        emit cardOpenModeChanged(m_cardOpenMode);
    }

signals:
    void cardOpenModeChanged(int cardOpenMode);

private:
    int m_cardOpenMode = 1;
    ConfigObjectArray *m_gtCoreCfgs;
};

class GTCardCfgManager
{
private:
    GTCardCfgManager()
    {
        gtCardCfg = new GTCardConfig();
        gtCardCfgsFile = new ConfigFile("GTCardConfig", gtCardCfg, "./config/motionConfig/GTCardConfig.json");
        gtCardCfgsFile->populate();
    }

public:
    static GTCardCfgManager &getIns()
    {
        static GTCardCfgManager instance;
        return instance;
    }

    GTCardConfig *getGtCardCfg() const
    {
        return gtCardCfg;
    }

private:
    ConfigFile *gtCardCfgsFile;
    GTCardConfig *gtCardCfg;
};

#endif    // GTCONTROLCARDCONFIG_H
