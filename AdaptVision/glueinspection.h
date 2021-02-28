#ifndef GLUEINSPECTION_H
#define GLUEINSPECTION_H
#include <QObject>
#include "scvision.h"
#include "AVL.h"
#include "STD.h"

class glueInspection
{
    struct RegionJudgeState
    {
        avl::Region region1;
        atl::Array< avl::Region > regionArray1;
        atl::Array< avl::Region > regionArray2;
        atl::Array< avl::Region > regionArray3;
    };


    struct WidthJudgeState
    {
        avl::Image image1;
        atl::Conditional< atl::Array< atl::Conditional< avl::Path > > > pathArray1;
        atl::Array< avl::Path > pathArray2;
        atl::Array< avl::Path > pathArray3;
        atl::Array< avl::Path > pathArray4;
        atl::Array< avl::Path > pathArray5;
        atl::Array< avl::Path > pathArray6;
        atl::Conditional< avl::Path > path1;
        atl::Conditional< avl::Path > path2;
        atl::Conditional< avl::Path > path3;
        atl::Array< avl::Point2D > point2DArray1;
        atl::Conditional< avl::Path > path4;
        atl::Conditional< atl::Array< atl::Conditional< avl::Segment2D > > > segment2DArray1;
        avl::Profile profile1;
        atl::Array< float > realArray1;
        atl::Array< avl::Segment2D > segment2DArray2;
        atl::Conditional< atl::String > string1;
        atl::Array< bool > boolArray1;
        atl::Array< bool > boolArray2;
        atl::Array< avl::Segment2D > segment2DArray3;
        atl::Array< avl::Segment2D > segment2DArray4;
        atl::Conditional< atl::String > string2;
        atl::Array< atl::Conditional< avl::Path > > pathArray7;
        avl::Image image2;
        atl::Array< atl::Conditional< avl::Path > > pathArray8;
        avl::Image image3;
        avl::Image image4;
        atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray1;
        atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray2;
        atl::Conditional< atl::Array< atl::Conditional< atl::String > > > stringArray3;
        avl::Image image5;
        atl::Conditional< atl::String > string3;
        atl::String string4;
        atl::String string5;
        atl::Array< atl::Conditional< atl::String > > stringArray4;
    };

public:
    glueInspection();
    static bool glueCheck(QImage &imageBefore,
                          QImage &imageAfter,
                          double resolution,
                          QImage &resultImage,
                          double &inMinGlueWidthSpec,
                          double &inMaxGlueWidthSpec,
                          double &inAvgGlueWidthSpec,
                          double &outMinGlueWidth,
                          double &outMaxGlueWidth,
                          double &outMaxAvgGlueWidth);
    static void RegionJudge( RegionJudgeState& state, const avl::Image& inSubtractImage, const avl::Image& inAfterImage, bool& outRegionOk, atl::Conditional< avl::Region >& outRegion, avl::Region& outRegion1, avl::Region& outRegion2, atl::Conditional< avl::Region >& outRegion3 );
    static void WidthJudge( WidthJudgeState& state, bool inRegionOk, atl::Conditional< const avl::Region& > inRegion, float inResolution, float inMinWidth, float inMaxWidth, const avl::Image& inAfterImage, float inMaxAveWidth, bool& outResultOK, avl::Image& outResultImage, atl::Conditional< float >& outMaxWidth, atl::Conditional< float >& outMinWidth, atl::Conditional< float >& outAveWidth );
    static void diffenenceImage(QImage image1, QImage image2);
};

#endif // GLUEINSPECTION_H
