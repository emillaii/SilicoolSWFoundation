#include "configshareserver.h"

ConfigChangeMonitor::ConfigChangeMonitor(QObject *parent, ConfigShareSimpleSource *configShareSource, const QString &name, ConfigBase *config)
    : QObject(parent), configShareSource(configShareSource), name(name), config(config)
{
    onConfigChangedSlotIndex = getMethodIndex(metaObject(), QMetaMethod::Slot, "onConfigChanged");
    if (onConfigChangedSlotIndex == -1)
    {
        qFatal("Did not find method 'onConfigChanged' on class 'ConfigChangeMonitor'!");
    }
    timer.setInterval(50);
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, this, &ConfigChangeMonitor::onTimeout);
    config->uniquelyConnectConfigChangedSignalToSlot(this, onConfigChangedSlotIndex, true);
}

QByteArray ConfigChangeMonitor::getConfigContent()
{
    return config->toJsonBinaryData(name);
}

void ConfigShareServer::init(const QString &serverAddress)
{
    host.setHostUrl(serverAddress);
    host.enableRemoting(this);
}

void ConfigShareServer::share(const QString &name, ConfigBase *config)
{
    if (configChangeMonitors.contains(name))
    {
        qFatal("Config has been shared! Name: %s", name.toUtf8().data());
    }
    ConfigChangeMonitor *configChangeMonitor = new ConfigChangeMonitor(this, this, name, config);
    configChangeMonitors[name] = configChangeMonitor;
}

QByteArray ConfigShareServer::getConfig(QString name)
{
    if (configChangeMonitors.contains(name))
    {
        return configChangeMonitors[name]->getConfigContent();
    }
    qCritical() << QString("Config named %1 has not been shared, now return empty content.").arg(name);
    return QByteArray();
}
