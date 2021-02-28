#ifndef CONFIGARRAY_H
#define CONFIGARRAY_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "configbase.h"
#include "configmanager.h"

class UTILITIESSHARED_EXPORT ConfigArray;

class AddArrayItemCommand : public CommandInterface
{
public:
    AddArrayItemCommand(ConfigArray *target, int index);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigArray *target;
    int index;
};

class RemoveArrayItemCommand : public CommandInterface
{
public:
    RemoveArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigArray *target;
    int index;
    QVariant oldValue;
};

class SetArrayItemCommand : public CommandInterface
{
public:
    SetArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue, QVariant &newValue);

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

    ConfigArray *target;
    int index;
    QVariant oldValue;
    QVariant newValue;
};

class UTILITIESSHARED_EXPORT ConfigArray : public ConfigBase
{
    Q_OBJECT

public:
    ConfigArray(ConfigElementInfo::Type elementType, QObject *parent = nullptr);

    void setOptionalProperty(QVariantList &options);

    void setReadOnlyProperty()
    {
        if (m_isOptional)
        {
            qCritical() << "Array element can not be optional and read only at the same time!";
            return;
        }
        m_isReadOnly = true;
    }

    void setSelectFileProperty(bool isAbsolutePath = true)
    {
        m_isAbsolutePath = isAbsolutePath;
        m_isSelectFile = true;
    }

    void setSelectFolderProperty(bool isAbsolutePath = true)
    {
        m_isAbsolutePath = isAbsolutePath;
        m_isSelectFolder = true;
    }

    const QVariantList &members()
    {
        return m_members;
    }

    const QVariant &at(int i) const
    {
        return m_members[i];
    }

    const QVariant &operator[](int i) const
    {
        return m_members[i];
    }

    Q_INVOKABLE int count() const
    {
        return m_members.count();
    }

    Q_INVOKABLE bool add(int index)
    {
        if (index < 0 || index > m_members.count())
        {
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(m_members.count());
            return false;
        }
        return ConfigManager::getIns()->executeCommand(new AddArrayItemCommand(this, index));
    }

    Q_INVOKABLE bool remove(int index)
    {
        if (index < 0 || index >= m_members.count())
        {
            qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(m_members.count());
            return false;
        }
        QVariant oldValue = m_members[index];
        return ConfigManager::getIns()->executeCommand(new RemoveArrayItemCommand(this, index, oldValue));
    }

    Q_INVOKABLE bool isReadOnly() const
    {
        return m_isReadOnly;
    }

    Q_INVOKABLE bool isOptional() const
    {
        return m_isOptional;
    }

    Q_INVOKABLE bool isSelectFile() const
    {
        return m_isSelectFile;
    }

    Q_INVOKABLE bool isSelectFolder() const
    {
        return m_isSelectFolder;
    }

    Q_INVOKABLE QString convertToGivenPathFormat(QString unused, QString selectedPath);

    Q_INVOKABLE ConfigElementInfo::Type getConfigType() const
    {
        return elementType;
    }

    Q_INVOKABLE QVariantList getOptions(QString name) const
    {
        Q_UNUSED(name)
        return m_options;
    }

    Q_INVOKABLE QVariant getConfig(int index) const
    {
        if (index < 0 || index >= m_members.count())
        {
            qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(m_members.count());
            return QVariant();
        }

#ifdef ConfigUnitTest
        qDebug() << QString("Get config, index: %1 ").arg(index) << "Value: " << m_members[index];
#endif

        return m_members[index];
    }

    Q_INVOKABLE bool setConfig(int index, QVariant value);

    // ConfigBase interface
public:
    virtual void uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect) override;
    virtual bool read(const QJsonValue &jsonValue) override;
    virtual void write(QJsonValue &jsonValue) override;

signals:
    void configInserted(int index, QVariant value);
    void configRemoved(int index);
    void configChanged(int index, QVariant value);

private:
    bool canConvertToCurrentType(QVariant &value);

    bool executeSetConfig(int index, QVariant value);

    bool executeAddItem(int index);

    bool executeRemoveItem(int index);

private:
    friend class ConfigFile;
    friend class ConfigObject;
    friend class SetArrayItemCommand;
    friend class AddArrayItemCommand;
    friend class RemoveArrayItemCommand;

    ConfigElementInfo::Type elementType;
    bool m_isOptional = false;
    bool m_isReadOnly = false;
    bool m_isSelectFile = false;
    bool m_isSelectFolder = false;
    bool m_isAbsolutePath = false;
    QVariantList m_options;
    QVariantList m_members;
};

#endif    // CONFIGARRAY_H
