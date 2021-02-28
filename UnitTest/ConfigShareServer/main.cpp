#include "../ConfigManagerTest/configmanagerunittest.h"
#include "configManager/configarray.h"
#include "configManager/configshareserver.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();

    auto css = &ConfigShareServer::getIns();
    TestClassA tca;
    TestClassC tcc;
    ConfigArray intArray(ConfigElementInfo::Int);

    ConfigFile tcaFile("tca", &tca, "tca.json");
    ConfigFile tccFile("tcc", &tcc, "tcc.json");
    ConfigFile intArrayFile("intArray", &intArray, "intArray.json");
    tcaFile.populate();
    tccFile.populate();
    intArrayFile.populate();

    css->share("tca", &tca);
    css->share("tcc", &tcc);
    css->share("intArray", &intArray);

    QRemoteObjectHost host;
    host.setHostUrl(QUrl("tcp://127.0.0.1:8000"));
    host.enableRemoting(&(ConfigShareServer::getIns()));

    engine.rootContext()->setContextProperty("tca", &tca);
    engine.rootContext()->setContextProperty("tcc", &tcc);
    engine.rootContext()->setContextProperty("intArray", &intArray);

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
