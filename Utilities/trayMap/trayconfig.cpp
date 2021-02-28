#include "trayconfig.h"

void TrayConfig::getValidUiIndex(int &unitRowIndex, int &unitColumnIndex, int &rowIndex, int &columnIndex)
{
    validateIndex(unitRowIndex, unitRows(), "UnitRowIndex out of range");
    validateIndex(unitColumnIndex, unitColumns(), "UnitColumnIndex out of range");
    validateIndex(rowIndex, rowsInsideUnit(), "Row index out of range");
    validateIndex(columnIndex, columnsInsideUnit(), "Column index out of range");
}

QPointF TrayConfig::getDeltaToLeftTop(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex)
{
    double xDelta = unitColumnIndex * unitColumnDelta() + columnIndex * columnDelta();
    double yDelta = unitRowIndex * unitRowDelta() + rowIndex * rowDelta();
    return QPointF(xDelta, yDelta);
}

QPointF TrayConfig::getDeltaToRightTop(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex)
{
    double deltaX = (unitColumns() - 1 - unitColumnIndex) * unitColumnDelta() + (columnsInsideUnit() - 1 - columnIndex) * columnDelta();
    double deltaY = unitRowIndex * unitRowDelta() + rowIndex * rowDelta();
    return QPointF(deltaX, deltaY);
}

void TrayConfig::getValidUiIndex(int &unitIndex, int &rowIndex, int &columnIndex)
{
    validateIndex(unitIndex, unitColumns() * unitRows(), "UnitIndex out of range");
    validateIndex(rowIndex, rowsInsideUnit(), "Row index out of range");
    validateIndex(columnIndex, columnsInsideUnit(), "Column index out of range");
}

QPointF TrayConfig::getDeltaToLeftTop(int unitIndex, int rowIndex, int columnIndex)
{
    int unitRowIndex = unitIndex / unitColumns();
    int unitColumnIndex = unitIndex % unitColumns();
    return getDeltaToLeftTop(unitRowIndex, unitColumnIndex, rowIndex, columnIndex);
}

QPointF TrayConfig::getDeltaToRightTop(int unitIndex, int rowIndex, int columnIndex)
{
    int unitRowIndex = unitIndex / unitColumns();
    int unitColumnIndex = unitIndex % unitColumns();
    return getDeltaToRightTop(unitRowIndex, unitColumnIndex, rowIndex, columnIndex);
}
