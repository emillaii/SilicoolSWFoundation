#include "affinetransformcalc.h"

QMatrix AffineTransformCalc::calcAffineTransform(const QVector<QPointF> &pixelPoints, const QVector<QPointF> &motorPoints)
{
    SC_ASSERT(pixelPoints.count() == motorPoints.count());
    SC_ASSERT(pixelPoints.count() >= 3);

    std::vector<cv::Point2f> cvPixelPoints = toCvPoints(pixelPoints);
    std::vector<cv::Point2f> cvMotorPoints = toCvPoints(motorPoints);

    cv::Mat transformMat = cv::estimateRigidTransform(cvPixelPoints, cvMotorPoints, true);
    if (transformMat.empty())
    {
        throw SilicolAbort("Calculate affine transform failed!");
    }

    return QMatrix(transformMat.ptr<double>(0)[0], transformMat.ptr<double>(1)[0], transformMat.ptr<double>(0)[1], transformMat.ptr<double>(1)[1],
                   transformMat.ptr<double>(0)[2], transformMat.ptr<double>(1)[2]);
}

std::vector<cv::Point2f> AffineTransformCalc::toCvPoints(const QVector<QPointF> &points)
{
    std::vector<cv::Point2f> cvPoints;
    for (int i = 0; i < points.count(); i++)
    {
        cvPoints.push_back(cv::Point2f(points[i].x(), points[i].y()));
    }
    return cvPoints;
}
