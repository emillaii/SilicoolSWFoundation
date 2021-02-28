#include "twodimensioncalibration.h"

TwoDimensionCalibration::~TwoDimensionCalibration()
{
    freeDatas(&m_aPoints);
    freeDatas(&m_bPoints);
}

void TwoDimensionCalibration::setData(int rows, int columns, QList<QPointF> &aPoints, QList<QPointF> &bPoints)
{
    isValid = false;
    if (rows < 1 || columns < 1)
    {
        qCWarning(tdcCate()) << "Rows or columns was less than 1!";
        return;
    }
    if (rows < 2 && columns < 2)
    {
        qCWarning(tdcCate()) << "Rows and columns were less than 2 at the same time!";
        return;
    }
    if (aPoints.length() != rows * columns || bPoints.length() != rows * columns)
    {
        qCWarning(tdcCate()) << "Error points length!";
        return;
    }

    freeDatas(&m_aPoints);
    freeDatas(&m_bPoints);
    m_rows = rows;
    m_columns = columns;
    populate(&m_aPoints, reinterpret_cast<char *>(aPoints.toVector().data()));
    populate(&m_bPoints, reinterpret_cast<char *>(bPoints.toVector().data()));
    isValid = true;
}

void TwoDimensionCalibration::load(QString fileName)
{
    isValid = false;
    QFile f(fileName);
    if (!f.exists())
    {
        qCWarning(tdcCate) << "File not exist!" << fileName;
        return;
    }

    f.open(QIODevice::ReadOnly);
    QByteArray datas = f.readAll();
    if (datas.length() < sizeof(int) * 2)
    {
        qCWarning(tdcCate) << "File size was too small!" << datas.length();
        return;
    }

    freeDatas(&m_aPoints);
    freeDatas(&m_bPoints);

    m_rows = *reinterpret_cast<int *>(datas.data());
    m_columns = *(reinterpret_cast<int *>(datas.data()) + 1);
    if (m_rows < 1 || m_columns < 1)
    {
        qCWarning(tdcCate()) << "Rows or columns was less than 1!";
        return;
    }
    if (m_rows < 2 && m_columns < 2)
    {
        qCWarning(tdcCate()) << "Rows and columns were less than 2 at the same time!";
        return;
    }

    if (datas.length() != headerLen() + pointsRawDataLen() * 2)
    {
        qCWarning(tdcCate()) << "Error file size!" << datas.length();
        return;
    }

    populate(&m_aPoints, datas.data() + headerLen());
    populate(&m_bPoints, datas.data() + headerLen() + pointsRawDataLen());
    isValid = true;
}

void TwoDimensionCalibration::save(QString fileName)
{
    if (!isValid)
    {
        qCWarning(tdcCate()) << "Calibration data is invalid!";
        return;
    }
    char *datas = new char[headerLen() + pointsRawDataLen() * 2];
    *reinterpret_cast<int *>(datas) = m_rows;
    *(reinterpret_cast<int *>(datas) + 1) = m_columns;
    duplicate(&m_aPoints, datas + headerLen());
    duplicate(&m_bPoints, datas + headerLen() + pointsRawDataLen());

    if (!QFileInfo(fileName).dir().exists())
    {
        QDir().mkpath(QFileInfo(fileName).dir().absolutePath());
    }

    QFile f(fileName);
    f.open(QIODevice::WriteOnly);
    f.write(datas, headerLen() + pointsRawDataLen() * 2);
    f.close();
    delete[] datas;
}

QPointF TwoDimensionCalibration::nnA2B(QPointF aPoint)
{
    if (!isValid)
    {
        qCWarning(tdcCate()) << "Invalid two dimension calibration!";
        return aPoint;
    }
    if (m_rows < 2)
    {
        return pointXA2B(aPoint);
    }
    if (m_columns < 2)
    {
        return pointYA2B(aPoint);
    }
    int rowIndex = -1;
    int columnIndex = -1;
    if (!findNearestAPoint(aPoint, rowIndex, columnIndex))
    {
        for (int i = 0; i < m_rows - 1; i++)
        {
            if (isInRange(aPoint.y(), m_aPoints[i][0].y(), m_aPoints[i + 1][0].y()))
            {
                rowIndex = i;
                break;
            }
        }
        if (rowIndex < 0)
        {
            if (aPoint.y() <= m_aPoints[0][0].y() && aPoint.y() <= m_aPoints[m_rows - 1][0].y())
            {
                rowIndex = 0;
            }
            else
            {
                rowIndex = m_rows - 2;
            }
        }

        for (int i = 0; i < m_columns - 1; i++)
        {
            if (isInRange(aPoint.x(), m_aPoints[0][i].x(), m_aPoints[0][i + 1].x()))
            {
                columnIndex = i;
                break;
            }
        }
        if (columnIndex < 0)
        {
            if (aPoint.x() <= m_aPoints[0][0].x() && aPoint.x() <= m_aPoints[0][m_columns - 1].x())
            {
                columnIndex = 0;
            }
            else
            {
                columnIndex = m_columns - 2;
            }
        }
    }

    QVector<QPointF> aPoints;
    QVector<QPointF> bPoints;
    aPoints << m_aPoints[rowIndex][columnIndex] << m_aPoints[rowIndex + 1][columnIndex] << m_aPoints[rowIndex][columnIndex + 1];
    bPoints << m_bPoints[rowIndex][columnIndex] << m_bPoints[rowIndex + 1][columnIndex] << m_bPoints[rowIndex][columnIndex + 1];
    QPointF bPoint = affineTransformCalc.calcAffineTransform(aPoints, bPoints).map(aPoint);
    qDebug(tdcCate()) << "Find a points:" << aPoints;
    qDebug(tdcCate()) << "Map a point to b point:" << aPoint << bPoint;
    return bPoint;
}

