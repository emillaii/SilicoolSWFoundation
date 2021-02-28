#include "edgefitting.h"
#include "ATL/Dummy.h"
#include "ATL/Error.h"
#include "ATL/Optional.h"
#include "ExpressionsRuntime/ExpressionsRuntime.h"
#include "Serialization/Serialization.h"
#include "commonutils.h"
#include <QElapsedTimer>

EdgeFitting::EdgeFitting() {}

double EdgeFitting::EdgeTransition(EdgeTransitionState &state, const avl::Point2D &inValue1, const avl::Point2D &inValue2, const avl::Image &inImage)
{
    float real1;
    float real2;
    float real3;
    float real4;
    atl::Conditional<float> real5;
    atl::Conditional<float> real6;
    atl::Conditional<float> real7;

    // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear() and Array::Reserve
    // function in hand-written programs.
    avs::AvsFilter_CreateArray<avl::Point2D>(inValue1, inValue2, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, atl::NIL, state.point2DArray1);

    // Function AvsFilter_MakePath is intended for generated code only. Consider use of proper Path constructor instead.
    avs::AvsFilter_MakePath(state.point2DArray1, false, state.path1);
    avl::RotatePath(state.path1, atl::NIL, 90.0f, false, state.path1);
    avl::RescalePath(state.path1, atl::NIL, 0.1f, false, state.path1);
    avl::PathLength(state.path1, atl::Dummy<float>().Get());
    avl::ImageProfileAlongPath(state.imageProfileAlongPathState1, inImage, state.path1, atl::NIL, 5, avl::InterpolationMethod::Bilinear, 0.6f,
                               state.profile1, state.path2, atl::NIL, atl::Dummy<atl::Array<avl::Path>>().Get());
    avl::ProfileMaximum(state.profile1, avl::ProfileInterpolationMethod::Quadratic4, atl::Dummy<float>().Get(), atl::NIL, real1);
    avl::ProfileMinimum(state.profile1, avl::ProfileInterpolationMethod::Quadratic4, atl::Dummy<float>().Get(), atl::NIL, real2);
    real3 = _avfml_st_real(_avfml_ld_real(real1) * 0.9);
    real4 = _avfml_st_real((_avfml_ld_real(real1) - _avfml_ld_real(real2)) * 0.2 + _avfml_ld_real(real2));
    avl::SubtractFromProfile(state.profile1, real3, state.profile2);
    avl::SubtractFromProfile(state.profile1, real4, state.profile1);
    avl::ProfileZeroCrossings(state.profile2, state.realArray1);
    avl::ProfileZeroCrossings(state.profile1, state.realArray2);

    // AvsFilter_GetArrayElement_OrNil is intended for generated code only, consider use of built-in language features in regular programs.
    avs::AvsFilter_GetArrayElement_OrNil<float>(state.realArray1, 0, false, real5);

    // AvsFilter_GetArrayElement_OrNil is intended for generated code only, consider use of built-in language features in regular programs.
    avs::AvsFilter_GetArrayElement_OrNil<float>(state.realArray2, 0, false, real6);
    real7 = _avfml_cnd_st_real(_avfml_cnd_absf(_avfml_cnd_subf(_avfml_cnd_ld_real(real5), _avfml_cnd_ld_real(real6))));
    // qInfo("Edge Transition: %f", real7.Get());
    return real7.Get();
}

