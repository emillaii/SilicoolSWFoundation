#ifndef SCVISION_H
#define SCVISION_H

#include "CompositeElement/dispensepath.h"
#include "VisionLocation/visionlocationconfig.h"
#include "enumhelper.h"
#include "scvision_global.h"
#include <QImage>

#define _IN
#define _OUT
#define _INOUT

struct SCVISIONSHARED_EXPORT PRResultStruct
{
public:
    PRResultStruct() : x(0), y(0), theta(0), width(0), height(0) {}

    PRResultStruct(double x, double y, double theta, double width, double height) : x(x), y(y), theta(theta), width(width), height(height) {}

    QString toString() const
    {
        return QString("x: %1, y: %2, theta: %3, w: %4, h: %5").arg(x).arg(y).arg(theta).arg(width).arg(height);
    }

    double x;
    double y;

    double ori_x;
    double ori_y;

    double theta;

    double width;
    double height;
};

class SCVISIONSHARED_EXPORT PRResultImageInfo : public QObject
{
    Q_OBJECT

public:
    PRResultImageInfo(QObject *parent = nullptr) : QObject(parent) {}
};

class SCVISIONSHARED_EXPORT SCVision : public QObject
{
    Q_OBJECT

public:
    void init();
    void dispose();

    virtual void initImpl() {}
    virtual void disposeImpl() {}
    virtual void handleDutTypeChanged() {}

    virtual ~SCVision() {}

    ///
    /// \brief performPr
    /// \param image input image
    /// \param prConfig input pr config
    /// \param resultImageInfo out result image info, would be passed to drawResultImage next.
    /// \param prResult out pr result
    /// \return
    ///
    virtual bool
    performPr(_IN QImage &image, _IN VisionLocationConfig *prConfig, _OUT PRResultImageInfo **resultImageInfo, _OUT PRResultStruct &prResult)
        = 0;

    virtual bool performPr(QImage &image,
                           VisionLocationConfig *prConfig,
                           PRResultImageInfo **resultImageInfo,
                           PRResultStruct &prResult,
                           int roiRowIndex,
                           int rowColIndex)
    {
        Q_UNUSED(image)
        Q_UNUSED(prConfig)
        Q_UNUSED(resultImageInfo)
        Q_UNUSED(prResult)
        Q_UNUSED(roiRowIndex)
        Q_UNUSED(rowColIndex)
        return false;
    }

    ///
    /// \brief drawResultImage
    /// \param image in and out image
    /// \param resultImageInfo created by perform pr implementation
    ///
    virtual void drawResultImage(_INOUT QImage &image, _IN PRResultImageInfo *resultImageInfo) = 0;

    virtual bool glueCheck(QImage &imageBefore,
                           QImage &imageAfter,
                           double resoultion,
                           QImage &resultImage,
                           double &outMinGlueWidth,
                           double &outMaxGlueWidth,
                           double &outMaxAvgGlueWidth)
    {
        Q_UNUSED(imageBefore)
        Q_UNUSED(imageAfter)
        Q_UNUSED(resoultion)
        Q_UNUSED(resultImage)
        Q_UNUSED(outMinGlueWidth)
        Q_UNUSED(outMaxGlueWidth)
        Q_UNUSED(outMaxAvgGlueWidth)
        return false;
    }

    virtual QVector<PathEndPoint> readDispensePath(QString fileName) = 0;

    virtual bool checkDongle()
    {
        return true;
    }

private:
    bool isInit = false;
};

#endif    // SCVISION_H
