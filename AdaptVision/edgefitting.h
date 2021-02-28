#ifndef EDGEFITTING_H
#define EDGEFITTING_H

#include "AVL.h"
#include "STD.h"
#include "adaptvisionprresultimageinfo.h"
#include "scvision.h"
#include <QObject>

static QHash<QString, atl::Conditional<avl::SegmentFittingField>> segmentFittingFieldHashMap;
static QHash<QString, atl::Conditional<avl::GrayModel>> grayModelHashMap;
static QHash<QString, atl::Conditional<avl::Vector2D>> vector2DHashMap;
static QHash<QString, avl::Region> regionHashMap;

struct EdgeTransitionState
{
    atl::Array<avl::Point2D> point2DArray1;
    avl::Path path1;
    avl::ImageProfileAlongPathState imageProfileAlongPathState1;
    avl::Profile profile1;
    avl::Path path2;
    avl::Profile profile2;
    atl::Array<float> realArray1;
    atl::Array<float> realArray2;
};

class EdgeFitting
{
public:
    EdgeFitting();
    static bool PR_Edge_Fitting(QImage image,
                                QString pr_name,
                                PRResultStruct &prResult,
                                QVariantMap args,
                                AdaptVisionPRResultImageInfo *prResultImageInfo,
                                double &edgeTransition,
                                bool outEdgeTransition);

    static double EdgeTransition(EdgeTransitionState &state, const avl::Point2D &inValue1, const avl::Point2D &inValue2, const avl::Image &inImage);
};

#endif    // EDGEFITTING_H
