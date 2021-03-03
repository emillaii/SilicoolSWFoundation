#ifndef HIKVISION_H
#define HIKVISION_H

#include "errorHandling/scassert.h"
#include "hikvisionlocationconfig.h"
#include "hikvisionresultimageinfo.h"
#include "scvision.h"
#include <QObject>

class HikVision : public SCVision
{
    Q_OBJECT
public:
    explicit HikVision(QObject *parent = nullptr)
    {
        // load vision master
    }
    ~HikVision()
    {
        // unload vision master
    }

    // SCVision interface
public:
    bool performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
    {
        HikVisionLocationConfig *hikVLCfg = qobject_cast<HikVisionLocationConfig *>(prConfig);
        SC_ASSERT(hikVLCfg != nullptr);

        HikVisionResultImageInfo *hikResultImageInfo = new HikVisionResultImageInfo();
        (*resultImageInfo) = hikResultImageInfo;

        // TBD
        return true;
    }
    bool performPr(QImage &image,
                   VisionLocationConfig *prConfig,
                   PRResultImageInfo **resultImageInfo,
                   PRResultStruct &prResult,
                   int roiRowIndex,
                   int rowColIndex)
    {
        throw SilicolAbort("Unimplemented function: performPr");
    }
    double getObjectSharpness(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo)
    {
        throw SilicolAbort("Unimplemented function: getObjectSharpness");
    }
    void drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo)
    {
        HikVisionResultImageInfo *hikResultImageInfo = qobject_cast<HikVisionResultImageInfo *>(resultImageInfo);
        SC_ASSERT(hikResultImageInfo != nullptr);

        // TBD
    }
    bool glueCheck(QImage &imageBefore,
                   QImage &imageAfter,
                   double resoultion,
                   QImage &resultImage,
                   double &outMinGlueWidth,
                   double &outMaxGlueWidth,
                   double &outMaxAvgGlueWidth)
    {
        // TBD
        return true;
    }
    QVector<PathEndPoint> readDispensePath(QString fileName)
    {
        // TBD
        return QVector<PathEndPoint>();
    }
    bool checkDongle()
    {
        // TBD
        return true;
    }
};

#endif    // HIKVISION_H
