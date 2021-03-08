#ifndef CONFIGOBJECTARRAY_H
#define CONFIGOBJECTARRAY_H

#include "commonmethod.h"
#include "configobject.h"
#include "utilities_global.h"
#include <QAbstractTableModel>

class UTILITIESSHARED_EXPORT ConfigObjectArray;

class UTILITIESSHARED_EXPORT AddConfigObjectComand : public CommandInterface
{
public:
    AddConfigObjectComand(ConfigObjectArray *target, int index, ConfigObject *instance = nullptr) : target(target), index(index), instance(instance)
    {
    }

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray *target;
    int index;
    ConfigObject *instance;
};

class RemoveConfigObjectCommand : public CommandInterface
{
public:
    RemoveConfigObjectCommand(ConfigObjectArray *target, int index, ConfigObject *oldConfigObj)
        : target(target), index(index), oldConfigObj(oldConfigObj)
    {
    }

    // CommandInterface interface
public:
    virtual bool execute() override;
    virtual bool unExecute() override;
    virtual QString getExecutionLog() override;
    virtual QString getUnExecutionLog() override;

private:
    ConfigObjectArray *target;
    int index;
    ConfigObject *oldConfigObj;
};

class UTILITIESSHARED_EXPORT ConfigObjectArray : public ConfigBase
{
    Q_OBJECT

public:
    ///
    /// \brief ConfigObjectArray
    /// \param metaObjectOfConfigObj
    /// Make sure: 1. Class the metaObject represent derived from ConfigObject.
    /// 2. The constructor of the class the metaObject represent has been marked as Q_INVOKABLE, and has been given
    /// correct parameter.
    /// \param parent
    ///
    ConfigObjectArray(const QMetaObject *metaObjectOfConfigObj,
                      QObject *parent = nullptr,
                      QGenericArgument val0 = QGenericArgument(nullptr),
                      QGenericArgument val1 = QGenericArgument(nullptr),
                      QGenericArgument val2 = QGenericArgument(nullptr),
                      QGenericArgument val3 = QGenericArgument(nullptr),
                      QGenericArgument val4 = QGenericArgument(nullptr));

    void setUIAddRemovable(bool uiAddRemovable);

    ConfigObject *const &at(int index) const;
    ConfigObject *const &operator[](int index) const;
    ConfigObject *&operator[](int index);

    Q_INVOKABLE int count() const;

    Q_INVOKABLE bool uiAddRemovable() const;

    Q_INVOKABLE bool add(int index);

    Q_INVOKABLE bool remove(int index);

    Q_INVOKABLE void handleEvent(int index, QString event);

    Q_INVOKABLE QVariantList getEvents() const;

    Q_INVOKABLE QStringList getTitles() const;

    Q_INVOKABLE QObject *getConfig(int index) const;

    Q_INVOKABLE QString translate(QString configName) const;

    template <typename T>
    T *getConfig(int index)
    {
        T *config = qobject_cast<T *>(at(index));
        if (config == nullptr)
        {
            qFatal("Get config with type %s failed! Stored data type is %s.", T::staticMetaObject.className(), metaObjectOfConfigObj->className());
        }
        return config;
    }

    void scaleTo(int arrayLength);

    // ConfigBase interface
public:
    virtual bool read(const QJsonValue &jsonValue) override;
    virtual void write(QJsonValue &jsonValue) override;
    void uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect) override;

signals:
    void configInserted(int index, ConfigObject *value);
    void configRemoved(int index);

public:
    ///
    /// \brief executeAddConfigObject
    /// Generally, you do not need to use this API. Please use "add" instead!
    ///
    bool executeAddConfigObject(int index, ConfigObject *instance = nullptr);
    ///
    /// \brief executeRemoveConfigObject
    /// Generally, you do not need to use this API. Please use "remove" instead!
    ///
    bool executeRemoveConfigObject(int index);

private:
    friend class AddConfigObjectComand;
    friend class RemoveConfigObjectCommand;

    const QMetaObject *metaObjectOfConfigObj;
    QList<ConfigObject *> configObjects;
    ConfigObject *templateObject;

    QList<QObject *> configChangedSignalReceivers;
    QList<int> configChangedReceiverSlotIndexs;

    QGenericArgument val0;
    QGenericArgument val1;
    QGenericArgument val2;
    QGenericArgument val3;
    QGenericArgument val4;

    bool m_uiAddRemovable = true;
};

#endif    // CONFIGOBJECTARRAY_H
