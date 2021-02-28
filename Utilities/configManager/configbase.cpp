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

ConfigBase::ConfigBase(ConfigElementInfo::Type type, QObject *parent) : QObject(parent), m_configType(type) {}

ConfigElementInfo::Type ConfigBase::configType() const
{
    return m_configType;
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