bool EdgeFitting::PR_Edge_Fitting(QImage image,
                                  QString prName,
                                  PRResultStruct &prResult,
                                  QVariantMap args,
                                  AdaptVisionPRResultImageInfo *prResultImageInfo,
                                  double &edgeTransition,
                                  bool outEdgeTransition)
{
    QElapsedTimer timer;
    EdgeTransitionState edgeTransitionState1;
    timer.start();
    if (!checkFileExist(prName))
        return false;
    bool detect_small_hole = false;
    int selectedCol = 0;
    selectedCol = args["col"].toInt();
    double objectScore = args["objectScore"].toDouble();
    qInfo("Selected Col: %d objectScore: %f", selectedCol, objectScore);
    atl::String g_constData1;
    atl::String g_constData2;
    atl::String g_emptyString;
    atl::String g_constData3;
    atl::String g_constData4;
    atl::String g_constData5;
    atl::String g_constData6;
    atl::String g_constData7;
    atl::String g_constData8;
    atl::String g_constData9;
    atl::String g_constData10;
    atl::String g_constData11;
    atl::String g_constData12;
    atl::String g_constData13;
    atl::String g_constData14;
    atl::String g_constData15;
    atl::String g_constData16;
    atl::Array<atl::Conditional<avl::Location>> g_constData17;

    QString edgeFittingField1Filename = prName;
    QString edgeFittingField2Filename = prName;
    QString edgeFittingField3Filename = prName;
    QString edgeFittingField4Filename = prName;
    QString offsetFilename = prName;
    QString searchHoleFilename = prName;
    QString searchRegionFilename = prName;

    edgeFittingField1Filename.replace("_edgeModel", "_edgeFittingField1");
    edgeFittingField2Filename.replace("_edgeModel", "_edgeFittingField2");
    edgeFittingField3Filename.replace("_edgeModel", "_edgeFittingField3");
    edgeFittingField4Filename.replace("_edgeModel", "_edgeFittingField4");
    offsetFilename.replace("_edgeModel", "_offset");
    searchHoleFilename.replace("_edgeModel", "_searchHole");
    searchRegionFilename.replace("_edgeModel", "_searchRegion");

    g_emptyString = L"";
    g_constData11 = L"SegmentFittingField?";
    g_constData12 = L"GrayModel?";
    g_constData13 = L"Vector2D?";
    g_constData14 = L"CircleFittingField?";
    g_constData15 = L"Angle: ";
    g_constData16 = L" Score:";
    g_constData17.Reset(1);
    g_constData17[0] = avl::Location(200, 60);

    atl::String string1;
    atl::String string2 = edgeFittingField1Filename.toStdString().c_str();
    ;
    atl::String string3 = edgeFittingField2Filename.toStdString().c_str();
    ;
    atl::String string4 = edgeFittingField3Filename.toStdString().c_str();
    ;
    atl::String string5 = edgeFittingField4Filename.toStdString().c_str();
    ;
    atl::String string6 = prName.toStdString().c_str();
    atl::String string7 = offsetFilename.toStdString().c_str();
    atl::String string8 = searchHoleFilename.toStdString().c_str();

    atl::Conditional<avl::SegmentFittingField> segmentFittingField1;
    atl::Conditional<avl::SegmentFittingField> segmentFittingField2;
    atl::Conditional<avl::SegmentFittingField> segmentFittingField3;
    atl::Conditional<avl::SegmentFittingField> segmentFittingField4;
    atl::Conditional<avl::GrayModel> grayModel1;
    atl::Conditional<avl::Vector2D> vector2D1;
    atl::Conditional<avl::CircleFittingField> circleFittingField1;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState1;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState2;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState3;
    avs::FitSegmentToEdgesState fitSegmentToEdgesState4;
    atl::Conditional<atl::String> string9;
    atl::String string10;
    atl::Array<avl::Point2D> point2DArray1;
    avs::FitCircleToEdgesState fitCircleToEdgesState1;
    atl::String string11;
    atl::Conditional<avl::Rectangle2D> rectangle2D1;
    atl::Conditional<avl::Point2D> point2D1;
    atl::Conditional<avl::Point2D> point2D2;
    atl::Conditional<avl::Circle2D> circle2D1;
    avl::Image image2;
    avl::Image image3;
    avl::Image image4;
    avl::Image image5;
    atl::Array<atl::Conditional<atl::String>> stringArray1;
    avl::Image image6;
    avl::Region region1;
    atl::Array<avl::Region> regionArray1;
    atl::Array<avl::Region> regionArray2;
    atl::Array<float> realArray1;
    avl::Region region2;
    double x_resize_ratio = 0;
    double y_resize_ratio = 0;
    try
    {
        avl::Image image0;
        // image = image.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput(image.width(), image.height(), image.bytesPerLine(), avl::PlainType::Type::UInt8, image.depth() / 8, image.bits());
        avl::RealignImagePitch(imageInput, image0);
        avl::Image image1;
        if (image.width() > 2000)
        {
            avl::ResizeImage(image0, image.width() / 2, image.height() / 2, avl::ResizeMethod::NearestNeighbour, image1);
        }
        else
        {
            avl::ResizeImage(image0, image.width(), image.height(), avl::ResizeMethod::NearestNeighbour, image1);
        }
        x_resize_ratio = image.width() / image1.Width();
        y_resize_ratio = image.height() / image1.Height();

        avs::LoadObject<atl::Conditional<avl::SegmentFittingField>>(string2, avl::StreamMode::Binary, g_constData11, segmentFittingField1);
        avs::LoadObject<atl::Conditional<avl::SegmentFittingField>>(string3, avl::StreamMode::Binary, g_constData11, segmentFittingField2);
        avs::LoadObject<atl::Conditional<avl::SegmentFittingField>>(string4, avl::StreamMode::Binary, g_constData11, segmentFittingField3);
        avs::LoadObject<atl::Conditional<avl::SegmentFittingField>>(string5, avl::StreamMode::Binary, g_constData11, segmentFittingField4);

        avs::LoadObject<atl::Conditional<avl::GrayModel>>(string6, avl::StreamMode::Binary, g_constData12, grayModel1);
        avs::LoadObject<atl::Conditional<avl::Vector2D>>(string7, avl::StreamMode::Binary, g_constData13, vector2D1);
        avs::LoadObject<avl::Region>(searchRegionFilename.toStdString().c_str(), avl::StreamMode::Binary, L"Region", region1);

        bool circleFittingFieldFileExist = false;
        avl::TestFileExists(string8, circleFittingFieldFileExist);
        if (circleFittingFieldFileExist)
        {
            avs::LoadObject<atl::Conditional<avl::CircleFittingField>>(string8, avl::StreamMode::Binary, g_constData14, circleFittingField1);
        }

        if (grayModel1 != atl::NIL)
        {
            atl::Conditional<avl::Object2D> object2D1;

            avl::SplitRegionIntoBlobs(region1, avl::RegionConnectivity::EightDirections, 1, atl::NIL, false, regionArray1,
                                      atl::Dummy<atl::Array<int>>().Get());
            avl::SortRegions(regionArray1, avl::RegionFeature::MassCenterX, avl::SortingOrder::Ascending, regionArray2, realArray1);
            avl::GetNthElement<avl::Region>(regionArray2, realArray1, selectedCol, avl::SortingOrder::Ascending, region2, atl::Dummy<float>().Get(),
                                            atl::Dummy<int>().Get());
            avl::LocateSingleObject_NCC(image1, region2, grayModel1.Get(), 0, 3, false, 0.3f, object2D1, atl::NIL,
                                        atl::Dummy<atl::Array<avl::Image>>().Get(), atl::Dummy<atl::Array<avl::Image>>().Get(),
                                        atl::Dummy<atl::Conditional<atl::Array<float>>>().Get());

            if (object2D1 != atl::NIL)
            {
                // Check score
                qInfo("Score: %f", object2D1.Get().Score());
                if (object2D1.Get().score < objectScore)
                {
                    qWarning("Object score does not pass %f < %f", object2D1.Get().score, objectScore);
                    return false;
                }
                avl::Point2D mark;
                avl::CoordinateSystem2D coordinateSystem2D1;
                atl::Conditional<avl::Line2D> line2D1;
                atl::Conditional<avl::Line2D> line2D2;
                atl::Conditional<avl::Point2D> point2D3;
                atl::Conditional<avl::Line2D> line2D3;
                atl::Conditional<avl::Point2D> point2D4;
                float real1;
                mark = object2D1.Get().Point();
                real1 = object2D1.Get().Angle();
                qInfo("Mark: %f %f angle: %f", object2D1.Get().Point().X(), object2D1.Get().Point().Y(), real1);
                avl::CreateCoordinateSystemFromPoint(mark, real1, 1.0f, 1.0f, coordinateSystem2D1);
                if (segmentFittingField2 != atl::NIL)
                {
                    atl::Conditional<avl::Segment2D> segment2D1;

                    avs::AvsFilter_FitSegmentToEdges(
                        fitSegmentToEdgesState1, image1, segmentFittingField2.Get(), coordinateSystem2D1, 100, 3, avl::InterpolationMethod::Bilinear,
                        avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best,
                        atl::NIL, 0.1f, avl::MEstimator::Type::Tukey, segment2D1, atl::NIL, atl::NIL, atl::NIL,
                        atl::Dummy<atl::Array<avl::Segment2D>>().Get(), atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(),
                        atl::Dummy<atl::Array<avl::Profile>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get());

                    if (segment2D1 != atl::NIL)
                    {
                        line2D1.AssignNonNil();

                        avl::Segment2DToLine2D(segment2D1.Get(), line2D1.Get());
                    }
                    else
                    {
                        line2D1 = atl::NIL;
                    }
                }
                else
                {
                    line2D1 = atl::NIL;
                }

                if (segmentFittingField1 != atl::NIL)
                {
                    atl::Conditional<avl::Segment2D> segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and
                    // FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges(
                        fitSegmentToEdgesState2, image1, segmentFittingField1.Get(), coordinateSystem2D1, 100, 3, avl::InterpolationMethod::Bilinear,
                        avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best,
                        atl::NIL, 0.1f, avl::MEstimator::Type::Tukey, segment2D1, atl::NIL, atl::NIL, atl::NIL,
                        atl::Dummy<atl::Array<avl::Segment2D>>().Get(), atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(),
                        atl::Dummy<atl::Array<avl::Profile>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get());

                    if (segment2D1 != atl::NIL)
                    {
                        line2D2.AssignNonNil();

                        avl::Segment2DToLine2D(segment2D1.Get(), line2D2.Get());

                        if (line2D1 != atl::NIL)
                        {
                            avl::LineLineIntersection(line2D2.Get(), line2D1.Get(), point2D3);
                        }
                        else
                        {
                            point2D3 = atl::NIL;
                        }
                    }
                    else
                    {
                        line2D2 = atl::NIL;
                        point2D3 = atl::NIL;
                    }
                }
                else
                {
                    line2D2 = atl::NIL;
                    point2D3 = atl::NIL;
                }

                if (segmentFittingField3 != atl::NIL)
                {
                    atl::Conditional<avl::Segment2D> segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and
                    // FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges(
                        fitSegmentToEdgesState3, image1, segmentFittingField3.Get(), coordinateSystem2D1, 100, 3, avl::InterpolationMethod::Bilinear,
                        avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best,
                        atl::NIL, 0.1f, avl::MEstimator::Type::Tukey, segment2D1, atl::NIL, atl::NIL, atl::NIL,
                        atl::Dummy<atl::Array<avl::Segment2D>>().Get(), atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(),
                        atl::Dummy<atl::Array<avl::Profile>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get());

                    if (segment2D1 != atl::NIL)
                    {
                        line2D3.AssignNonNil();

                        avl::Segment2DToLine2D(segment2D1.Get(), line2D3.Get());

                        if (line2D1 != atl::NIL)
                        {
                            avl::LineLineIntersection(line2D1.Get(), line2D3.Get(), point2D4);
                        }
                        else
                        {
                            point2D4 = atl::NIL;
                        }
                    }
                    else
                    {
                        line2D3 = atl::NIL;
                        point2D4 = atl::NIL;
                    }
                }
                else
                {
                    line2D3 = atl::NIL;
                    point2D4 = atl::NIL;
                }

                if (segmentFittingField4 != atl::NIL)
                {
                    atl::Conditional<avl::Segment2D> segment2D1;

                    // Function AvsFilter_FitSegmentToEdges is intended for generated code only. Consider use of CreateFittingMap and
                    // FitSegmentToEdges functions in regular programs.
                    avs::AvsFilter_FitSegmentToEdges(
                        fitSegmentToEdgesState4, image1, segmentFittingField4.Get(), coordinateSystem2D1, 100, 3, avl::InterpolationMethod::Bilinear,
                        avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 1.0f, avl::EdgeTransition::Any), avl::Selection::Best,
                        atl::NIL, 0.1f, avl::MEstimator::Type::Tukey, segment2D1, atl::NIL, atl::NIL, atl::NIL,
                        atl::Dummy<atl::Array<avl::Segment2D>>().Get(), atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(),
                        atl::Dummy<atl::Array<avl::Profile>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get());

                    if (segment2D1 != atl::NIL && line2D2 != atl::NIL && line2D3 != atl::NIL)
                    {
                        avl::Line2D line2D4;
                        atl::Conditional<avl::Point2D> point2D5;
                        atl::Conditional<avl::Point2D> point2D6;

                        avl::Segment2DToLine2D(segment2D1.Get(), line2D4);
                        avl::LineLineIntersection(line2D2.Get(), line2D4, point2D5);
                        avl::LineLineIntersection(line2D3.Get(), line2D4, point2D6);

                        if (outEdgeTransition)
                        {
                            edgeTransition = EdgeTransition(edgeTransitionState1, point2D4.Get(), point2D5.Get(), image1);
                        }

                        if (point2D3 != atl::NIL && point2D4 != atl::NIL && point2D6 != atl::NIL && point2D5 != atl::NIL)
                        {
                            float real1;

                            rectangle2D1.AssignNonNil();
                            point2D1.AssignNonNil();
                            string9.AssignNonNil();

                            real1 = object2D1.Get().Score();
                            avl::RealToString(real1, string10);

                            // AvsFilter_CreateArray is intended for use in generated code only. Consider use of proper constructor or Array::Clear()
                            // and Array::Reserve function in hand-written programs.
                            avs::AvsFilter_CreateArray<avl::Point2D>(point2D3.Get(), point2D4.Get(), point2D6.Get(), point2D5.Get(), atl::NIL,
                                                                     atl::NIL, atl::NIL, atl::NIL, point2DArray1);
                            avl::PointsBoundingRectangle(point2DArray1, avl::BoundingRectangleFeature::MinimalArea, 0.0f,
                                                         avl::RectangleOrientation::Vertical, rectangle2D1.Get(), point2D1.Get(), atl::NIL, atl::NIL);

                            if (vector2D1 != atl::NIL)
                            {
                                point2D2.AssignNonNil();

                                avl::TranslatePoint(point2D1.Get(), vector2D1.Get(), false, point2D2.Get());
                            }
                            else
                            {
                                point2D2 = atl::NIL;
                            }

                            avl::Rectangle2D rectangle2D2;
                            float real3;
                            avl::CoordinateSystem2D coordinateSystem2D2;

                            avl::NormalizeRectangleOrientation(rectangle2D1.Get(), 0.0f, avl::RectangleOrientation::Vertical, rectangle2D2);
                            real3 = rectangle2D2.Angle();
                            avl::RotatePoint(point2D2.Get(), point2D1.Get(), real3, false, point2D2.Get());
                            prResult.theta = real3;
                            avl::RealToString(real3, string11);

                            if (circleFittingField1 != atl::NIL)
                            {
                                coordinateSystem2D2 = avl::CoordinateSystem2D(point2D1.Get(), real3, 1.0f);

                                avs::AvsFilter_FitCircleToEdges(
                                    fitCircleToEdgesState1, image1, circleFittingField1.Get(), coordinateSystem2D2, 10, 5,
                                    avl::InterpolationMethod::Bilinear,
                                    avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::Any),
                                    avl::Selection::Best, atl::NIL, 0.1f, avl::CircleFittingMethod::AlgebraicTaubin, atl::NIL, circle2D1, atl::NIL,
                                    atl::NIL, atl::NIL, atl::Dummy<atl::Array<avl::Segment2D>>().Get(),
                                    atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get(),
                                    atl::Dummy<atl::Array<avl::Profile>>().Get());
                            }
                            else
                            {
                                circle2D1 = atl::NIL;
                            }

                            avs::AvsFilter_ConcatenateStrings(g_constData15, string11, g_constData16, string10, g_emptyString, g_emptyString,
                                                              g_emptyString, g_emptyString, string9.Get());
                        }
                        else
                        {
                            rectangle2D1 = atl::NIL;
                            point2D1 = atl::NIL;
                            string9 = atl::NIL;
                            point2D2 = atl::NIL;
                            circle2D1 = atl::NIL;
                        }
                    }
                    else
                    {
                        rectangle2D1 = atl::NIL;
                        point2D1 = atl::NIL;
                        point2D2 = atl::NIL;
                        circle2D1 = atl::NIL;
                        string9 = atl::NIL;
                    }
                }
                else
                {
                    rectangle2D1 = atl::NIL;
                    point2D1 = atl::NIL;
                    point2D2 = atl::NIL;
                    circle2D1 = atl::NIL;
                    string9 = atl::NIL;
                }
            }
            else
            {
                rectangle2D1 = atl::NIL;
                point2D1 = atl::NIL;
                point2D2 = atl::NIL;
                circle2D1 = atl::NIL;
                string9 = atl::NIL;
            }
        }
        else
        {
            rectangle2D1 = atl::NIL;
            point2D1 = atl::NIL;
            point2D2 = atl::NIL;
            circle2D1 = atl::NIL;
            string9 = atl::NIL;
        }

        if (point2D1 != atl::NIL)
        {
            prResult.ori_x = point2D1.Get().X() * x_resize_ratio;
            prResult.ori_y = point2D1.Get().Y() * y_resize_ratio;
        }
        else
        {
            qWarning("Cannot find target point 1");
            return false;
        }

        if (point2D2 != atl::NIL)
        {
            prResult.x = point2D2.Get().X() * x_resize_ratio;
            prResult.y = point2D2.Get().Y() * y_resize_ratio;
        }
        else
        {
            qWarning("Cannot find target point 2");
            return false;
        }

        if (circle2D1 != atl::NIL)
        {
            qInfo("Detected small hole at x: %f y: %f", circle2D1.Get().Center().X(), circle2D1.Get().Center().Y());
        }
        else
        {
            if (detect_small_hole)
            {
                qWarning("Cannot find the small hole");
            }
        }

        qInfo("Finish Calculation: %d", timer.elapsed());
        timer.restart();

        prResultImageInfo->rect_x = rectangle2D1.Get().X() * x_resize_ratio;
        prResultImageInfo->rect_y = rectangle2D1.Get().Y() * y_resize_ratio;

        prResultImageInfo->rect_angle = rectangle2D1.Get().Angle();

        prResultImageInfo->rect_ori_x = rectangle2D1.Get().Origin().X() * x_resize_ratio;
        prResultImageInfo->rect_ori_y = rectangle2D1.Get().Origin().Y() * y_resize_ratio;

        prResultImageInfo->rect_width = rectangle2D1.Get().Width() * x_resize_ratio;
        prResultImageInfo->rect_height = rectangle2D1.Get().Height() * y_resize_ratio;

        prResultImageInfo->result_ori_x = point2D1.Get().X() * x_resize_ratio;
        prResultImageInfo->result_ori_y = point2D1.Get().Y() * y_resize_ratio;

        prResultImageInfo->result_x = point2D2.Get().X() * x_resize_ratio;
        prResultImageInfo->result_y = point2D2.Get().Y() * y_resize_ratio;

        prResultImageInfo->resultString = QString("Angle: ").append(QString::number(prResult.theta));
    }
    catch (const atl::Error &error)
    {
        qWarning("Throw Error: %s", error.Message());
        return false;
    }
    return true;
}
