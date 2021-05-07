#ifndef ALARMHELPER_H
#define ALARMHELPER_H

#include "DBHelper/dbhelper.h"
#include "DBHelper/mysqltablemodel.h"
#include <QDateTime>
#include <QDebug>
#include <QObject>

class AlarmHelper : public QObject
{
    Q_OBJECT
public:
    explicit AlarmHelper(QObject *parent = nullptr);

    ~AlarmHelper() override;

    MySqlTableModel *alarmModel() const
    {
        return m_alarmModel;
    }

    void appendAlarm(const QString &module, QString errMsg);

private:
    void removeInvalidRecords();

private:
    MySqlTableModel *m_alarmModel;
    DBHelper dbHelper;
};

#endif    // ALARMHELPER_H
