//
//
//  Generated by StarUML(tm) C++ Add-In
//
//  @ Project : Untitled
//  @ File Name : Logger.h
//  @ Date : 2020/1/7
//  @ Author :
//
//

#if !defined(LOGGER_H)
#define LOGGER_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "../zmqWrapper/publisher.h"
#include "../zmqWrapper/subscriber.h"
#include "LoggerConfig.h"
#include "consoleoutputer.h"
#include "functional"
#include "loglevel.h"
#include <QDateTime>

class UTILITIESSHARED_EXPORT Logger : public QObject
{
    Q_OBJECT

private:
    Logger();

public:
    static Logger *getIns()
    {
        static Logger instance;
        return &instance;
    }

    void registerCategory(QLoggingCategory *category);

    void init(bool outputLogToLocalConsole = false);

    void msgHandler(const QJsonObject &jsonObj);

    static void qlogMessageHandler(QtMsgType msgType, const QMessageLogContext &context, const QString &msg);

    ~Logger() override;

private:
    void initLogLevel(const QString &categoryName);

    void handleSetLogLevel(const QString &category, int logLevel);

    void handleGetLogLevel(const QString &category);

    void setLogEnable(QLoggingCategory &category, int logLevel);

    void setLogEnable(QLoggingCategory &category, QtMsgType qmsgType, int logLevel)
    {
        category.setEnabled(qmsgType, qmsgTypeToLevel[qmsgType] >= logLevel);
    }

private:
    bool isInit = false;
    bool outputLogToLocalConsole = false;
    LoggerConfig *loggerConfig = nullptr;
    ConfigFile *logConfigFile = nullptr;
    QMap<QString, QLoggingCategory *> logCategorys;
    QMap<QString, LogCategoryConfig *> logCategoryConfigs;
    QMap<QtMsgType, int> qmsgTypeToLevel;
    Publisher *msgPublisher;
    Subscriber *msgSubscriber;
    ConsoleOutputer consoleOutputer;
};

class CategoryRegister
{
public:
    CategoryRegister(QLoggingCategory &category)
    {
        Logger::getIns()->registerCategory(&category);
    }
};

#define SILICOOL_CREATE_LOGGING_CATEGORY(categoryGetterName, categoryName)                                                                           \
    QLoggingCategory &categoryGetterName()                                                                                                           \
    {                                                                                                                                                \
        static QLoggingCategory category(categoryName);                                                                                              \
        return category;                                                                                                                             \
    }                                                                                                                                                \
    static CategoryRegister categoryGetterName##___(categoryGetterName());

#define SILICOOL_DECLARE_LOGGING_CATEGORY(name, exportStatement) extern exportStatement QLoggingCategory &name();

#endif    // LOGGER_H
