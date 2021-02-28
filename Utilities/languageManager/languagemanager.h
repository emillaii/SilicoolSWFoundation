#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include "../utilities_global.h"
#include "languageconfig.h"
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>

class UTILITIESSHARED_EXPORT LanguageManager : public QObject
{
    Q_OBJECT

public:
    LanguageManager(LanguageConfig *languageConfig, QGuiApplication &app, QQmlApplicationEngine &engine)
        : languageConfig(languageConfig), app(app), engine(engine)
    {
        connect(languageConfig, &LanguageConfig::languageChanged, this, &LanguageManager::onLanguageChanged);
    }

private slots:
    void onLanguageChanged(int language)
    {
        QString suffix;
        if (language == LanguageConfig::Chinese)
        {
            suffix = "chinese.qm";
        }
        else if (language == LanguageConfig::English)
        {
            suffix = "english.qm";
        }
        else
        {
            qCritical("Unknown language: %d", language);
            return;
        }

        foreach (auto translator, installedTranslators)
        {
            app.removeTranslator(translator);
        }
        installedTranslators.clear();

        for (int i = 0; i < languageConfig->languagePackageNames()->count(); i++)
        {
            QString langPackName = QString(":/languagePackage/%1_%2")
                                       .arg(languageConfig->languagePackageNames()->at(i).toString())
                                       .arg(suffix);
            QTranslator *translator = new QTranslator();
            if (translator->load(langPackName))
            {
                app.installTranslator(translator);
                installedTranslators.append(translator);
            }
            else
            {
                qCritical() << "Load language package failed!" << langPackName;
            }
        }
        engine.retranslate();
    }

private:
    LanguageConfig *languageConfig;
    QGuiApplication &app;
    QQmlApplicationEngine &engine;
    QList<QTranslator *> installedTranslators;
};

#endif    // LANGUAGEMANAGER_H
