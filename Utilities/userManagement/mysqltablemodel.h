#ifndef USERTABLEMODEL_H
#define USERTABLEMODEL_H

#include "utilities_global.h"
#include <QObject>
#include <qsqlfield.h>
#include <qsqlrecord.h>
#include <qsqltablemodel.h>

class UTILITIESSHARED_EXPORT MySqlTableModel : public QSqlTableModel
{
    Q_OBJECT

public:
    MySqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    Q_INVOKABLE QVariant get(int rowIndex, QString fieldName);

    // QAbstractItemModel interface
public:
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};

#endif    // USERTABLEMODEL_H
