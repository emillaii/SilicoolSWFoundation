#ifndef CIRCLEEDGEFITTING_H
#define CIRCLEEDGEFITTING_H

#include "AVL.h"
#include "STD.h"
#include "adaptvisionprresultimageinfo.h"
#include "scvision.h"
#include <QObject>

class CircleEdgeFitting
{
public:
    CircleEdgeFitting();
    static bool PR_Circle_Edge_Fitting(
        QImage image, QString pr_name, PRResultStruct &prResult, QVariantMap args, AdaptVisionPRResultImageInfo *prResultImageInfo);
};

#endif    // CIRCLEEDGEFITTING_H
