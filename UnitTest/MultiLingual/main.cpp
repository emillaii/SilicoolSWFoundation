#include "configfile.h"
#include "languagemanager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    LanguageConfig languageConfig;
    ConfigFile languageConfigFile("languageConfig", &languageConfig, "languageConfig.json", false);
    QQmlApplicationEngine engine;
    LanguageManager languageManager(&languageConfig, app, engine);
    engine.rootContext()->setContextProperty("languageConfig", &languageConfig);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    languageConfigFile.populate(true);

    return app.exec();
}
