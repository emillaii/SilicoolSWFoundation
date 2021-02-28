#include "commandextractor.h"
#include "lensloader.h"
#include "lut.h"
#include "subscriber.h"
#include "taskengineemanager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LensLoader lensLoader;
    Lut lut;
    TaskEngineeManager tem;

    //    extractCommand<Lut>("../JsCommands");
    //    extractCommand<LensLoader>("../JsCommands");
    //    return 0;

    qmlRegisterType<TaskEngineeManager>("TaskEngineeManager", 1, 0, "TaskEngineeManager");
    engine.rootContext()->setContextProperty("lensLoader", &lensLoader);
    engine.rootContext()->setContextProperty("lut", &lut);
    engine.rootContext()->setContextProperty("tem", &tem);

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
