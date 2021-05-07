#include "dbhelper.h"

DBHelper::DBHelper(QString connectionName, QString dbName, QString dbUserName, QString dbPassword)
    : connectionName(connectionName), dbName(dbName), dbUserName(dbUserName), dbPassword(dbPassword)
{
    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(dbName);
        db.setUserName(dbUserName);
        db.setPassword(dbPassword);
    }

    if (!db.open())
    {
        qCritical() << QString("Open DB failed! DB: %1, Error: %2").arg(dbName).arg(db.lastError().text());
    }
}

DBHelper::~DBHelper()
{
    db.close();
}

bool DBHelper::isTableExist(QString tableName)
{
    QSqlQuery query(db);
    query.prepare("select count(*) from sqlite_master where type='table' and name=?");
    query.addBindValue(tableName);
    if (!query.exec())
    {
        qCritical() << query.lastError();
        return false;
    }
    if (query.next())
    {
        return query.value(0).toInt() != 0;
    }
    else
    {
        return false;
    }
}
