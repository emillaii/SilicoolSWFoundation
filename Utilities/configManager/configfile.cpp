#include "configfile.h"

ConfigFile::ConfigFile(const QString &configBaseName, ConfigBase *configBase, const QString &fileName, bool encript)
    : configBaseName(configBaseName), configBase(configBase), fileName(fileName), encript(encript)
{
    saveConfigTimer.setInterval(50);
    saveConfigTimer.setSingleShot(true);
    connect(&saveConfigTimer, &QTimer::timeout, this, &ConfigFile::save);

    if (configBase == nullptr)
    {
        qFatal("configBase is null!");
        return;
    }
    m_requestSaveConfigSlotIndex = getMethodIndex(metaObject(), QMetaMethod::Slot, "requestSaveConfig");
    if (m_requestSaveConfigSlotIndex == -1)
    {
        qFatal("Did not find method 'requestSaveConfig' on class 'ConfigFile'");
    }
    configBase->uniquelyConnectConfigChangedSignalToSlot(this, m_requestSaveConfigSlotIndex, true);
}

bool ConfigFile::populate(bool createEmptyFileIfFileNotExist)
{
    setAutoSave(false);
    bool result = populateImpl(createEmptyFileIfFileNotExist);
    setAutoSave(true);
    return result;
}

void ConfigFile::setAutoSave(bool autoSave)
{
    configBase->uniquelyConnectConfigChangedSignalToSlot(this, m_requestSaveConfigSlotIndex, autoSave);
}

void ConfigFile::resetFileName(const QString &fileName)
{
    this->fileName = fileName;
}

void ConfigFile::save()
{
    qDebug() << QString("Save config file! File name: %1").arg(fileName);

    QFileInfo fileInfo(fileName);
    QDir dir;
    if (!dir.exists(fileInfo.absolutePath()))
    {
        dir.mkpath(fileInfo.absolutePath());
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qCritical() << "Open file failed: " << fileName;
        return;
    }
    QJsonObject topJsonObj;
    QJsonValue jsonValue;
    configBase->write(jsonValue);
    topJsonObj[configBaseName] = jsonValue;
    topJsonObj["Timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QJsonDocument newJsonDoc(topJsonObj);
    if (encript)
    {
        file.write(newJsonDoc.toJson().toBase64());
    }
    else
    {
        file.write(newJsonDoc.toJson());
    }
    file.close();
}

void ConfigFile::requestSaveConfig()
{
    saveConfigTimer.start();
}

bool ConfigFile::populateImpl(bool createEmptyFileIfFileNotExist)
{
    QFile file(fileName);
    if (!file.exists())
    {
        qCritical() << QString("Did not exist json file: %1").arg(fileName);
        if (createEmptyFileIfFileNotExist)
        {
            save();
            qInfo() << QString("Create an empty config file!");
        }
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical() << QString("Open json file failed: %1").arg(fileName);
        return false;
    }
    QByteArray fileContent = file.readAll();
    file.close();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray::fromBase64(fileContent));
    if (jsonDoc.isNull() || jsonDoc.isEmpty())
    {
        jsonDoc = QJsonDocument::fromJson(fileContent);
        if (jsonDoc.isNull() || jsonDoc.isEmpty())
        {
            qCritical() << QString("Json file %1 is invalid!").arg(fileName);
            return false;
        }
    }

    if (!jsonDoc.object().contains(configBaseName))
    {
        qCritical() << QString("Did not exist config base named %1 in json file %2.").arg(configBaseName).arg(fileName);
        return false;
    }
    return configBase->read(jsonDoc.object()[configBaseName]);
}
