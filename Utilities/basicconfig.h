#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include "languageManager/languageconfig.h"
#include "utilities_global.h"
#include <enumhelper.h>

class UTILITIESSHARED_EXPORT BasicConfig : public ConfigObject
{
    Q_OBJECT

public:
    enum ChartTheme
    {
        ChartThemeLight = 0,
        ChartThemeBlueCerulean,
        ChartThemeDark,
        ChartThemeBrownSand,
        ChartThemeBlueNcs,
        ChartThemeHighContrast,
        ChartThemeBlueIcy,
        ChartThemeQt
    };
    Q_ENUM(ChartTheme)

    Q_PROPERTY(LanguageConfig *languageConfig READ languageConfig)
    Q_PROPERTY(QString skin READ skin WRITE setSkin NOTIFY skinChanged)
    Q_PROPERTY(QString dutType READ dutType WRITE setDutType NOTIFY dutTypeChanged)
    Q_PROPERTY(QString lastInputUser READ lastInputUser WRITE setLastInputUser NOTIFY lastInputUserChanged)
    Q_PROPERTY(ChartTheme chartTheme READ chartTheme WRITE setChartTheme NOTIFY chartThemeChanged)
    Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged)
    Q_PROPERTY(bool showTaskEngineStates READ showTaskEngineStates WRITE setShowTaskEngineStates NOTIFY showTaskEngineStatesChanged)

public:
    BasicConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        m_languageConfig = new LanguageConfig(this);
        init();
    }

    QString dutRelatedMotionConfigDir() const
    {
        return QString("./config/DutType/%1/motion").arg(m_dutType);
    }

    QString dutRelatedVisionConfigDir() const
    {
        return QString("./config/DutType/%1/vision").arg(m_dutType);
    }

    QString dutRelatedWorkersConfigDir() const
    {
        return QString("./config/DutType/%1/workers").arg(m_dutType);
    }

    LanguageConfig *languageConfig() const
    {
        return m_languageConfig;
    }

    QString dutType() const
    {
        return m_dutType;
    }

    QString skin() const
    {
        return m_skin;
    }

    QString lastInputUser() const
    {
        return m_lastInputUser;
    }

    ChartTheme chartTheme() const
    {
        return m_chartTheme;
    }

    QString macAddress() const
    {
        return m_macAddress;
    }

    bool showTaskEngineStates() const
    {
        return m_showTaskEngineStates;
    }

public slots:
    void setDutType(QString dutType)
    {
        if (m_dutType == dutType)
            return;

        m_dutType = dutType;
        emit dutTypeChanged(m_dutType);
    }

    void setSkin(QString skin)
    {
        if (m_skin == skin)
            return;

        m_skin = skin;
        emit skinChanged(m_skin);
    }

    void setLastInputUser(QString lastInputUser)
    {
        if (m_lastInputUser == lastInputUser)
            return;

        m_lastInputUser = lastInputUser;
        emit lastInputUserChanged(m_lastInputUser);
    }

    void setChartTheme(ChartTheme chartTheme)
    {
        if (m_chartTheme == chartTheme)
            return;

        m_chartTheme = chartTheme;
        emit chartThemeChanged(m_chartTheme);
    }

    void setMacAddress(QString macAddress)
    {
        if (m_macAddress == macAddress)
            return;

        m_macAddress = macAddress;
        emit macAddressChanged(m_macAddress);
    }

    void setShowTaskEngineStates(bool showTaskEngineStates)
    {
        if (m_showTaskEngineStates == showTaskEngineStates)
            return;

        m_showTaskEngineStates = showTaskEngineStates;
        emit showTaskEngineStatesChanged(m_showTaskEngineStates);
    }

signals:
    void dutTypeChanged(QString dutType);

    void skinChanged(QString skin);

    void lastInputUserChanged(QString lastInputUser);

    void chartThemeChanged(ChartTheme chartTheme);

    void macAddressChanged(QString macAddress);

    void showTaskEngineStatesChanged(bool showTaskEngineStates);

private:
    LanguageConfig *m_languageConfig;
    QString m_dutType = "DefaultDUT";
    QString m_skin;
    QString m_lastInputUser;
    ChartTheme m_chartTheme{ ChartThemeLight };
    QString m_macAddress = "";
    bool m_showTaskEngineStates = false;
};

#endif    // BASICCONFIG_H
