#include "circleedgefitting.h"
#include <QElapsedTimer>

CircleEdgeFitting::CircleEdgeFitting() {}

bool CircleEdgeFitting::PR_Circle_Edge_Fitting(
    QImage image, QString prName, PRResultStruct &prResult, QVariantMap args, AdaptVisionPRResultImageInfo *prResultImageInfo)
{
    qInfo("PR_Circle_Edge_Fitting");

    atl::String g_constData1;
    atl::String g_constData2;
    atl::String g_constData3;
    atl::String g_constData4;
    atl::String g_constData5;
    atl::String g_constData6;
    atl::String g_emptyString;
    atl::String g_constData7;
    atl::String g_constData8;
    atl::String g_constData9;
    atl::String g_constData10;
    atl::String g_constData11;
    atl::String g_constData12;

    QString grayModelFilename = prName;
    QString circleFittingFieldFilename = prName;
    QString regionFilename = prName;

    g_constData3 = L"GrayModel";
    g_constData5 = L"CircleFittingField";
    avl::Region region1;
    avl::Image image2;
    avl::GrayModel grayModel1;
    avl::Image image3;
    atl::Conditional<atl::Array<atl::Conditional<avl::Point2D>>> point2DArray1;
    avs::FitCircleToEdgesState fitCircleToEdgesState1;
    atl::Array<atl::Conditional<avl::Edge1D>> edge1DArray1;
    avl::Image image4;
    avl::Image image5;
    avl::Image image6;
    avl::Image image7;

    QString edgeFittingFieldFilename = prName;
    QString searchRegionFilename = prName;

    edgeFittingFieldFilename.replace("_circleEdgeFittingModel", "_circleEdgeFittingField");
    searchRegionFilename.replace("_circleEdgeFittingModel", "_circleEdgeFittingRegion");

    QElapsedTimer timer;
    timer.start();

    double x_resize_ratio = 0;
    double y_resize_ratio = 0;
    try
    {
        // if (image.depth() != 8)
        //     image = image.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput(image.width(), image.height(), image.bytesPerLine(), avl::PlainType::Type::UInt8, image.depth() / 8, image.bits());
        avl::RealignImagePitch(imageInput, image2);

        avl::CircleFittingField circleFittingField1;
        int integer1;
        int integer2;
        float real1;
        float real2;
        atl::Conditional<avl::Object2D> object2D1;
        atl::Conditional<avl::Point2D> point2D1;
        atl::Conditional<avl::Circle2D> circle2D1;
        atl::Conditional<avl::Point2D> point2D2;

        avs::LoadObject<avl::Region>(searchRegionFilename.toStdString().c_str(), avl::StreamMode::Binary, L"Region", region1);
        avs::LoadObject<avl::GrayModel>(prName.toStdString().c_str(), avl::StreamMode::Binary, g_constData3, grayModel1);
        avs::LoadObject<avl::CircleFittingField>(edgeFittingFieldFilename.toStdString().c_str(), avl::StreamMode::Binary, g_constData5,
                                                 circleFittingField1);

        qWarning("Load Object finished");
        integer1 = imageInput.Width();
        integer2 = imageInput.Height();

        if (image.width() > 2000)
        {
            avl::ResizeImage(image2, image.width() / 2, image.height() / 2, avl::ResizeMethod::NearestNeighbour, image3);
        }
        else
        {
            avl::ResizeImage(image2, image.width(), image.height(), avl::ResizeMethod::NearestNeighbour, image3);
        }

        x_resize_ratio = imageInput.Width() / image3.Width();
        y_resize_ratio = imageInput.Height() / image3.Height();

        timer.restart();
        avl::LocateSingleObject_NCC(image3, region1, grayModel1, 0, 3, false, 0.3f, object2D1, atl::NIL, atl::Dummy<atl::Array<avl::Image>>().Get(),
                                    atl::Dummy<atl::Array<avl::Image>>().Get(), atl::Dummy<atl::Conditional<atl::Array<float>>>().Get());
        qInfo("LocateSingleObject_NCC timeElapsed: %d", timer.elapsed());

        qWarning("LocateSingleObject_NCC finished");
        if (object2D1 != atl::NIL)
        {
            avl::CoordinateSystem2D coordinateSystem2D1;
            point2D1.AssignNonNil();
            point2DArray1.AssignNonNil();
            point2D1.Get() = object2D1.Get().Point();
            atl::Conditional<avl::Point2D> point2D3;
            point2D3.AssignNonNil();
            point2D3.Get().x = object2D1.Get().Point().X() * 2;
            point2D3.Get().y = object2D1.Get().Point().Y() * 2;
            coordinateSystem2D1 = avl::CoordinateSystem2D(point2D3.Get(), 0.0f, 1.0f);
            timer.restart();
            avs::AvsFilter_FitCircleToEdges(
                fitCircleToEdgesState1, imageInput, circleFittingField1, coordinateSystem2D1, 100, 5, avl::InterpolationMethod::Bilinear,
                avl::EdgeScanParams(avl::ProfileInterpolationMethod::Quadratic4, 1.0f, 5.0f, avl::EdgeTransition::BrightToDark),
                avl::Selection::First, atl::NIL, 0.1f, avl::CircleFittingMethod::AlgebraicTaubin, atl::NIL, circle2D1, edge1DArray1, atl::NIL,
                atl::NIL, atl::Dummy<atl::Array<avl::Segment2D>>().Get(), atl::Dummy<atl::Array<avl::Rectangle2D>>().Get(),
                atl::Dummy<atl::Array<avl::Profile>>().Get(), atl::Dummy<atl::Array<avl::Profile>>().Get());
            qInfo("AvsFilter_FitCircleToEdges timeElapsed: %d", timer.elapsed());

            qWarning("AvsFilter_FitCircleToEdges finished");

            point2DArray1.Get().Resize(edge1DArray1.Size());
            if (circle2D1 != atl::NIL)
            {
                point2D2.AssignNonNil();

                point2D2.Get() = circle2D1.Get().Center();
            }
            else
            {
                point2D2 = atl::NIL;
            }
        }
        else
        {
            qWarning("Cannot find object");
            return false;
        }
        prResult.x = circle2D1.Get().Center().X();
        prResult.y = circle2D1.Get().Center().Y();

        prResult.ori_x = circle2D1.Get().Center().X();
        prResult.ori_y = circle2D1.Get().Center().Y();

        prResultImageInfo->result_x = prResult.x;
        prResultImageInfo->result_y = prResult.y;
        prResultImageInfo->result_ori_x = prResult.ori_x;
        prResultImageInfo->result_ori_y = prResult.ori_y;

        prResultImageInfo->radius = circle2D1.Get().Radius();
        return true;
    }
    catch (const atl::Error &error)
    {
        qWarning("Throw Error: %s", error.Message());
        return false;
    }
}
