#include "logtoviewerhandler.h"

LogToViewerHandler::LogToViewerHandler() : QObject(), msgPublisher("!@#$%")
{
    logToViewerConfig = new LogToViewerConfig();
    logToViewerConfigFile = new ConfigFile("LogToViewerConfig", logToViewerConfig, "./config/logConfig/logToViewerConfig.json", false);
    addressToBeListenedConfig = new ConfigArray(ConfigElementInfo::Other);
    addressToBeListenedConfigFile
        = new ConfigFile("AddressToBeListened", addressToBeListenedConfig, "./config/logConfig/addressToBeListened.json", false);

    connect(this, &LogToViewerHandler::newLog, this, &LogToViewerHandler::handleNewLog);

    appendCategory(defaultCategoryName);
    appendCategory(warnLogName);
}

void LogToViewerHandler::init()
{
    logToViewerConfigFile->populate(true);
    addressToBeListenedConfigFile->populate(true);

    LogCollector::init(addressToBeListenedConfig);
    detectLogBufferTimerID = startTimer(DetectLogBufferInterval);
    try
    {
        msgPublisher.bind(logToViewerConfig->msgPublishAddr());
    }
    catch (std::exception &ex)
    {
        qFatal(ex.what());
    }
}

LogToViewerHandler::~LogToViewerHandler()
{
    logSubscriber.stopListenMessage();
    if (detectLogBufferTimerID != -1)
    {
        killTimer(detectLogBufferTimerID);
        detectLogBufferTimerID = -1;
    }
    foreach (auto logModel, logModels.values())
    {
        delete logModel;
    }
}

void LogToViewerHandler::setLogLevel(QString category, int level)
{
    QJsonObject jsonMsg;
    jsonMsg["msgName"] = "setLogLevel";
    jsonMsg["category"] = category;
    jsonMsg["level"] = level;
    msgPublisher.sendJson(jsonMsg);
}

void LogToViewerHandler::updateLogLevel(QString category)
{
    QJsonObject jsonMsg;
    jsonMsg["msgName"] = "getLogLevel";
    jsonMsg["category"] = category;
    msgPublisher.sendJson(jsonMsg);
}

QObject *LogToViewerHandler::getLogModel(QString category)
{
    if (logModels.contains(category))
    {
        return logModels[category];
    }
    else
    {
        qCritical() << "Did not find category:" << category;
        return nullptr;
    }
}

void LogToViewerHandler::onMessage(const QJsonObject &jsonObj)
{
    if (jsonObj["msgName"].toString() == "getLogLevelRsp")
    {
        QString category = jsonObj["category"].toString();
        int logLevel = jsonObj["logLevel"].toInt();
        emit logLevelPublished(category, logLevel);
    }
}

void LogToViewerHandler::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == detectLogBufferTimerID)
    {
        foreach (auto logModel, logModels.values())
        {
            logModel->flush();
        }
    }
}

void LogToViewerHandler::handleNewLog(QString category, int logLevel, QString msg)
{
    if (!logModels.contains(category))
    {
        appendCategory(category);
        emit newLogModelAttached(category);
    }
    logModels[category]->append(logLevel, msg);
    if (category != defaultCategoryName)
    {
        logModels[defaultCategoryName]->append(logLevel, msg);
    }
    if (logLevel >= LogLevel::Warning)
    {
        logModels[warnLogName]->append(logLevel, msg);
    }
}
