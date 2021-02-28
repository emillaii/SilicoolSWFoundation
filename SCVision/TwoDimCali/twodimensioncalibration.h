#ifndef TWODIMENSIONCALIBRATION_H
#define TWODIMENSIONCALIBRATION_H

#include "Calibration/affinetransformcalc.h"
#include "loging/Logger.h"
#include "scvision_global.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QObject>

SILICOOL_DECLARE_LOGGING_CATEGORY(tdcCate, )

class SCVISIONSHARED_EXPORT TwoDimensionCalibration
{
public:
    TwoDimensionCalibration &operator=(const TwoDimensionCalibration &) = delete;
    TwoDimensionCalibration(const TwoDimensionCalibration &) = delete;

    explicit TwoDimensionCalibration() {}

    ~TwoDimensionCalibration();

    void setData(int rows, int columns, QList<QPointF> &aPoints, QList<QPointF> &bPoints);

    void load(QString fileName);

    void save(QString fileName);

    ///
    /// \brief nnA2B    Nearest neighbor A to B
    ///
    QPointF nnA2B(QPointF aPoint);

    void printSelf();

private:
    int headerLen() const
    {
        return sizeof(int) * 2;
    }

    int pointsRawDataLen() const
    {
        return sizeof(QPointF) * m_rows * m_columns;
    }

    bool findNearestAPoint(QPointF aPoint, int &rowIndex, int &columnIndex);

    QPointF pointXA2B(QPointF aPoint);

    QPointF pointYA2B(QPointF aPoint);

    double calcY(double x1, double x2, double y1, double y2, double x);

    bool isInRange(qreal value, qreal range1, qreal range2)
    {
        return (value >= range1 && value <= range2) || (value >= range2 && value <= range1);
    }

    void freeDatas(QPointF ***pointsPtr);

    void populate(QPointF ***pointsPtr, const char *rawData);

    void duplicate(QPointF ***pointsPtr, char *dest);

private:
    bool isValid = false;
    int m_rows = -1;
    int m_columns = -1;
    QPointF **m_aPoints = nullptr;
    QPointF **m_bPoints = nullptr;
    AffineTransformCalc affineTransformCalc;
};

#endif    // TWODIMENSIONCALIBRATION_H
