#include "MotionManager/motionmanager.h"
#include "mastermotionmanager.h"
#include "motionutility.h"
#include "xtmotionfactory.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    //    QQuickStyle::setStyle("Material");
    //    QQuickStyle::setStyle("Universal");
    QQuickStyle::setStyle("Fusion");
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();

    qmlRegisterType<MotionElement>("MotionElement", 1, 0, "MotionElement");

    XTMotionFactory xtMotionFactory;
    MasterMotionManager mms(&xtMotionFactory);
    engine.rootContext()->setContextProperty("masterMotionManager", &mms);
    MotionManager mm;
    engine.rootContext()->setContextProperty("motionManager", &mm);

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
