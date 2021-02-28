#include "filecopier.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Silicool");

    QQmlApplicationEngine engine;

    // ConfigManager
    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();
    engine.rootContext()->setContextProperty("configManager", ConfigManager::getIns());

    FileCopier fileCopier;
    engine.rootContext()->setContextProperty("sourceFiles", fileCopier.sourceFiles());
    engine.rootContext()->setContextProperty("destDirs", fileCopier.destDirs());
    engine.rootContext()->setContextProperty("fileCopier", &fileCopier);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
