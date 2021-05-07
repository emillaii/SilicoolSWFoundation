#include "alarmhelper.h"

AlarmHelper::AlarmHelper(QObject *parent) : QObject(parent), dbHelper("qt_sql_alarms_connection", "Alarms.db")
{
    if (!dbHelper.isTableExist("Alarms"))
    {
        QSqlQuery createTable(dbHelper.db);
        createTable.prepare(
            "create table Alarms(timeStamp varchar(120), mSecsSinceEpoch bigint primary key, module varchar(120), alarmMessage varchar(1024))");
        if (!createTable.exec())
        {
            qCritical() << createTable.lastError().text();
        }
    }
    m_alarmModel = new MySqlTableModel(this, dbHelper.db);
    m_alarmModel->setTable("Alarms");
    m_alarmModel->setSort(1, Qt::DescendingOrder);
    m_alarmModel->select();
    removeInvalidRecords();
}

AlarmHelper::~AlarmHelper()
{
    delete m_alarmModel;
}

void AlarmHelper::appendAlarm(const QString &module, QString errMsg)
{
    errMsg.replace("\r\n", "  ").replace("\r", "  ").replace("\n", "  ");
    QSqlQuery query(dbHelper.db);
    query.prepare("insert into Alarms(timeStamp, mSecsSinceEpoch, module, alarmMessage) values(?,?,?,?)");
    query.addBindValue(QDateTime::currentDateTime().toString());
    query.addBindValue(QDateTime::currentDateTime().currentMSecsSinceEpoch());
    query.addBindValue(module);
    query.addBindValue(errMsg);
    if (!query.exec())
    {
        qCritical() << QString("Insert alarm message failed! Module: %1, Message: %2").arg(module).arg(errMsg);
        return;
    }
    m_alarmModel->select();
}

void AlarmHelper::removeInvalidRecords()
{
    qint64 invalidTime = QDateTime::currentMSecsSinceEpoch() - static_cast<qint64>(180) * 24 * 60 * 60 * 1000;
    QSqlQuery remove(dbHelper.db);
    remove.prepare("delete from Alarms where mSecsSinceEpoch<?");
    remove.addBindValue(invalidTime);
    if (!remove.exec())
    {
        qCritical() << tr("Remove invalid records from alarm database failed!") << remove.lastError();
        return;
    }
    m_alarmModel->select();
}
