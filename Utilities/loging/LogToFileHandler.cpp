#include "LogToFileHandler.h"

LogToFileHandler::LogToFileHandler(bool outputToConsole) : outputToConsole(outputToConsole)
{
    logToFileConfig = new LogToFileConfig();
    logToFileConfigFile = new ConfigFile("LogToFileConfig", logToFileConfig, "./config/logConfig/logToFileConfig.json", false);
    addressToBeListenedConfig = new ConfigArray(ConfigElementInfo::Other);
    addressToBeListenedConfigFile
        = new ConfigFile("AddressToBeListened", addressToBeListenedConfig, "./config/logConfig/addressToBeListened.json", false);
}

void LogToFileHandler::init()
{
    logToFileConfigFile->populate(true);
    addRollbackFile(defaultCategoryName, "silicoolLog");
    addressToBeListenedConfigFile->populate(true);
    LogCollector::init(addressToBeListenedConfig);
}

LogToFileHandler::~LogToFileHandler()
{
    logSubscriber.stopListenMessage();
    foreach (auto rollbackFile, rollbackFiles.values())
    {
        delete rollbackFile;
    }
}

void LogToFileHandler::onNewLog(const QString &category, int logLevel, const QString &msg)
{
    if (!rollbackFiles.contains(category))
    {
        addRollbackFile(category, category);
    }
    rollbackFiles[category]->appendLine(msg);

    if (category != defaultCategoryName)
    {
        rollbackFiles[defaultCategoryName]->appendLine(msg);
    }

    if (logLevel >= LogLevel::Warning)
    {
        if (!rollbackFiles.contains(warningCategory))
        {
            addRollbackFile(warningCategory, warningLogName);
        }
        rollbackFiles[warningCategory]->appendLine(msg);
    }

    if (outputToConsole)
    {
        consoleOutputer.outputLogToConsole(logLevel, msg);
    }
}
