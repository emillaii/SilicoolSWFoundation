#include "configManager/configfile.h"
#include "errorHandling/silicolerror.h"
#include "mashinestateindicator.h"
#include "statemachine.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

void test()
{
    throw SilicolAbort("Test abort msg");
}

int main(int argc, char *argv[])
{
    set_terminate(scTerminate);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();

    test();

    Buzzer buzzer;
    TowerLight towerLight;
    MashineStateIndicatorConfig mashineStateIndicatorConfig;
    engine.rootContext()->setContextProperty("mashineStateIndicatorConfig", &mashineStateIndicatorConfig);
    ConfigFile machineStateIndicatorConfigFile("mashineStateIndicatorConfig", &mashineStateIndicatorConfig,
                                               "mashineStateIndicatorConfig.json");
    machineStateIndicatorConfigFile.populate();
    MashineStateIndicator machineStateIndicator(&buzzer, &towerLight, &mashineStateIndicatorConfig);

    Dispatcher dispatcher;
    StateMachine sm(&dispatcher);
    qRegisterMetaType<SMD::State>();
    QObject::connect(&sm, &StateMachine::currentStateChanged, &machineStateIndicator,
                     &MashineStateIndicator::onStateChanged);

    engine.rootContext()->setContextProperty("sm", &sm);
    qmlRegisterType<SMD>("SMD", 1, 0, "SMD");

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
