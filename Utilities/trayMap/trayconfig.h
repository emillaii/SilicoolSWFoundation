#ifndef TRAYCONFIG_H
#define TRAYCONFIG_H

#include "configManager/configobject.h"
#include "errorHandling/silicolerror.h"
#include "indexvalidator.h"
#include "utilities_global.h"

class UTILITIESSHARED_EXPORT TrayConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(int unitRows READ unitRows WRITE setUnitRows NOTIFY unitRowsChanged)
    Q_PROPERTY(double unitRowDelta READ unitRowDelta WRITE setUnitRowDelta NOTIFY unitRowDeltaChanged)
    Q_PROPERTY(int unitColumns READ unitColumns WRITE setUnitColumns NOTIFY unitColumnsChanged)
    Q_PROPERTY(double unitColumnDelta READ unitColumnDelta WRITE setUnitColumnDelta NOTIFY unitColumnDeltaChanged)
    Q_PROPERTY(int rowsInsideUnit READ rowsInsideUnit WRITE setRowsInsideUnit NOTIFY rowsInsideUnitChanged)
    Q_PROPERTY(double rowDelta READ rowDelta WRITE setRowDelta NOTIFY rowDeltaChanged)
    Q_PROPERTY(int columnsInsideUnit READ columnsInsideUnit WRITE setColumnsInsideUnit NOTIFY columnsInsideUnitChanged)
    Q_PROPERTY(double columnDelta READ columnDelta WRITE setColumnDelta NOTIFY columnDeltaChanged)
    Q_PROPERTY(int rows READ rows WRITE setRows NOTIFY rowsChanged)
    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY columnsChanged)

public:
    TrayConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("rows");
        setReadOnlyProperty("columns");
        init();
    }

    void getValidUiIndex(int &unitRowIndex, int &unitColumnIndex, int &rowIndex, int &columnIndex);
    QPointF getDeltaToLeftTop(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex);
    QPointF getDeltaToRightTop(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex);

    void getValidUiIndex(int &unitIndex, int &rowIndex, int &columnIndex);
    QPointF getDeltaToLeftTop(int unitIndex, int rowIndex, int columnIndex);
    QPointF getDeltaToRightTop(int unitIndex, int rowIndex, int columnIndex);

    int materialCount() const
    {
        return rows() * columns();
    }

    int rows() const
    {
        return unitRows() * rowsInsideUnit();
    }

    int columns() const
    {
        return unitColumns() * columnsInsideUnit();
    }

    int unitRows() const
    {
        return m_unitRows;
    }

    int unitColumns() const
    {
        return m_unitColumns;
    }

    int rowsInsideUnit() const
    {
        return m_rowsInsideUnit;
    }

    int columnsInsideUnit() const
    {
        return m_columnsInsideUnit;
    }

    double rowDelta() const
    {
        return m_rowDelta;
    }

    double columnDelta() const
    {
        return m_columnDelta;
    }

    double unitRowDelta() const
    {
        return m_unitRowDelta;
    }

    double unitColumnDelta() const
    {
        return m_unitColumnDelta;
    }

public slots:
    void setUnitRows(int unitRows)
    {
        if (m_unitRows == unitRows)
            return;

        if (unitRows < 1)
        {
            qCritical() << tr("unitRows must greater than 0!");
            emit unitRowsChanged(m_unitRows);
            return;
        }
        m_unitRows = unitRows;
        emit unitRowsChanged(m_unitRows);
        emit rowsChanged(rows());
    }

    void setUnitColumns(int unitColumns)
    {
        if (m_unitColumns == unitColumns)
            return;

        if (unitColumns < 1)
        {
            qCritical() << tr("unitColumns must greater than 0!");
            emit unitColumnsChanged(m_unitColumns);
            return;
        }
        m_unitColumns = unitColumns;
        emit unitColumnsChanged(m_unitColumns);
        emit columnsChanged(columns());
    }

    void setRowsInsideUnit(int rowsInsideUnit)
    {
        if (m_rowsInsideUnit == rowsInsideUnit)
            return;

        if (rowsInsideUnit < 1)
        {
            qCritical() << tr("rowsInsideUnit must greater than 0!");
            emit rowsInsideUnitChanged(m_rowsInsideUnit);
            return;
        }
        m_rowsInsideUnit = rowsInsideUnit;
        emit rowsInsideUnitChanged(m_rowsInsideUnit);
        emit rowsChanged(rows());
    }

    void setColumnsInsideUnit(int columnsInsideUnit)
    {
        if (m_columnsInsideUnit == columnsInsideUnit)
            return;

        if (columnsInsideUnit < 1)
        {
            qCritical() << tr("columnsInsideUnit must greater than 0!");
            emit columnsInsideUnitChanged(m_columnsInsideUnit);
            return;
        }
        m_columnsInsideUnit = columnsInsideUnit;
        emit columnsInsideUnitChanged(m_columnsInsideUnit);
        emit columnsChanged(columns());
    }

    void setRowDelta(double rowDelta)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_rowDelta, rowDelta))
            return;

        if (rowDelta < 0)
        {
            qCritical() << tr("RowDelta must greater than 0!");
            emit rowDeltaChanged(m_rowDelta);
            return;
        }
        m_rowDelta = rowDelta;
        emit rowDeltaChanged(m_rowDelta);
    }

    void setColumnDelta(double columnDelta)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_columnDelta, columnDelta))
            return;

        if (columnDelta < 0)
        {
            qCritical() << tr("ColumnDelta must greater than 0!");
            emit columnDeltaChanged(m_columnDelta);
            return;
        }
        m_columnDelta = columnDelta;
        emit columnDeltaChanged(m_columnDelta);
    }

    void setUnitRowDelta(double unitRowDelta)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_unitRowDelta, unitRowDelta))
            return;

        if (unitRowDelta < 0)
        {
            qCritical() << tr("UnitRowDelta must greater than 0!");
            emit unitRowDeltaChanged(m_unitRowDelta);
            return;
        }
        m_unitRowDelta = unitRowDelta;
        emit unitRowDeltaChanged(m_unitRowDelta);
    }

    void setUnitColumnDelta(double unitColumnDelta)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_unitColumnDelta, unitColumnDelta))
            return;

        if (unitColumnDelta < 0)
        {
            qCritical() << tr("UnitColumnDelta must greater than 0!");
            emit unitColumnDeltaChanged(m_unitColumnDelta);
            return;
        }
        m_unitColumnDelta = unitColumnDelta;
        emit unitColumnDeltaChanged(m_unitColumnDelta);
    }

private slots:
    void setRows(int rows)
    {
        Q_UNUSED(rows)
    }

    void setColumns(int columns)
    {
        Q_UNUSED(columns)
    }

signals:
    void unitRowsChanged(int unitRows);

    void unitColumnsChanged(int unitColumns);

    void rowsInsideUnitChanged(int rowsInsideUnit);

    void columnsInsideUnitChanged(int columnsInsideUnit);

    void rowDeltaChanged(double rowDelta);

    void columnDeltaChanged(double columnDelta);

    void unitRowDeltaChanged(double unitRowDelta);

    void unitColumnDeltaChanged(double unitColumnDelta);

    void rowsChanged(int rows);

    void columnsChanged(int columns);

private:
    int m_unitRows = 1;
    int m_unitColumns = 1;
    int m_rowsInsideUnit = 1;
    int m_columnsInsideUnit = 1;
    double m_rowDelta = 1;
    double m_columnDelta = 1;
    double m_unitRowDelta = 1;
    double m_unitColumnDelta = 1;
};

#endif    // TRAYCONFIG_H
