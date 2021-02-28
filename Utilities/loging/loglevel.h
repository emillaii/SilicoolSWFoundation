#ifndef LOGLEVEL_H
#define LOGLEVEL_H

#include <QMap>

class LogLevel
{
public:
    enum Level
    {
        Debug = 0,
        Info,
        Warning,
        Critical,
        Fatal
    };

private:
    LogLevel()
    {
        levelToString[Debug] = "Debug";
        levelToString[Info] = "Info";
        levelToString[Warning] = "Warning";
        levelToString[Critical] = "Critical";
        levelToString[Fatal] = "Fatal";
    }

public:
    static const LogLevel &getIns()
    {
        static const LogLevel instance;
        return instance;
    }

    QString toString(int level) const
    {
        return levelToString[level];
    }

private:
    QMap<int, QString> levelToString;
};

#endif    // LOGLEVEL_H
