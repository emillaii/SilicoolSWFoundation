#include "configobject.h"

SetObjectItemCommand::SetObjectItemCommand(ConfigObject *target, QString &propName, QVariant &oldValue, QVariant &newValue)
    : target(target), propName(propName), oldValue(oldValue), newValue(newValue)
{
}

bool SetObjectItemCommand::execute()
{
    return target->executeSetConfig(propName, newValue);
}

bool SetObjectItemCommand::unExecute()
{
    return target->executeSetConfig(propName, oldValue);
}

QString SetObjectItemCommand::getExecutionLog()
{
    return QString("%1,%2,Set,ConfigName:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(propName)
        .arg(oldValue.toString())
        .arg(newValue.toString());
}

QString SetObjectItemCommand::getUnExecutionLog()
{
    return QString("%1,%2,Set,ConfigName:%3,OldValue:%4,NewValue:%5")
        .arg(getCurrentDateTime())
        .arg(target->objectName())
        .arg(propName)
        .arg(newValue.toString())
        .arg(oldValue.toString());
}

ConfigObject::ConfigObject(QObject *parent) : ConfigBase(ConfigElementInfo::ConfigObj, parent) {}

void ConfigObject::setOptionalProperty(const QString &configName, const QVariantList &options)
{
    m_configOptions[configName] = options;
}

void ConfigObject::setReadOnlyProperty(const QString &configName)
{
    m_readOnlyConfigNames.append(configName);
}

void ConfigObject::hide(const QString &configName)
{
    m_hiddenConfigNames.append(configName);
}

void ConfigObject::setSelectFileProperty(const QString &configName, bool isAbsolutePath)
{
    if (!m_selectFileConfigNames.contains(configName))
    {
        m_selectFileConfigNames[configName] = isAbsolutePath;
    }
}

void ConfigObject::setSelectFolderProperty(const QString &configName, bool isAbsolutePath)
{
    if (!m_selectFolderConfigNames.contains(configName))
    {
        m_selectFolderConfigNames[configName] = isAbsolutePath;
    }
}

void ConfigObject::addEvent(const QString &event)
{
    if (!m_events.contains(event))
    {
        m_events.append(event);
    }
}

void ConfigObject::handleEvent(QString event)
{
    qDebug() << tr("Unhandled event:") << event;
}

QVariantList ConfigObject::getEvents() const
{
    return toVariantList(m_events);
}

void ConfigObject::disableTranslate()
{
    m_isDisableTranslate = true;
}

void ConfigObject::init()
{
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop(metaObject()->property(i));
        QString propName(QString(prop.name()));
        m_configProperties[propName] = prop;

        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            auto configBasePropValue = propValue.value<ConfigBase *>();
            if (configBasePropValue == nullptr)
            {
                qFatal("%s::%s is null!", metaObject()->className(), prop.name());
            }
            m_configTypes[propName] = configBasePropValue->configType();
        }
        else if (prop.isEnumType())
        {
            m_configTypes[propName] = ConfigElementInfo::Other;
            if (!m_enumConfigNames.contains(propName))
            {
                m_enumConfigNames.append(propName);
            }
            if (!m_hiddenConfigNames.contains(propName) && !m_readOnlyConfigNames.contains(propName))
            {
                QVariantList keys;
                for (int t = 0; t < prop.enumerator().keyCount(); t++)
                {
                    keys.append(prop.enumerator().key(t));
                }
                setOptionalProperty(propName, keys);
            }
        }
        else
        {
            m_configTypes[propName] = ConfigElementInfo::getType(prop.type());
        }
        if (propName != QString("objectName") && !m_hiddenConfigNames.contains(propName))
        {
            if (!m_configNamesToShow.contains(propName))
            {
                m_configNamesToShow.append(propName);
            }
        }
    }

    foreach (auto readOnlyConfig, m_readOnlyConfigNames)
    {
        if (m_configOptions.contains(readOnlyConfig))
        {
            qCritical() << QString("Config can not be ReadOnly and Optional at the same time! Config name: %1").arg(readOnlyConfig);
        }
    }
    foreach (auto configName, m_configNamesToShow)
    {
        if (m_configTypes[configName] == ConfigElementInfo::Bool && m_configOptions.contains(configName))
        {
            qCritical() << QString("Bool config can not be optional! Config name: %1").arg(configName);
        }
    }

    const QMetaObject *superClass = metaObject()->superClass();
    QString configObjClassName = ConfigObject::staticMetaObject.className();
    while (configObjClassName != superClass->className())
    {
        if (!m_baseClassNames.contains(superClass->className()))
        {
            m_baseClassNames.append(superClass->className());
        }
        superClass = superClass->superClass();
    }
    if (!m_isDisableTranslate)
    {
        ConfigObjectLanguageManager::getIns().solve(metaObject()->className(), m_baseClassNames, m_configNamesToShow);
    }

    isInit = true;
}

