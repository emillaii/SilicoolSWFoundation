#include "configarray.h"

SetArrayItemCommand::SetArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue, QVariant &newValue)
    : target(target), index(index), oldValue(oldValue), newValue(newValue)
{
}

bool SetArrayItemCommand::execute()
{
    return target->executeSetConfig(index, newValue);
}

bool SetArrayItemCommand::unExecute()
{
    return target->executeSetConfig(index, oldValue);
}

QString SetArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Set,Index:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(oldValue.toString())
        .arg(newValue.toString());
}

QString SetArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Set,Index:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(index)
        .arg(newValue.toString())
        .arg(oldValue.toString());
}

AddArrayItemCommand::AddArrayItemCommand(ConfigArray *target, int index) : target(target), index(index) {}

bool AddArrayItemCommand::execute()
{
    return target->executeAddItem(index);
}

bool AddArrayItemCommand::unExecute()
{
    return target->executeRemoveItem(index);
}

QString AddArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Add,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

QString AddArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3").arg(getCurrentDateTime()).arg(target->objectName()).arg(index);
}

RemoveArrayItemCommand::RemoveArrayItemCommand(ConfigArray *target, int index, QVariant &oldValue) : target(target), index(index), oldValue(oldValue)
{
}

bool RemoveArrayItemCommand::execute()
{
    return target->executeRemoveItem(index);
}

bool RemoveArrayItemCommand::unExecute()
{
    return target->executeAddItem(index) && target->executeSetConfig(index, oldValue);
}

QString RemoveArrayItemCommand::getExecutionLog()
{
    return QString("%1,%2,Remove,Index:%3,Value:%4").arg(getCurrentDateTime()).arg(target->objectName()).arg(index).arg(oldValue.toString());
}

QString RemoveArrayItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Insert,Index:%3,Value:%4").arg(getCurrentDateTime()).arg(target->objectName()).arg(index).arg(oldValue.toString());
}

ConfigArray::ConfigArray(ConfigElementInfo::Type elementType, QObject *parent)
    : ConfigBase(ConfigElementInfo::ConfigArr, parent), elementType(elementType)
{
    if (elementType == ConfigElementInfo::ConfigArr || elementType == ConfigElementInfo::ConfigObj || elementType == ConfigElementInfo::ConfigObjArr)
    {
        qFatal("The element type of ConfigArray can only be simple type!");
    }
}

void ConfigArray::setOptionalProperty(QVariantList &options)
{
    if (elementType == ConfigElementInfo::Bool)
    {
        qCritical() << "Array element of bool type can not be set to optional!";
        return;
    }
    if (m_isReadOnly)
    {
        qCritical() << "Array element can not be optional and read only at the same time!";
        return;
    }
    if (options.count() == 0)
    {
        qCritical() << "Options is empty!";
        return;
    }
    auto optionType = ConfigElementInfo::getType(options[0].type());
    if (optionType != elementType)
    {
        qCritical()
            << QString("The type of option is not the same as element type! Option type: %1, element type: %2").arg(optionType).arg(elementType);
        return;
    }
    m_isOptional = true;
    m_options = options;
}

QString ConfigArray::convertToGivenPathFormat(QString unused, QString selectedPath)
{
    Q_UNUSED(unused)
    if (!m_isAbsolutePath)
    {
        QDir d(QCoreApplication::applicationDirPath());
        return d.relativeFilePath(selectedPath);
    }
    return selectedPath;
}

bool ConfigArray::setConfig(int index, QVariant value)
{
    if (index < 0 || index >= m_members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(m_members.count());
        return false;
    }

    if (!canConvertToCurrentType(value))
    {
        return false;
    }
    QVariant oldValue = m_members[index];
    if (oldValue == value)
    {
        return true;
    }
    return ConfigManager::getIns()->executeCommand(new SetArrayItemCommand(this, index, oldValue, value));
}

bool ConfigArray::read(const QJsonValue &jsonValue)
{
    QJsonArray jsonArr = jsonValue.toArray();

    while (count() > jsonArr.count())
    {
        executeRemoveItem(count() - 1);
    }
    while (count() < jsonArr.count())
    {
        executeAddItem(count());
    }

    bool result = true;
    for (int i = 0; i < jsonArr.count(); i++)
    {
        QVariant jsonValue = jsonArr[i].toVariant();
        if (!executeSetConfig(i, jsonValue))
        {
            result = false;
        }
    }
    return result;
}

void ConfigArray::write(QJsonValue &jsonValue)
{
    QJsonArray jsonArr;

    for (int i = 0; i < m_members.count(); i++)
    {
        jsonArr.append(m_members[i].toJsonValue());
    }
    jsonValue = QJsonValue(jsonArr);
}

bool ConfigArray::canConvertToCurrentType(QVariant &value)
{
    if (elementType == ConfigElementInfo::Int)
    {
        if (value.canConvert<int>())
        {
            value = value.toInt();
            return true;
        }
        else
        {
            qCritical() << QString("Can not convert to type int. value: ") << value;
            return false;
        }
    }

    if (elementType == ConfigElementInfo::Double)
    {
        if (value.canConvert<double>())
        {
            value = value.toDouble();
            return true;
        }
        else
        {
            qCritical() << QString("Can not convert to type double. value: ") << value;
            return false;
        }
    }

    if (elementType == ConfigElementInfo::Bool)
    {
        if (value.canConvert<bool>())
        {
            value = value.toBool();
            return true;
        }
        else
        {
            qCritical() << QString("Can not convert to type bool. value: ") << value;
            return false;
        }
    }

    if (elementType == ConfigElementInfo::Other)
    {
        if (value.canConvert<QString>())
        {
            value = value.toString();
            return true;
        }
        else
        {
            qCritical() << QString("Can not convert to type string. value: ") << value;
            return false;
        }
    }

    qCritical() << "Unknow element type:" << elementType;
    return false;
}

void ConfigArray::uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect)
{
    for (int i = 0; i < metaObject()->methodCount(); i++)
    {
        auto method = metaObject()->method(i);
        if (method.methodType() == QMetaMethod::Signal)
        {
            if (QString(method.name()) == QString("configInserted") || QString(method.name()) == QString("configRemoved")
                || QString(method.name()) == QString("configChanged"))
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
}

bool ConfigArray::executeSetConfig(int index, QVariant value)
{
    if (index < 0 || index >= m_members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(m_members.count());
        return false;
    }

    if (!canConvertToCurrentType(value))
    {
        return false;
    }
    if (m_members[index] == value)
    {
        return true;
    }
    m_members.replace(index, value);
    emit configChanged(index, m_members[index]);
    return true;
}

bool ConfigArray::executeAddItem(int index)
{
    if (index < 0 || index > m_members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2]").arg(index).arg(m_members.count());
        return false;
    }

    QVariant qv;
    if (elementType == ConfigElementInfo::Int)
    {
        qv = QVariant::fromValue(0);
    }
    else if (elementType == ConfigElementInfo::Double)
    {
        qv = QVariant::fromValue(0.0);
    }
    else if (elementType == ConfigElementInfo::Bool)
    {
        qv = QVariant::fromValue(false);
    }
    else
    {
        qv = QVariant::fromValue(QString());
    }
    m_members.insert(index, qv);
    emit configInserted(index, qv);
    return true;
}

bool ConfigArray::executeRemoveItem(int index)
{
    if (index < 0 || index >= m_members.count())
    {
        qCritical() << QString("Index %1 out of range [0, %2)").arg(index).arg(m_members.count());
        return false;
    }

    m_members.removeAt(index);
    emit configRemoved(index);
    return true;
}
