#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "MotionManager/motionmanager.h"
#include "VisionLocation/visionlocation.h"
#include "affinetransformcalc.h"
#include "calibrationconfig.h"
#include "errorHandling/scassert.h"
#include "mymath.h"
#include "opencv2/opencv.hpp"
#include "scvision_global.h"
#include <QObject>
#include <QPointF>
#include <QVector>
#include <qmath.h>

typedef std::function<void(CalibrationConfig *, QVector<QPointF> &, QVector<QPointF> &)> CustomCaliFunc;
typedef std::function<void(PrOffset)> CustomPerformPrResultFunc;
typedef std::function<QPointF()> CustomGetPixelPointFunc;

class CalibrationMoveInfo
{
public:
    CalibrationMoveInfo(QString axisName, double step) : axisName(axisName), step(step) {}

    QString axisName;
    double step;
};

class SCVISIONSHARED_EXPORT Calibration : public QObject
{
    Q_OBJECT

public:
    explicit Calibration(CalibrationConfig *config, VisionLocation *location, QObject *parent = nullptr);

    QString calibrationName() const;

    CalibrationConfig *config() const
    {
        return m_config;
    }

    void performCalibration();

    void performPRResult(PrOffset offset);
    void performPROriginResult(PrOffset offset);

    QPointF getOnePxielDistance();
    QPointF getOneXPxielDistance();
    QPointF getOneYPxielDistance();

    void setCustomCaliFunc(CustomCaliFunc customCaliFunc, CustomPerformPrResultFunc customPerformPrResultFunc);
    void setCustomGetPixelPointFunc(CustomGetPixelPointFunc customGetPixelPointFunc);

    QPointF getPixelPoint();

private:
    void getCorrespondingPixelMotorPoints(QVector<QPointF> &pixelPoints, QVector<QPointF> &motorPoints);
    void calcAffineTransform(const QVector<QPointF> &pixelPoints, const QVector<QPointF> &motorPoints);
    bool calculateMatrixAttribute(QVector<QPointF> p, QVector<QPointF> m, double &scaleX, double &scaleY, double &closestAngle);
    QPointF getPixelPoint(int times, double errorLimit);

private:
    CalibrationConfig *m_config;
    VisionLocation *location;

    AffineTransformCalc affineTransformCalc;
    CustomGetPixelPointFunc customGetPixelPointFunc = nullptr;
    CustomCaliFunc customCaliFunc = nullptr;
    CustomPerformPrResultFunc customPerformPrResultFunc = nullptr;
};

#endif    // CALIBRATION_H
