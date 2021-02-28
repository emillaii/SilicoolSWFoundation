#ifndef AFFINETRANSFORMCALC_H
#define AFFINETRANSFORMCALC_H

#include "errorHandling/scassert.h"
#include "opencv2/opencv.hpp"
#include <QMatrix>
#include <QVector>

class AffineTransformCalc
{
public:
    AffineTransformCalc() = default;

    QMatrix calcAffineTransform(const QVector<QPointF> &pixelPoints, const QVector<QPointF> &motorPoints);

private:
    std::vector<cv::Point2f> toCvPoints(const QVector<QPointF> &points);
};

#endif    // AFFINETRANSFORMCALC_H
