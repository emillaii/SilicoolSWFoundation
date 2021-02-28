#include "configshareclient.h"

ConfigShareClient::ConfigShareClient()
{
    connect(this, &ConfigShareClient::synchronizeAllConfigSignal, this, &ConfigShareClient::synchronizeAllConfig, Qt::QueuedConnection);
}

void ConfigShareClient::init(const QString &serverAddress)
{
    if (isInit)
    {
        return;
    }
    node = new QRemoteObjectNode;
    node->connectToNode(QUrl(serverAddress));
    configShareReplica = node->acquire<ConfigShareReplica>();
    connect(configShareReplica, &ConfigShareReplica::ConfigChanged, this, &ConfigShareClient::onConfigChanged);
    connect(configShareReplica, &ConfigShareReplica::stateChanged, this, &ConfigShareClient::onConnectionStateChanged);
    connect(this, &QObject::destroyed, configShareReplica, &QObject::deleteLater);
    connect(this, &QObject::destroyed, node, &QObject::deleteLater);
    isInit = true;
}

void ConfigShareClient::requestSynchronizeAllConfig()
{
    if (!isInit)
    {
        qFatal("You should init ConfigShareClient first!");
    }
    m_isSyncAllConfigRequestExecuteDone = false;
    m_syncAllConfigRequestExecuteResult = false;
    emit synchronizeAllConfigSignal();
}

bool ConfigShareClient::isSyncAllConfigRequestExecuteDone() const
{
    return m_isSyncAllConfigRequestExecuteDone;
}

bool ConfigShareClient::syncAllConfigRequestExecuteResult() const
{
    return m_syncAllConfigRequestExecuteResult;
}

void ConfigShareClient::bindToRemoteConfig(const QString &name, ConfigBase *config)
{
    if (!requestBindingConfigs.contains(name))
    {
        requestBindingConfigs[name] = QList<ConfigBase *>();
    }
    if (!requestBindingConfigs[name].contains(config))
    {
        requestBindingConfigs[name].append(config);
    }
}

bool ConfigShareClient::setConfig(const QString &name, const QByteArray &content)
{
    bool result = true;
    QList<ConfigBase *> configs = requestBindingConfigs[name];
    for (int i = 0; i < configs.count(); i++)
    {
        if (!configs[i]->fromJsonBinaryData(content, name))
        {
            result = false;
        }
    }
    return result;
}

void ConfigShareClient::onConfigChanged(QString name, QByteArray content)
{
    if (requestBindingConfigs.contains(name))
    {
        setConfig(name, content);
    }
}

void ConfigShareClient::synchronizeAllConfig()
{
    synchronizeAllConfigImpl();
    qInfo("Synchronize all config done, result: %d.", m_syncAllConfigRequestExecuteResult);
    m_isSyncAllConfigRequestExecuteDone = true;
}

void ConfigShareClient::onConnectionStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)
    if (newState == QRemoteObjectReplica::Valid)
    {
        qInfo("Connection state to server turn to Valid, now synchronize all config automaticlly.");
        requestSynchronizeAllConfig();
    }
}

void ConfigShareClient::synchronizeAllConfigImpl()
{
    if (!configShareReplica->isReplicaValid())
    {
        qCritical("ConfigShareReplica is not valid! Please check the connection.");
        return;
    }
    bool result = true;
    for (const QString &name : requestBindingConfigs.keys())
    {
        auto reply = configShareReplica->getConfig(name);
        if (!silicoolWait(
                1000, [&reply] { return reply.isFinished(); }, 20))
        {
            qCritical("ConfigShareReplica wait for finished timeout! Please check the connection");
            result = false;
            return;
        }

        QByteArray content = reply.returnValue();
        if (content.isEmpty())
        {
            qCritical() << QString("Config named %1 is empty! Server may not share this config.").arg(name);
            result = false;
            continue;
        }
        if (!setConfig(name, content))
        {
            qCritical() << QString("Set config named %1 failed! The data struct of request may not the same as "
                                   "response!")
                               .arg(name);
            result = false;
            continue;
        }
    }
    m_syncAllConfigRequestExecuteResult = result;
}
