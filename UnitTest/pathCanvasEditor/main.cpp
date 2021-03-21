#include "pathCanvasEditor/pathsettingtable.h"
#include "pathCanvasEditor/pointstablemodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Silicool");

    QQmlApplicationEngine engine;

    qmlRegisterType<PointsTableModel>("PointsModel", 1, 0, "PointsTableModel");
    qmlRegisterType<PathSettingTable>("PathSettingModel", 1, 0, "PathSettingTableModel");

    PathSettingTable pathSettingTable;
    engine.rootContext()->setContextProperty("pathSettingTable", &pathSettingTable);

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
