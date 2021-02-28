#ifndef ADAPTVISION_H
#define ADAPTVISION_H

#include "adaptvision_global.h"
#include "adaptvisionlocationconfig.h"
#include "circleedgefitting.h"
#include "commonmethod.h"
#include "commonutils.h"
#include "edgefitting.h"
#include "genericncctemplatematching.h"
#include "glueinspection.h"

class ADAPTVISIONSHARED_EXPORT AdaptVision : public SCVision
{

public:
    AdaptVision();

    // SCVision interface
public:
    virtual bool checkDongle() override;
    bool performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult);
    bool performPr(QImage &image,
                   VisionLocationConfig *prConfig,
                   PRResultImageInfo **resultImageInfo,
                   PRResultStruct &prResult,
                   int roiRowIndex,
                   int rowColIndex);
    double getObjectSharpness(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo);
    void drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo);
    QVector<PathEndPoint> readDispensePath(QString fileName);
};

#endif    // ADAPTVISION_H
