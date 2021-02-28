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

class GTCoreCfgManager
{
private:
    GTCoreCfgManager()
    {
        gtCoreCfgs = new ConfigObjectArray(&GTCoreConfig::staticMetaObject);
        gtCoreCfgsFile = new ConfigFile("GTCoreConfigs", gtCoreCfgs, "./config/motionConfig/GTCoreConfigs.json");
        gtCoreCfgsFile->populate();
    }

public:
    static GTCoreCfgManager &getIns()
    {
        static GTCoreCfgManager instance;
        return instance;
    }

    ConfigObjectArray *getGtCoreCfgs() const
    {
        return gtCoreCfgs;
    }

private:
    ConfigFile *gtCoreCfgsFile;
    ConfigObjectArray *gtCoreCfgs;
};

#endif    // GTCONTROLCARDCONFIG_H
