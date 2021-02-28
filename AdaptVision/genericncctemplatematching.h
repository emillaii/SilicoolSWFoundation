#ifndef GENERICNCCTEMPLATEMATCHING_H
#define GENERICNCCTEMPLATEMATCHING_H

#include "adaptvisionprresultimageinfo.h"
#include "scvision.h"
#include <QObject>

class GenericNCCTemplateMatching
{
public:
    GenericNCCTemplateMatching();
    static bool PR_Generic_NCC_Template_Matching(
        QImage image, QString pr_name, PRResultStruct &prResult, QVariantMap args, AdaptVisionPRResultImageInfo *prResultImageInfo);
};

#endif    // GENERICNCCTEMPLATEMATCHING_H
