#include "Logger.h"
#include <iostream>

Logger::Logger()
{
    qmsgTypeToLevel[QtDebugMsg] = LogLevel::Debug;
    qmsgTypeToLevel[QtInfoMsg] = LogLevel::Info;
    qmsgTypeToLevel[QtWarningMsg] = LogLevel::Warning;
    qmsgTypeToLevel[QtCriticalMsg] = LogLevel::Critical;
    qmsgTypeToLevel[QtFatalMsg] = LogLevel::Fatal;

    msgPublisher = new Publisher("!@#$%");
    msgSubscriber = new Subscriber(nullptr, "!@#$%");

    registerCategory(QLoggingCategory::defaultCategory());
}

void Logger::registerCategory(QLoggingCategory *category)
{
    logCategorys[category->categoryName()] = category;
    if (isInit)
    {
        initLogLevel(category->categoryName());
    }
}

void Logger::init(bool outputLogToLocalConsole)
{
    this->outputLogToLocalConsole = outputLogToLocalConsole;
    loggerConfig = new LoggerConfig;
    logConfigFile = new ConfigFile("LoggerConfig", loggerConfig, "./config/logConfig/loggerConfig.json", false);
    logConfigFile->populate(true);
    auto categoryConfigs = loggerConfig->categoryConfigs();
    for (int i = 0; i < categoryConfigs->count(); i++)
    {
        auto categoryConfig = categoryConfigs->getConfig<LogCategoryConfig>(i);
        logCategoryConfigs[categoryConfig->category()] = categoryConfig;
    }
    foreach (auto registeredCategory, logCategorys.keys())
    {
        initLogLevel(registeredCategory);
    }

    try
    {
        msgPublisher->bind(loggerConfig->logPublishAddr());
        msgSubscriber->subscribe(loggerConfig->msgSubscribeAddr());
    }
    catch (std::exception &ex)
    {
        qFatal(ex.what());
    }
    msgSubscriber->installMessageHandle(std::bind(&Logger::msgHandler, this, std::placeholders::_1));
    msgSubscriber->startListenMessage();

    qSetMessagePattern("%{function}");
    qInstallMessageHandler(qlogMessageHandler);
    isInit = true;
}

void Logger::msgHandler(const QJsonObject &jsonObj)
{
    QString msgName = jsonObj["msgName"].toString();
    if (msgName == "setLogLevel")
    {
        handleSetLogLevel(jsonObj["category"].toString(), jsonObj["level"].toInt());
    }
    else if (msgName == "getLogLevel")
    {
        handleGetLogLevel(jsonObj["category"].toString());
    }
}

void Logger::qlogMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg)
{
    auto ins = getIns();

    int logLevel = ins->qmsgTypeToLevel[msgType];
    auto souceCodeFileName = QFileInfo(context.file).fileName();
    QString location = QString("%1:%2(%3)").arg(souceCodeFileName).arg(context.line).arg(qFormatLogMessage(msgType, context, msg));
    QString formatedMsg = QString("%1 [%2] [%3] %4: %5")
                              .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                              .arg(context.category)
                              .arg(LogLevel::getIns().toString(logLevel))
                              .arg(location)
                              .arg(msg);

    QJsonObject jsonObj;
    jsonObj["msgName"] = "newLog";
    jsonObj["category"] = context.category;
    jsonObj["level"] = logLevel;
    jsonObj["log"] = formatedMsg;
    ins->msgPublisher->sendJson(jsonObj);

    if (ins->outputLogToLocalConsole)
    {
        ins->consoleOutputer.outputLogToConsole(logLevel, formatedMsg);
    }
}

Logger::~Logger()
{
    msgSubscriber->stopListenMessage();
    qSetMessagePattern("%{if-category}%{category}: %{endif}%{message}");
    qInstallMessageHandler(nullptr);
    SILICOOL_DELETE(logConfigFile);
    SILICOOL_DELETE(loggerConfig);
    msgPublisher->deleteLater();
    msgSubscriber->deleteLater();
}

void Logger::initLogLevel(const QString &categoryName)
{
    if (!logCategoryConfigs.contains(categoryName))
    {
        LogCategoryConfig *newCategoryConfig = new LogCategoryConfig();
        newCategoryConfig->setCategory(categoryName);
        logCategoryConfigs[categoryName] = newCategoryConfig;
        loggerConfig->categoryConfigs()->executeAddConfigObject(0, newCategoryConfig);
    }
    auto categoryConfig = logCategoryConfigs[categoryName];
    if (categoryConfig->logLevel() >= LogLevel::Debug && categoryConfig->logLevel() < LogLevel::Fatal)
    {
        setLogEnable(*logCategorys[categoryName], categoryConfig->logLevel());
    }
}

void Logger::handleSetLogLevel(const QString &category, int logLevel)
{
    if (logCategorys.contains(category))
    {
        setLogEnable(*logCategorys[category], logLevel);
        logCategoryConfigs[category]->setLogLevel(logLevel);
    }
}

void Logger::handleGetLogLevel(const QString &category)
{
    if (logCategoryConfigs.contains(category))
    {
        QJsonObject jsonObj;
        jsonObj["msgName"] = "getLogLevelRsp";
        jsonObj["category"] = category;
        jsonObj["logLevel"] = logCategoryConfigs[category]->logLevel();
        msgPublisher->sendJson(jsonObj);
    }
}

void Logger::setLogEnable(QLoggingCategory &category, int logLevel)
{
    setLogEnable(category, QtDebugMsg, logLevel);
    setLogEnable(category, QtInfoMsg, logLevel);
    setLogEnable(category, QtWarningMsg, logLevel);
    setLogEnable(category, QtCriticalMsg, logLevel);
}
