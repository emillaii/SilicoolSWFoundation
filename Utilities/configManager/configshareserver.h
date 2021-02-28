#ifndef CONFIGSHARESERVER_H
#define CONFIGSHARESERVER_H

#include "commonmethod.h"
#include "configbase.h"
#include "rep_configShare_source.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QRemoteObjectHost>
#include <QTimer>

class ConfigChangeMonitor : public QObject
{
    Q_OBJECT

public:
    ConfigChangeMonitor(QObject *parent,
                        ConfigShareSimpleSource *configShareSource,
                        const QString &name,
                        ConfigBase *config);

public:
    QByteArray getConfigContent();

private slots:
    void onConfigChanged()
    {
        timer.start();
    }

    void onTimeout()
    {
        emit configShareSource->ConfigChanged(name, getConfigContent());
    }

private:
    ConfigShareSimpleSource *configShareSource;
    QString name;
    ConfigBase *config;
    QTimer timer;
    int onConfigChangedSlotIndex;
};

class UTILITIESSHARED_EXPORT ConfigShareServer : public ConfigShareSimpleSource
{
    Q_OBJECT

private:
    ConfigShareServer() : ConfigShareSimpleSource(nullptr) {}

public:
    virtual ~ConfigShareServer() override {}

    static ConfigShareServer &getIns()
    {
        static ConfigShareServer css;
        return css;
    }

    void init(const QString &serverAddress);

    ///
    /// \brief share
    /// You can share an config by an unique name
    /// \param name
    /// \param config
    ///
    void share(const QString &name, ConfigBase *config);

    // ConfigShareSource interface
public slots:
    virtual QByteArray getConfig(QString name) override;

private:
    QMap<QString, ConfigChangeMonitor *> configChangeMonitors;
    QRemoteObjectHost host;
};

#endif    // CONFIGSHARESERVER_H
