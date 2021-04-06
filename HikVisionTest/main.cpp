#include "TaskEngine/taskengineemanager.h"
#include "Vision/dummyvisionfactory.h"
#include "basicconfig.h"
#include "dummydispatcher.h"
#include "dummymotionelementfactory.h"
#include "duttypemanager.h"
#include "externexelaucher.h"
#include "languageManager/languagemanager.h"
#include "loging/LogToFileHandler.h"
#include "loging/Logger.h"
#include "loging/logtoviewerhandler.h"
#include "mashineStateIndicator/mashinestateindicator.h"
#include "systemCore/statemachine.h"
#include "uiHelper/uioperationimpl.h"
#include "userManagement/usermanagement.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickView>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Silicool");

    BasicConfig basicConfig;
    ConfigFile basicConfigFile("BasicConfig", &basicConfig, "./config/basicConfig.json");
    basicConfigFile.populate();

    // Set controls style
    QQuickStyle::setStyle(basicConfig.skin());

    DutTypeManager dutTypeManager(&basicConfig);

    QQuickView *startupView = new QQuickView();
    startupView->rootContext()->setContextProperty("dutTypeManager", &dutTypeManager);
    startupView->rootContext()->setContextProperty("basicConfig", &basicConfig);
    startupView->setSource(QUrl("qrc:/StartupViewer.qml"));
    startupView->setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    startupView->setColor("transparent");
    startupView->setResizeMode(QQuickView::SizeViewToRootObject);
    startupView->show();
    while (true)
    {
        if (dutTypeManager.launchResult() == 0)
        {
            startupView->close();
            delete startupView;
            return 0;
        }
        else if (dutTypeManager.launchResult() == 1)
        {
            startupView->close();
            delete startupView;
            break;
        }
        else
        {
            app.processEvents(QEventLoop::AllEvents, 200);
        }
    }

    Worker::setWorkerConfigDir(QString("./config/DutType/%1/workers").arg(basicConfig.dutType()));

    set_terminate(scTerminate);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("appVersion", QCoreApplication::applicationVersion());

    engine.rootContext()->setContextProperty("dutTypeManager", &dutTypeManager);

    engine.rootContext()->setContextProperty("basicConfig", &basicConfig);
    engine.rootContext()->setContextProperty("languagePackageNames", basicConfig.languageConfig()->languagePackageNames());
    engine.rootContext()->setContextProperty("isMarerialStyle", QVariant::fromValue(basicConfig.skin().toLower() == "material"));

    // Log system
    LogToFileHandler logServer(false);
    logServer.init();
    LogToViewerHandler logHandler;
    logHandler.init();
    engine.rootContext()->setContextProperty("logHandler", &logHandler);
    Logger::getIns()->init(true);    // client side

    // InstructionExecutor
    InstructionExecutionServer::getIns().init("local:ieServer");
    InstructionExecutionClient::getIns().init("local:ieServer");

    // ConfigManager
    qmlRegisterType<ConfigElementInfo>("ConfigElementType", 1, 0, "ConfigElementType");
    qmlRegisterType<ConfigObject>("ConfigObject", 1, 0, "ConfigObject");
    qRegisterMetaType<ConfigElementInfo::Type>();
    engine.rootContext()->setContextProperty("configManager", ConfigManager::getIns());

    // ConfigShare
    ConfigShareServer::getIns().init("local:configShareCenter");
    ConfigShareClient::getIns().init("local:configShareCenter");

    // UserManagement
    UserManagement userManagement;
    userManagement.init();
    qmlRegisterType<UserManagement>("UserMng", 1, 0, "UserMng");
    engine.rootContext()->setContextProperty("userManagement", &userManagement);
    engine.rootContext()->setContextProperty("userModel", userManagement.userModel);

    // UI operation
    qRegisterMetaType<MsgBoxItem>();
    qRegisterMetaType<MsgBoxIcon::Icon>();
    qmlRegisterType<MsgBoxIcon>("MsgBoxIcon", 1, 0, "MsgBoxIcon");
    InstructionExecutionServer::getIns().registerObject("UIOperationImpl", &UIOperationImpl::getIns());
    UIOperation::getIns()->init("UIOperationImpl");    // client side
    engine.rootContext()->setContextProperty("uiOperationImpl", &UIOperationImpl::getIns());
    engine.rootContext()->setContextProperty("msgBoxModel", UIOperationImpl::getIns().msgBoxModel());
    engine.rootContext()->setContextProperty("uiOperation", UIOperation::getIns());

    TaskEngineeManager tem(nullptr, 5);
    qmlRegisterType<TaskEngineeManager>("TaskEngineeManager", 1, 0, "TaskEngineeManager");
    engine.rootContext()->setContextProperty("tem", &tem);

    // LanguageManager
    LanguageManager languageManager(basicConfig.languageConfig(), app, engine);
    qmlRegisterType<LanguageConfig>("LanguageConfig", 1, 0, "LanguageConfig");
    engine.rootContext()->setContextProperty("languageConfig", basicConfig.languageConfig());

    // VisionTool
    ExternExeLaucher exeLaucher;
    engine.rootContext()->setContextProperty("externExeConfigs", exeLaucher.getExternExeConfigs());
    engine.rootContext()->setContextProperty("exeLaucher", &exeLaucher);

    // Vision
    QString dutRelatedVisionConfigDir = QString("./config/DutType/%1/vision").arg(basicConfig.dutType());
    VisionConfigDir::getIns().setDutRelatedConfigDir(dutRelatedVisionConfigDir);
    VisionConfigManager visionConfigManager(HikVisionLocationConfig::staticMetaObject, dutRelatedVisionConfigDir);
    DummyVisionFactory visionFactory(&visionConfigManager);
    VisionManager::getIns().setVisionFactory(&visionFactory);
    VisionManager::getIns().setVisionConfigManager(&visionConfigManager);
    VisionManager::getIns().preInit();
    engine.rootContext()->setContextProperty("wordopLscConfig", visionFactory.wordopLscConfig());
    auto hikVision = qobject_cast<HikVision *>(VisionManager::getIns().vision());
    engine.rootContext()->setContextProperty("hikVision", hikVision);
    engine.rootContext()->setContextProperty("hikVisionConfig", hikVision->config());
    VisionManager::getIns().setContextProperty(engine);

    // Motion
    qmlRegisterType<MotionElement>("MotionElement", 1, 0, "MotionElement");
    DummyMotionElementFactory motionElementFactory;
    MotionManager::getIns().setBasicElementFactory(&motionElementFactory);
    MasterMotionManager masterMotionManager(&motionElementFactory, QString("./config/DutType/%1/motion").arg(basicConfig.dutType()));
    masterMotionManager.setContextProperty(engine);

    visionConfigManager.setOptionalMotors(masterMotionManager.motionConfigManager()->axisNames());

    // XYZRDebugger
    engine.rootContext()->setContextProperty("xyzrDebuggerNames", XYZRDebuggerManager::getIns().debuggerNames());
    engine.rootContext()->setContextProperty("debuggerShortcuts", XYZRDebuggerManager::getIns().debuggerShortcuts());
    engine.rootContext()->setContextProperty("xyzrDebuggerManager", &XYZRDebuggerManager::getIns());
    engine.rootContext()->setContextProperty("xyzrDebuggerDefs", XYZRDebuggerManager::getIns().debuggerDefs());
    engine.rootContext()->setContextProperty("xyzrDebuggerConfigs", XYZRDebuggerManager::getIns().debuggerConfigs());
    visionConfigManager.setOptionalXYZR(XYZRDebuggerManager::getIns().debuggerNames());

    // State machine
    qRegisterMetaType<SMD::Event>();
    qRegisterMetaType<SMD::State>();
    DummyDispatcher dispatcher(&masterMotionManager);
    StateMachine stateMachine(&dispatcher);
    InstructionExecutionServer::getIns().registerObject("stateMachine", &stateMachine);
    ErrorReporter::getIns().init("MainSW", "stateMachine");
    engine.rootContext()->setContextProperty("stateMachine", &stateMachine);
    engine.rootContext()->setContextProperty("dispatcher", &dispatcher);
    dispatcher.setContextProperty(engine);
    qmlRegisterType<SMD>("SMD", 1, 0, "SMD");

    // Machine state indicator
    SCBuzzer buzzer;
    SCTowerLight towerLight;
    MachineStateIndicatorConfig msiConfig;
    ConfigFile msiConfigFile("MachineStateIndicator", &msiConfig, "./config/platformConfig/machineStateIndicatorConfig.json");
    msiConfigFile.populate();
    MachineStateIndicator msi(&buzzer, &towerLight, &msiConfig);
    MotionManager::getIns().attachBuzzerTowerLight(&buzzer, &towerLight);
    engine.rootContext()->setContextProperty("msiConfig", &msiConfig);
    QObject::connect(&stateMachine, &StateMachine::currentStateChanged, &msi, &MachineStateIndicator::onStateChanged);

    QQuickView loadingView(QUrl("qrc:/LoadingAnimation.qml"));
    loadingView.setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    loadingView.setColor("transparent");
    loadingView.setResizeMode(QQuickView::SizeViewToRootObject);
    loadingView.show();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url, &loadingView](QObject *obj, const QUrl &objUrl) {
            loadingView.close();
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    basicConfig.languageConfig()->publishLanguage();

    int result = app.exec();

    return result;
}
