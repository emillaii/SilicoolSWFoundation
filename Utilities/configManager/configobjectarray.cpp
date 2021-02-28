#include "configobjectarray.h"

bool AddConfigObjectComand::execute()
{
    return target->executeAddConfigObject(index, instance);
}

bool AddConfigObjectComand::unExecute()
{
    return target->executeRemoveConfigObject(index);
}

QString AddConfigObjectComand::getExecutionLog()
{
    return QString("%1,%2,Add,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

QString AddConfigObjectComand::getUnExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

bool RemoveConfigObjectCommand::execute()
{
    return target->executeRemoveConfigObject(index);
}

bool RemoveConfigObjectCommand::unExecute()
{
    return target->executeAddConfigObject(index, oldConfigObj);
}

QString RemoveConfigObjectCommand::getExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

QString RemoveConfigObjectCommand::getUnExecutionLog()
{
    return QString("%1,%2,Insert,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

ConfigObjectArray::ConfigObjectArray(const QMetaObject *metaObjectOfConfigObj,
                                     QObject *parent,
                                     QGenericArgument val0,
                                     QGenericArgument val1,
                                     QGenericArgument val2,
                                     QGenericArgument val3,
                                     QGenericArgument val4)
    : ConfigBase(ConfigElementInfo::ConfigObjArr, parent),
      metaObjectOfConfigObj(metaObjectOfConfigObj),
      val0(val0),
      val1(val1),
      val2(val2),
      val3(val3),
      val4(val4)
{
    templateObject = qobject_cast<ConfigObject *>(metaObjectOfConfigObj->newInstance(val0, val1, val2, val3, val4));
    if (templateObject == nullptr)
    {
        QString errorMsg("Create template object failed! Class name: %1. Please make sure: The constructor of %1 has been marked as "
                         "'Q_INVOKABLE', and has been given correct parameter. 2. Class %1 derived from ConfigObject");
        qFatal(errorMsg.arg(metaObjectOfConfigObj->className()).toUtf8());
        return;
    }
    templateObject->setParent(this);
    if (!templateObject->getIsInit())
    {
        qFatal("You should call ConfigObject::init() in the constructor of class %s", metaObjectOfConfigObj->className());
        return;
    }
}

void ConfigObjectArray::setUIAddRemovable(bool addRemovable)
{
    m_uiAddRemovable = addRemovable;
}

ConfigObject *const &ConfigObjectArray::at(int index) const
{
    if (index < 0 || index >= this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
    }
    return configObjects.at(index);
}

ConfigObject *const &ConfigObjectArray::operator[](int index) const
{
    return this->at(index);
}

ConfigObject *&ConfigObjectArray::operator[](int index)
{
    if (index < 0 || index >= this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
    }
    return configObjects[index];
}

int ConfigObjectArray::count() const
{
    return configObjects.count();
}

bool ConfigObjectArray::uiAddRemovable() const
{
    return m_uiAddRemovable;
}

bool ConfigObjectArray::add(int index)
{
    if (index < 0 || index > this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        return false;
    }
    return ConfigManager::getIns()->executeCommand(new AddConfigObjectComand(this, index));
}

bool ConfigObjectArray::remove(int index)
{
    if (index < 0 || index >= this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
        return false;
    }
    ConfigObject *oldConfigObj = configObjects[index];
    return ConfigManager::getIns()->executeCommand(new RemoveConfigObjectCommand(this, index, oldConfigObj));
}

void ConfigObjectArray::handleEvent(int index, QString event)
{
    if (index < 0 || index >= this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
        return;
    }

    at(index)->handleEvent(event);
}

QVariantList ConfigObjectArray::getEvents() const
{
    return templateObject->getEvents();
}

QStringList ConfigObjectArray::getTitles() const
{
    return templateObject->getConfigNamesToShow();
}

QObject *ConfigObjectArray::getConfig(int index) const
{
    return this->at(index);
}

QString ConfigObjectArray::translate(QString configName) const
{
    return templateObject->translate(configName);
}

void ConfigObjectArray::scaleTo(int arrayLength)
{
    while (count() > arrayLength)
    {
        remove(count() - 1);
    }
    while (count() < arrayLength)
    {
        add(count());
    }
}

bool ConfigObjectArray::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();
    bool result = true;

    while (count() > jsonArr.count())
    {
        auto configObj = at(count() - 1);
        executeRemoveConfigObject(count() - 1);
        configObj->setParent(nullptr);
        configObj->deleteLater();
    }
    while (count() < jsonArr.count())
    {
        executeAddConfigObject(count());
    }

    for (int i = 0; i < jsonArr.count(); i++)
    {
        if (!configObjects[i]->read(jsonArr[i].toObject()))
        {
            result = false;
        }
    }
    return result;
}

void ConfigObjectArray::write(QJsonValue &jsonValue)
{
    QJsonArray jsonArr;

    for (int i = 0; i < count(); i++)
    {
        QJsonValue jsonObj;
        configObjects[i]->write(jsonObj);
        jsonArr.append(jsonObj);
    }
    jsonValue = QJsonValue(jsonArr);
}

void ConfigObjectArray::uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect)
{
    if (connect)
    {
        if (!configChangedSignalReceivers.contains(receiver))
        {
            configChangedSignalReceivers.append(receiver);
            configChangedReceiverSlotIndexs.append(slotIndex);
        }
    }
    else
    {
        if (configChangedSignalReceivers.contains(receiver))
        {
            int index = configChangedSignalReceivers.indexOf(receiver);
            configChangedSignalReceivers.removeAt(index);
            configChangedReceiverSlotIndexs.removeAt(index);
        }
    }

    for (int i = 0; i < metaObject()->methodCount(); i++)
    {
        auto method = metaObject()->method(i);
        if (method.methodType() == QMetaMethod::Signal)
        {
            if (QString(method.name()) == QString("configInserted") || QString(method.name()) == QString("configRemoved"))
            {
                if (connect)
                {
                    QMetaObject::connect(this, method.methodIndex(), receiver, slotIndex, Qt::AutoConnection | Qt::UniqueConnection);
                }
                else
                {
                    QMetaObject::disconnect(this, method.methodIndex(), receiver, slotIndex);
                }
            }
        }
    }
    for (int i = 0; i < configObjects.count(); i++)
    {
        configObjects[i]->uniquelyConnectConfigChangedSignalToSlot(receiver, slotIndex, connect);
    }
}

bool ConfigObjectArray::executeAddConfigObject(int index, ConfigObject *instance)
{
    if (index < 0 || index > this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(this->count());
        return false;
    }

    if (instance == nullptr)
    {
        instance = qobject_cast<ConfigObject *>(metaObjectOfConfigObj->newInstance(val0, val1, val2, val3, val4));
        if (instance == nullptr)
        {
            QString errorMsg("Create object failed! Class name: %1. Please make sure: The constructor of %1 has been "
                             "marked as "
                             "'Q_INVOKABLE', and has been given correct parameter. 2. Class %1 derived from ConfigObject");
            qCritical() << errorMsg.arg(metaObjectOfConfigObj->className());
            return false;
        }
    }
    if (instance->parent() == nullptr)
    {
        instance->setParent(this);
    }
    for (int i = 0; i < configChangedSignalReceivers.count(); i++)
    {
        instance->uniquelyConnectConfigChangedSignalToSlot(configChangedSignalReceivers[i], configChangedReceiverSlotIndexs[i], true);
    }
    configObjects.insert(index, instance);
    emit configInserted(index, instance);
    return true;
}

bool ConfigObjectArray::executeRemoveConfigObject(int index)
{
    if (index < 0 || index >= this->count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(this->count());
        return false;
    }

    ConfigObject *configObj = this->at(index);
    configObjects.removeAt(index);
    for (int i = 0; i < configChangedSignalReceivers.count(); i++)
    {
        configObj->uniquelyConnectConfigChangedSignalToSlot(configChangedSignalReceivers[i], configChangedReceiverSlotIndexs[i], false);
    }
    emit configRemoved(index);
    return true;
}
