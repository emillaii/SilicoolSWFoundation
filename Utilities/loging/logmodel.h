#ifndef LOGMODEL_H
#define LOGMODEL_H

#include "../utilities_global.h"
#include <QAbstractListModel>

struct LogItem
{
public:
    int level;
    QString log;
};

enum LogModelRole
{
    LogModelLevelRole = Qt::UserRole + 1,
    LogModelLogRole
};

class UTILITIESSHARED_EXPORT LogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LogModel();

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
    {
        Q_UNUSED(parent)
        return logs.count();
    }

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (index.row() >= 0 && index.row() < logs.count())
        {
            if (role == LogModelLevelRole)
            {
                return QVariant(logs[index.row()]->level);
            }
            if (role == LogModelLogRole)
            {
                return QVariant(logs[index.row()]->log);
            }
        }
        return QVariant();
    }

    virtual QHash<int, QByteArray> roleNames() const override
    {
        return logRoleNames;
    }

    void append(int logLevel, const QString &msg);
    void flush();

public slots:
    void onClearLog();

private:
    void removeFrontRows(int rowCount);

private:
    const int MAX_LINES = 1500;
    QList<LogItem *> logs;
    QList<LogItem *> logBuffer;
    QHash<int, QByteArray> logRoleNames;
};

#endif    // LOGMODEL_H
