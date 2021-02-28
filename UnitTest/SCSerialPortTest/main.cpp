#include "TaskEngine/instructionexecutionclient.h"
#include "TaskEngine/instructionexecutionserver.h"
#include "serialporttest.h"
#include "uiHelper/uioperation.h"
#include "uiHelper/uioperationimpl.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // InstructionExecutor
    InstructionExecutionServer::getIns().init("local:ieServer");
    InstructionExecutionClient::getIns().init("local:ieServer");

    // UI operation
    qmlRegisterType<MsgBoxIcon>("MsgBoxIcon", 1, 0, "MsgBoxIcon");
    InstructionExecutionServer::getIns().registerObject("UIOperationImpl", &UIOperationImpl::getIns());
    UIOperation::getIns()->init("UIOperationImpl");    // client side
    engine.rootContext()->setContextProperty("uiOperationImpl", &UIOperationImpl::getIns());
    engine.rootContext()->setContextProperty("msgBoxModel", UIOperationImpl::getIns().msgBoxModel());
    engine.rootContext()->setContextProperty("uiOperation", UIOperation::getIns());

    SerialPortTest serialPortTest;
    engine.rootContext()->setContextProperty("serialPortTest", &serialPortTest);

    qDebug() << "main thread" << QThread::currentThreadId();

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
