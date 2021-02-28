#ifndef CONFIGOBJECTLANGUAGEMANAGER_H
#define CONFIGOBJECTLANGUAGEMANAGER_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>

class ConfigObjectLanguageManager
{
private:
    ConfigObjectLanguageManager() {}

public:
    static ConfigObjectLanguageManager &getIns()
    {
        static ConfigObjectLanguageManager instance;
        return instance;
    }

    void solve(QString className, QStringList &baseClassNames, QStringList configNames);

    QString tanslate(QString className, QString configName);

private:
    const QString languagePackgeDir = "./ConfigObjectLanguagePackge/Chinese/";
    QMap<QString, QMap<QString, QString> *> languagePackges;
};

#endif    // CONFIGOBJECTLANGUAGEMANAGER_H
