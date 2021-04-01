#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include "commonmethod.h"
#include "utilities_global.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaMethod>
#include <QObject>
#include <QVariant>
#include <qtimer.h>

class UTILITIESSHARED_EXPORT ConfigElementInfo : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        ConfigArr,
        ConfigObj,
        ConfigObjArr,

        Bool,      // UI will create a Checkbox to visualize the corresponding config
        Int,       // UI will create a TextField with a IntValidator to visualize the corresponding config
        Double,    // UI will create a TextField with a DoubleValidator to visualize the corresponding config
        Other
    };
    Q_ENUM(Type)

    enum Property
    {
        Optional,       // UI will create a ComboBox to visualize the corresponding config
        ReadOnly,       // UI item`s enable will be set to false
        SelectFile,     // UI will create an additional button. A file dialog will be opened if you clicked this button
        SelectFolder    // UI will create an additional button. A folder dialog will be opened if you clicked this
                        // button
    };
    Q_ENUM(Property)

    static Type getType(QVariant::Type qvType);
};

class UTILITIESSHARED_EXPORT ConfigFile;

class UTILITIESSHARED_EXPORT ConfigBase : public QObject
{
    Q_OBJECT

public:
    ConfigBase(ConfigElementInfo::Type type, QObject *parent = nullptr);

    Q_INVOKABLE ConfigElementInfo::Type configType() const;

    void bind(ConfigBase *other, bool isTwoWay = true);
    void unBind(bool isTwoWay = true);

    QByteArray toJsonBinaryData(const QString &objName = "Obj");
    bool fromJsonBinaryData(const QByteArray &jsBinaryData, const QString &objName = "Obj");

    virtual bool read(const QJsonValue &jsonValue) = 0;
    virtual void write(QJsonValue &jsonValue) = 0;
    virtual void uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect) = 0;

private slots:
    void reqSyncConfig();
    void onSyncConfig();

private:
    friend class ConfigFile;

    ConfigElementInfo::Type m_configType;

    ConfigBase *bindedConfigObj = nullptr;
    QTimer syncConfigTimer;
    int reqSyncConfigSlotIndex = -1;
};

#endif    // CONFIGBASE_H
