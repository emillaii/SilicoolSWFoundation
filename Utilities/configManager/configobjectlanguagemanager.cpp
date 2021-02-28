#include "configobjectlanguagemanager.h"

void ConfigObjectLanguageManager::solve(QString className, QStringList &baseClassNames, QStringList configNames)
{
    if (languagePackges.contains(className))
    {
        return;
    }
    foreach (auto baseClassName, baseClassNames)
    {
        if (languagePackges.contains(baseClassName))
        {
            auto baseConfigNames = languagePackges[baseClassName]->keys();
            foreach (auto baseConfigName, baseConfigNames)
            {
                int index = configNames.indexOf(baseConfigName);
                if (index >= 0)
                {
                    configNames.removeAt(index);
                }
            }
        }
    }
    QMap<QString, QString> *translates = new QMap<QString, QString>;
    foreach (auto configName, configNames)
    {
        (*translates)[configName] = "";
    }
    languagePackges[className] = translates;
    QFile file(languagePackgeDir + className + ".json");
    if (!file.exists())
    {
        QJsonObject jsonObj;
        foreach (auto configName, configNames)
        {
            jsonObj[configName] = "";
        }
        QJsonDocument jsonDoc(jsonObj);
        QFileInfo fileInfo(file.fileName());
        QDir dir;
        if (!dir.exists(fileInfo.absolutePath()))
        {
            dir.mkpath(fileInfo.absolutePath());
        }
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(jsonDoc.toJson());
        file.close();
    }
    else
    {
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QByteArray fileContent = file.readAll();
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(fileContent);
        QJsonObject jo = jsonDoc.object();
        bool joChanged = false;
        foreach (auto configName, configNames)
        {
            if (jo.contains(configName))
            {
                (*translates)[configName] = jo[configName].toString();
            }
            else
            {
                jo[configName] = "";
                joChanged = true;
            }
        }
        auto joKeys = jo.keys();
        foreach (auto joKey, joKeys)
        {
            if (!configNames.contains(joKey))
            {
                jo.remove(joKey);
                joChanged = true;
            }
        }
        if (joChanged)
        {
            QJsonDocument newJsonDoc(jo);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            file.write(newJsonDoc.toJson());
            file.close();
        }
    }
}

QString ConfigObjectLanguageManager::tanslate(QString className, QString configName)
{
    if (languagePackges.contains(className))
    {
        if (languagePackges[className]->contains(configName))
        {
            return (*languagePackges[className])[configName];
        }
    }
    return "";
}
