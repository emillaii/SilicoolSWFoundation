#include "logmodel.h"

LogModel::LogModel()
{
    logRoleNames[LogModelLevelRole] = "logLevel";
    logRoleNames[LogModelLogRole] = "logStr";
}

void LogModel::append(int logLevel, const QString &msg)
{
    logBuffer.append(new LogItem{ logLevel, msg });
}

void LogModel::flush()
{
    if (logBuffer.count() > 0)
    {
        if (logs.count() > MAX_LINES)
        {
            removeFrontRows(logs.count() - MAX_LINES / 2);
        }

        beginInsertRows(QModelIndex(), logs.count(), logs.count() + logBuffer.count() - 1);
        foreach (auto logItem, logBuffer)
        {
            logs.append(logItem);
        }
        logBuffer.clear();
        endInsertRows();
    }
}

void LogModel::onClearLog()
{
    if (logs.count() > 0)
    {
        removeFrontRows(logs.count());
    }
}

void LogModel::removeFrontRows(int rowCount)
{
    beginRemoveRows(QModelIndex(), 0, rowCount - 1);
    for (int i = 0; i < rowCount; i++)
    {
        LogItem *logItem = logs.takeFirst();
        delete logItem;
    }
    endRemoveRows();
}
