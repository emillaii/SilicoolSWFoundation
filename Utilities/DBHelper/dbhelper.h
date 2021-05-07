#ifndef DBHELPER_H
#define DBHELPER_H

#include "utilities_global.h"
#include <QDebug>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qsqlerror.h>

class UTILITIESSHARED_EXPORT DBHelper
{
public:
    explicit DBHelper(QString connectionName, QString dbName, QString dbUserName = "Silicol", QString dbPassword = "Silicol_Psw");

    ~DBHelper();

    bool isTableExist(QString tableName);

public:
    QSqlDatabase db;

private:
    QString connectionName;
    QString dbName;
    QString dbUserName;
    QString dbPassword;
};

#endif    // DBHELPER_H
