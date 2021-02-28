#include "../ConfigManagerTest/configmanagerunittest.h"
#include "configManager/configarray.h"
#include "configManager/configshareclient.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qDebug() << "main thread id:" << QThread::currentThreadId();

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();

    ConfigShareClient *csc = &ConfigShareClient::getIns();
    TestClassA tca;
    TestClassC tcc;
    ConfigArray intArray(ConfigElementInfo::Int);

    csc->bindToRemoteConfig("tca", &tca);
    csc->bindToRemoteConfig("tcc", &tcc);
    csc->bindToRemoteConfig("intArray", &intArray);

    csc->init("tcp://127.0.0.1:8000");

    engine.rootContext()->setContextProperty("tca", &tca);
    engine.rootContext()->setContextProperty("tcc", &tcc);
    engine.rootContext()->setContextProperty("intArray", &intArray);

    engine.rootContext()->setContextProperty("replic", csc);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    int res = app.exec();
    csc->deleteLater();
    return res;
}
