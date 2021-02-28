#ifndef CONFIGSHARECLIENT_H
#define CONFIGSHARECLIENT_H

#include "commonmethod.h"
#include "configbase.h"
#include "rep_configShare_replica.h"
#include "utilities_global.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QRemoteObjectNode>
#include <QThread>

///
/// \brief The ConfigShareClient class
/// All config changed in server side will be synchronized to all client side automaticlly.
/// All config will be synchronized automaticlly when the connection was builed the first time
/// or when the connection was rebuilded.
///
/// example:
/// class CylinderConfig: public ConfigObject
/// {
///     ...
/// }
///
/// Server side:
/// int main()
/// {
///     ...
///     auto css = &ConfigShareServer::getIns();
///     CylinderConfig cylConfig;
///     ConfigFile cylConfigFile("cylConfig", &cylConfig, "cylConfig.json");
///     cylConfig.populate();
///     css->share("cylConfig", &cylConfig);
///     QRemoteObjectHost host;
///     host.setHostUrl(QUrl("tcp://..."));
///     host.enableRemoting(&(ConfigShareServer::getIns()));
///     // Start cilent process
///     ...
/// }
///
/// Client side:
/// int main()
/// {
///     ...
///     ConfigShareClient *csc = &ConfigShareClient::getIns();
///     CylinderConfig cylConfig;
///     csc->bindToRemoteConfig("cylConfig", &cylConfig);
///     csc->init("tcp://...");
///     ...
/// }
///
class UTILITIESSHARED_EXPORT ConfigShareClient : public QObject
{
    Q_OBJECT
private:
    explicit ConfigShareClient();

public:
    static ConfigShareClient &getIns()
    {
        static ConfigShareClient csc;
        return csc;
    }

    void init(const QString &serverAddress);

    Q_INVOKABLE void requestSynchronizeAllConfig();

    bool isSyncAllConfigRequestExecuteDone() const;

    bool syncAllConfigRequestExecuteResult() const;

    ///
    /// \brief bindToRemoteConfig
    /// You can bind an local config to server config by the name.
    /// \param name
    /// \param config
    ///
    void bindToRemoteConfig(const QString &name, ConfigBase *config);

private:
    bool setConfig(const QString &name, const QByteArray &content);

signals:
    void synchronizeAllConfigSignal();

private slots:
    void onConfigChanged(QString name, QByteArray content);

    void synchronizeAllConfig();

    void onConnectionStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState);

private:
    void synchronizeAllConfigImpl();

private:
    bool m_isSyncAllConfigRequestExecuteDone = false;
    bool m_syncAllConfigRequestExecuteResult = false;
    bool isInit = false;
    ConfigShareReplica *configShareReplica = nullptr;
    QRemoteObjectNode *node = nullptr;
    QMap<QString, QList<ConfigBase *>> requestBindingConfigs;
};

#endif    // CONFIGSHARECLIENT_H
