#include "lenspicker.h"
#include "taskengineemanager.h"
#include "uiHelper/uioperation.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    TaskEngineeManager tem;
    engine.rootContext()->setContextProperty("tem", &tem);
    LensPicker lensPicker;
    engine.rootContext()->setContextProperty("lensPicker", &lensPicker);

    qRegisterMetaType<MsgBoxItem>();
    qRegisterMetaType<MsgBoxIcon::Icon>();
    qmlRegisterType<MsgBoxIcon>("MsgBoxIcon", 1, 0, "MsgBoxIcon");
    MsgBoxModel msgBoxModel;
    UIOperation::getIns()->init(&msgBoxModel, app.thread()->currentThreadId());
    engine.rootContext()->setContextProperty("msgBoxModel", &msgBoxModel);
    engine.rootContext()->setContextProperty("uiOperation", UIOperation::getIns());

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
