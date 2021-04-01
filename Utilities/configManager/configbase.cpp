#include "configbase.h"

ConfigElementInfo::Type ConfigElementInfo::getType(QVariant::Type qvType)
{
    if (qvType == QVariant::Int || qvType == QVariant::UInt || qvType == QVariant::LongLong || qvType == QVariant::ULongLong)
    {
        return Int;
    }
    else if (qvType == QVariant::Double)
    {
        return Double;
    }
    else if (qvType == QVariant::Bool)
    {
        return Bool;
    }
    else
    {
        return Other;
    }
}

ConfigBase::ConfigBase(ConfigElementInfo::Type type, QObject *parent) : QObject(parent), m_configType(type)
{
    syncConfigTimer.setInterval(50);
    syncConfigTimer.setSingleShot(true);
    reqSyncConfigSlotIndex = getMethodIndex(metaObject(), QMetaMethod::Slot, "reqSyncConfig");
    connect(&syncConfigTimer, &QTimer::timeout, this, &ConfigBase::onSyncConfig);
}

ConfigElementInfo::Type ConfigBase::configType() const
{
    return m_configType;
}

void ConfigBase::bind(ConfigBase *other, bool isTwoWay)
{
    if (QString(metaObject()->className()) != other->metaObject()->className())
    {
        qCritical() << QString("Can not bind two instances which were different type! Type1: %1, Type2: %2")
                           .arg(metaObject()->className())
                           .arg(other->metaObject()->className());
        return;
    }
    if (bindedConfigObj != nullptr)
    {
        qWarning() << "Already binded!";
        return;
    }
    other->uniquelyConnectConfigChangedSignalToSlot(this, reqSyncConfigSlotIndex, true);
    bindedConfigObj = other;
    if (isTwoWay)
    {
        other->bind(this, false);
    }
}

void ConfigBase::unBind(bool isTwoWay)
{
    if (bindedConfigObj != nullptr)
    {
        bindedConfigObj->uniquelyConnectConfigChangedSignalToSlot(this, reqSyncConfigSlotIndex, false);
        if (isTwoWay)
        {
            bindedConfigObj->unBind(false);
        }
        bindedConfigObj = nullptr;
    }
}

QByteArray ConfigBase::toJsonBinaryData(const QString &objName)
{
    QJsonValue jv;
    write(jv);
    QJsonObject jb;
    jb[objName] = jv;
    QJsonDocument jd(jb);
    return jd.toBinaryData();
}

bool ConfigBase::fromJsonBinaryData(const QByteArray &jsBinaryData, const QString &objName)
{
    QJsonDocument jd = QJsonDocument::fromBinaryData(jsBinaryData);
    QJsonObject jb = jd.object();
    if (jb.contains(objName))
    {
        return read(jb[objName]);
    }
    else
    {
        qCritical() << "Read data from json binary data failed! Object not exist:" << objName;
        return false;
    }
}

void ConfigBase::reqSyncConfig()
{
    syncConfigTimer.start();
}

void ConfigBase::onSyncConfig()
{
    if (bindedConfigObj != nullptr)
    {
        QJsonValue jv;
        bindedConfigObj->write(jv);
        read(jv);
    }
}
