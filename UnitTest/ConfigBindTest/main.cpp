#include "QQmlContext"
#include "basicconfig.h"
#include "configmanagerunittest.h"
#include "languageManager/languagemanager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();
    engine.rootContext()->setContextProperty("configManager", ConfigManager::getIns());
    engine.rootContext()->setContextProperty("isMarerialStyle", QVariant(false));

    BasicConfig basicConfig;
    ConfigFile basicConfigFile("BasicConfig", &basicConfig, "./config/basicConfig.json");
    basicConfigFile.populate();

    LanguageManager languageManager(basicConfig.languageConfig(), app, engine);
    qmlRegisterType<LanguageConfig>("LanguageConfig", 1, 0, "LanguageConfig");
    engine.rootContext()->setContextProperty("languageConfig", basicConfig.languageConfig());

    TestClassB testClassB;
    testClassB.setObjectName("testClassB");
    ConfigFile testClassBConfigFile("testClassB", &testClassB, "testClassB.json");
    testClassBConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassB", &testClassB);

    TestClassB testClassB2;
    testClassB2.setObjectName("testClassB2");
    ConfigFile testClassB2ConfigFile("testClassB", &testClassB2, "testClassB2.json");
    testClassB2ConfigFile.populate();
    engine.rootContext()->setContextProperty("testClassB2", &testClassB2);

    Test t(&testClassB, &testClassB2);
    engine.rootContext()->setContextProperty("t", &t);

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