void ConfigObject::connectNotifySignal(QString configName, QQuickItem *receiver, QString slotName)
{
    if (!m_configProperties.contains(configName))
    {
        qCritical() << tr("Undefined configName! %1::%2").arg(metaObject()->className()).arg(configName);
        return;
    }
    auto prop = m_configProperties[configName];
    if (!prop.hasNotifySignal())
    {
        qCritical() << "Did not find notify signal! Config name: " << configName;
        return;
    }
    int slotIndex = getMethodIndex(receiver->metaObject(), QMetaMethod::Slot, slotName);
    if (slotIndex == -1)
    {
        qCritical() << "Did not find slot! Slot name: " << slotName;
        return;
    }
    if (receiver->metaObject()->method(slotIndex).parameterCount() != 0)
    {
        qWarning() << "Slot method having 0 paremeter is recommoned! Slot name: " << slotName;
    }
    QMetaObject::connect(this, prop.notifySignalIndex(), receiver, slotIndex);
}

QStringList ConfigObject::getConfigNamesToShow() const
{
    return m_configNamesToShow;
}

bool ConfigObject::isReadOnly(QString configName) const
{
    return m_readOnlyConfigNames.contains(configName);
}

bool ConfigObject::isOptional(QString configName) const
{
    return m_configOptions.contains(configName);
}

bool ConfigObject::isSelectFile(QString configName) const
{
    return m_selectFileConfigNames.contains(configName);
}

bool ConfigObject::isSelectFolder(QString configName) const
{
    return m_selectFolderConfigNames.contains(configName);
}

QString ConfigObject::translate(QString configName) const
{
    if (m_isDisableTranslate)
    {
        return configName;
    }
    QString translatedText = ConfigObjectLanguageManager::getIns().tanslate(metaObject()->className(), configName);
    if (!translatedText.isEmpty())
    {
        return translatedText;
    }
    foreach (auto baseClassName, m_baseClassNames)
    {
        translatedText = ConfigObjectLanguageManager::getIns().tanslate(baseClassName, configName);
        if (!translatedText.isEmpty())
        {
            return translatedText;
        }
    }
    return configName;
}

QString ConfigObject::convertToGivenPathFormat(QString configName, QString selectedPath)
{
    if (m_selectFileConfigNames.contains(configName) && !m_selectFileConfigNames[configName])
    {
        QDir d(QCoreApplication::applicationDirPath());
        return d.relativeFilePath(selectedPath);
    }
    if (m_selectFolderConfigNames.contains(configName) && !m_selectFolderConfigNames[configName])
    {
        QDir d(QCoreApplication::applicationDirPath());
        return d.relativeFilePath(selectedPath);
    }
    return selectedPath;
}

ConfigElementInfo::Type ConfigObject::getConfigType(QString configName) const
{
    return m_configTypes[configName];
}

QVariantList ConfigObject::getOptions(QString configName) const
{
    return m_configOptions[configName];
}

