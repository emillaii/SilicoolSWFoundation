#ifndef CONFIGOBJECT_H
#define CONFIGOBJECT_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "configbase.h"
#include "configmanager.h"
#include "configobjectlanguagemanager.h"
#include <QDebug>
#include <QDir>
#include <QJsonObject>
#include <QMetaProperty>
#include <QQuickItem>
#include <QVariant>

class UTILITIESSHARED_EXPORT ConfigObject;

class SetObjectItemCommand : public CommandInterface
{
public:
    SetObjectItemCommand(ConfigObject *target, QString &propName, QVariant &oldValue, QVariant &newValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObject *target;
    QString propName;
    QVariant oldValue;
    QVariant newValue;
};

class UTILITIESSHARED_EXPORT ConfigObject : public ConfigBase
{
    Q_OBJECT

public:
    ConfigObject(QObject *parent = nullptr);

    ///
    /// \brief setOptionalProperty This function should be called before calling
    /// init function
    /// \param configName   QT Property name
    /// \param options  The options will be the model of the corresponding ComboBox
    ///
    void setOptionalProperty(const QString &configName, const QVariantList &options);

    ///
    /// \brief setReadOnlyProperty  This function should be called before
    /// calling init function
    /// \param configName
    ///
    void setReadOnlyProperty(const QString &configName);

    ///
    /// \brief The hidden item will not be shown on UI.
    /// This function should be called before calling init function
    /// \param configName  QT Property name
    ///
    void hide(const QString &configName);

    void setSelectFileProperty(const QString &configName, bool isAbsolutePath = true);

    void setSelectFolderProperty(const QString &configName, bool isAbsolutePath = true);

    ///
    /// \brief addEvent
    /// You can add event, and overwrite 'handleEvent' function.
    /// If you added some events, UI would create the corresponding buttons.
    /// \param event
    ///
    void addEvent(const QString &event);

    Q_INVOKABLE virtual void handleEvent(QString event);

    Q_INVOKABLE QVariantList getEvents() const;

    void disableTranslate();

    ///
    /// \brief init
    /// Init must be called, otherwise the UI will not work,
    /// Calling this function in subclass`s constructor is recommended.
    /// If you have to call 'hide', make sure call it before call 'init'.
    /// If you have to call 'setReadOnlyProperty' or 'setOptionalProperty',
    /// make sure call them before the corresponding UI was loaded.
    /// If the subclass of ConfigObject has property
    /// which is pointer of ConfigObject or ConfigArray, make sure these
    /// pointers have been initialized! Otherwise the software may crashed!
    ///
    /// Warning: When you define an static instance of a class inheriting
    /// ConfigObject, you can not call this method in the constructor of that
    /// class! Otherwise software will crash. Because QT`s metaobject-system may
    /// not be initialized when constructing the static instance, i guess.
    ///
    void init();

    Q_INVOKABLE void connectNotifySignal(QString configName, QQuickItem *receiver, QString slotName);

    Q_INVOKABLE QStringList getConfigNamesToShow() const;

    Q_INVOKABLE bool isReadOnly(QString configName) const;

    Q_INVOKABLE bool isOptional(QString configName) const;

    Q_INVOKABLE bool isSelectFile(QString configName) const;

    Q_INVOKABLE bool isSelectFolder(QString configName) const;

    Q_INVOKABLE QString translate(QString configName) const;

    Q_INVOKABLE QString convertToGivenPathFormat(QString configName, QString selectedPath);

    Q_INVOKABLE ConfigElementInfo::Type getConfigType(QString configName) const;

    Q_INVOKABLE QVariantList getOptions(QString configName) const;

    ///
    /// \brief getConfig
    /// Enum type property will return the enum key name.
    ///
    Q_INVOKABLE QVariant getConfig(QString configName) const;

    Q_INVOKABLE bool setConfig(QString configName, QVariant value);

    bool getIsInit() const;

    bool read(const QJsonValue &jsonValue) override;
    void write(QJsonValue &jsonValue) override;
    void uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect) override;

private:
    bool executeSetConfig(const QString &configName, const QVariant &value);

private:
    friend class ConfigFile;
    friend class SetObjectItemCommand;

    bool isInit = false;
    bool m_isDisableTranslate = false;
    QStringList m_configNamesToShow;
    QStringList m_hiddenConfigNames;
    QStringList m_readOnlyConfigNames;
    QStringList m_enumConfigNames;
    QStringList m_baseClassNames;
    QMap<QString, bool> m_selectFileConfigNames;
    QMap<QString, bool> m_selectFolderConfigNames;
    QMap<QString, ConfigElementInfo::Type> m_configTypes;
    QMap<QString, QVariantList> m_configOptions;
    QMap<QString, QMetaProperty> m_configProperties;
    QStringList m_events;
};

#endif    // CONFIGOBJECT_H
