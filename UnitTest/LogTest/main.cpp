#include "loging/LogToFileHandler.h"
#include "loging/Logger.h"
#include "loging/logtoviewerhandler.h"
#include "logwrittermanager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    LogToFileHandler logServer(false);
    logServer.init();
    LogToViewerHandler logHandler;
    logHandler.init();
    engine.rootContext()->setContextProperty("logHandler", &logHandler);
    Logger::getIns()->init(true);    // client side

    LogWritterManager lwm;
    engine.rootContext()->setContextProperty("$lwm", &lwm);

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