QVariant ConfigObject::getConfig(QString configName) const
{
#ifdef ConfigUnitTest
    qDebug() << QString("Get config, name: %1 ").arg(configName) << "Value: " << QObject::property(configName.toStdString().c_str());
#endif

    QVariant value = QObject::property(configName.toStdString().c_str());
    if (!value.isValid())
    {
        qCritical() << tr("Undefined configName! %1::%2").arg(metaObject()->className()).arg(configName);
        return value;
    }
    if (m_enumConfigNames.contains(configName))
    {
        return m_configProperties[configName].enumerator().valueToKey(value.toInt());
    }
    return value;
}

bool ConfigObject::setConfig(QString configName, QVariant value)
{
    QVariant oldValue = QObject::property(configName.toUtf8());
    if (!oldValue.isValid())
    {
        qCritical() << tr("Undefined configName! %1::%2").arg(metaObject()->className()).arg(configName);
        return false;
    }
    if (oldValue == value)
    {
        return true;
    }
    return ConfigManager::getIns()->executeCommand(new SetObjectItemCommand(this, configName, oldValue, value));
}

bool ConfigObject::getIsInit() const
{
    return isInit;
}

bool ConfigObject::read(const QJsonValue &jsonValue)
{
    QJsonObject jsonObj = jsonValue.toObject();
    bool result = true;
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop = metaObject()->property(i);
        if (QString(prop.name()) == "objectName")
        {
            continue;
        }
        if (!jsonObj.contains(prop.name()))
        {
            qWarning() << QString("Read config value from json failed! Did not find key: %1").arg(prop.name());
            result = false;
            continue;
        }
        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            if (!propValue.value<ConfigBase *>()->read(jsonObj[prop.name()]))
            {
                result = false;
            }
        }
        else
        {
            if (!QObject::setProperty(prop.name(), jsonObj[prop.name()].toVariant()))
            {
                qWarning() << QString("Read config value from json failed! Config name: %1").arg(prop.name());
                result = false;
            }
        }
    }
    return result;
}

void ConfigObject::write(QJsonValue &jsonValue)
{
    QJsonObject jsonObj;
    for (int i = 0; i < metaObject()->propertyCount(); i++)
    {
        auto prop = metaObject()->property(i);
        if (QString(prop.name()) == "objectName")
        {
            continue;
        }
        auto propValue(QObject::property(prop.name()));
        if (propValue.canConvert<ConfigBase *>())
        {
            QJsonValue subJsonValue;
            propValue.value<ConfigBase *>()->write(subJsonValue);
            jsonObj[prop.name()] = subJsonValue;
        }
        else if (m_enumConfigNames.contains(prop.name()))
        {
            jsonObj[prop.name()] = m_configProperties[prop.name()].enumerator().valueToKey(propValue.toInt());
        }
        else
        {
            jsonObj[prop.name()] = propValue.toJsonValue();
        }
    }
    jsonValue = QJsonValue(jsonObj);
}

void ConfigObject::uniquelyConnectConfigChangedSignalToSlot(QObject *receiver, int slotIndex, bool connect)
{
    if (!isInit)
    {
        qCritical() << "ConfigObject is not initialized! Class:" << metaObject()->className();
        return;
    }

    foreach (auto prop, m_configProperties.values())
    {
        auto propValue = QObject::property(prop.name());
        if (propValue.canConvert<ConfigBase *>())
        {
            propValue.value<ConfigBase *>()->uniquelyConnectConfigChangedSignalToSlot(receiver, slotIndex, connect);
        }
        else
        {
            if (prop.hasNotifySignal())
            {
                if (connect)
                {
                    QMetaObject::connect(this, prop.notifySignalIndex(), receiver, slotIndex, Qt::AutoConnection | Qt::UniqueConnection);
                }
                else
                {
                    QMetaObject::disconnect(this, prop.notifySignalIndex(), receiver, slotIndex);
                }
            }
        }
    }
}

bool ConfigObject::executeSetConfig(const QString &configName, const QVariant &value)
{
    bool result = QObject::setProperty(configName.toUtf8(), value);
    if (!result)
    {
        qCritical() << QString("Set config failed! %1::%2").arg(metaObject()->className()).arg(configName);
    }
    return result;
}