void TwoDimensionCalibration::printSelf()
{
    if (!isValid)
    {
        return;
    }
    QDebug dbg = qDebug();
    dbg << "Rows:" << m_rows << "Columns:" << m_columns << "\r\n";
    dbg << "APoints:\r\n";
    for (int i = 0; i < m_rows; i++)
    {
        dbg << i << ":";
        for (int j = 0; j < m_columns; j++)
        {
            dbg << m_aPoints[i][j];
        }
        dbg << "\r\n";
    }
    dbg << "BPoints:\r\n";
    for (int i = 0; i < m_rows; i++)
    {
        dbg << i << ":";
        for (int j = 0; j < m_columns; j++)
        {
            dbg << m_bPoints[i][j];
        }
        dbg << "\r\n";
    }
}

bool TwoDimensionCalibration::findNearestAPoint(QPointF aPoint, int &rowIndex, int &columnIndex)
{
    QPointF leftTop;
    QPointF rightBottom;
    for (int i = 0; i < m_rows - 1; i++)
    {
        for (int j = 0; j < m_columns - 1; j++)
        {
            leftTop = m_aPoints[i][j];
            rightBottom = m_aPoints[i + 1][j + 1];
            if (isInRange(aPoint.x(), leftTop.x(), rightBottom.x()) && isInRange(aPoint.y(), leftTop.y(), rightBottom.y()))
            {
                rowIndex = i;
                columnIndex = j;
                return true;
            }
        }
    }
    return false;
}

QPointF TwoDimensionCalibration::pointXA2B(QPointF aPoint)
{
    int columnIndex = -1;
    for (int i = 0; i < m_columns - 1; i++)
    {
        if (isInRange(aPoint.x(), m_aPoints[0][i].x(), m_aPoints[0][i + 1].x()))
        {
            columnIndex = i;
            break;
        }
    }
    if (columnIndex < 0)
    {
        if (aPoint.x() <= m_aPoints[0][0].x() && aPoint.x() <= m_aPoints[0][m_columns - 1].x())
        {
            columnIndex = 0;
        }
        else
        {
            columnIndex = m_columns - 2;
        }
    }
    QPointF a1 = m_aPoints[0][columnIndex];
    QPointF a2 = m_aPoints[0][columnIndex + 1];
    QPointF b1 = m_bPoints[0][columnIndex];
    QPointF b2 = m_bPoints[0][columnIndex + 1];
    double bPointX = calcY(a1.x(), a2.x(), b1.x(), b2.x(), aPoint.x());
    QPointF bPoint(bPointX, aPoint.y());
    qDebug(tdcCate()) << "Find a points:" << a1 << a2;
    qDebug(tdcCate()) << "Map a point to b point:" << aPoint << bPoint;
    return bPoint;
}

QPointF TwoDimensionCalibration::pointYA2B(QPointF aPoint)
{
    int rowIndex = -1;
    for (int i = 0; i < m_rows - 1; i++)
    {
        if (isInRange(aPoint.y(), m_aPoints[i][0].y(), m_aPoints[i + 1][0].y()))
        {
            rowIndex = i;
            break;
        }
    }
    if (rowIndex < 0)
    {
        if (aPoint.y() <= m_aPoints[0][0].y() && aPoint.y() <= m_aPoints[m_rows - 1][0].y())
        {
            rowIndex = 0;
        }
        else
        {
            rowIndex = m_rows - 2;
        }
    }
    QPointF a1 = m_aPoints[rowIndex][0];
    QPointF a2 = m_aPoints[rowIndex + 1][0];
    QPointF b1 = m_bPoints[rowIndex][0];
    QPointF b2 = m_bPoints[rowIndex + 1][0];
    double bPointY = calcY(a1.y(), a2.y(), b1.y(), b2.y(), aPoint.y());
    QPointF bPoint = QPointF(aPoint.x(), bPointY);
    qDebug(tdcCate()) << "Find a points:" << a1 << a2;
    qDebug(tdcCate()) << "Map a point to b point:" << aPoint << bPoint;
    return bPoint;
}

double TwoDimensionCalibration::calcY(double x1, double x2, double y1, double y2, double x)
{
    if (qFuzzyCompare(x1, x2))
    {
        return x;
    }
    double k = (y1 - y2) / (x1 - x2);
    double b = y1 - k * x1;
    return k * x + b;
}

void TwoDimensionCalibration::freeDatas(QPointF ***pointsPtr)
{
    if (*pointsPtr != nullptr && m_rows > 0)
    {
        for (int i = 0; i < m_rows; i++)
        {
            delete[]((*pointsPtr)[i]);
        }
        delete[](*pointsPtr);
    }
    *pointsPtr = nullptr;
}

void TwoDimensionCalibration::populate(QPointF ***pointsPtr, const char *rawData)
{
    *pointsPtr = new QPointF *[m_rows];
    for (int i = 0; i < m_rows; i++)
    {
        (*pointsPtr)[i] = new QPointF[m_columns];
        memcpy((*pointsPtr)[i], rawData + sizeof(QPointF) * m_columns * i, sizeof(QPointF) * m_columns);
    }
}

void TwoDimensionCalibration::duplicate(QPointF ***pointsPtr, char *dest)
{
    QPointF **points = *pointsPtr;
    for (int i = 0; i < m_rows; i++)
    {
        memcpy(dest + sizeof(QPointF) * m_columns * i, points[i], sizeof(QPointF) * m_columns);
    }
}

SILICOOL_CREATE_LOGGING_CATEGORY(tdcCate, "TwoDimCali")
