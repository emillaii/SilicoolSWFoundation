#include "genericncctemplatematching.h"
#include "AVL.h"
#include "STD.h"
#include "commonutils.h"

GenericNCCTemplateMatching::GenericNCCTemplateMatching() {}

bool GenericNCCTemplateMatching::PR_Generic_NCC_Template_Matching(
    QImage image, QString prName, PRResultStruct &prResult, QVariantMap args, AdaptVisionPRResultImageInfo *prResultImageInfo)
{
    QElapsedTimer timer;
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

    QString offsetFilename = prName;
    QString searchHoleFilename = prName;
    QString searchRegionFilename = prName;

    // offsetFilename.replace("_edgeModel", "_offset");
    // searchHoleFilename.replace("_edgeModel", "_searchHole");
    searchRegionFilename.replace("_templateModel", "_searchRegion");

    g_emptyString = L"";
    g_constData11 = L"SegmentFittingField?";
    g_constData12 = L"GrayModel?";
    g_constData13 = L"Vector2D?";
    g_constData14 = L"CircleFittingField?";
    g_constData15 = L"Angle: ";
    g_constData16 = L" Score:";

    atl::String string1;
    atl::String string6 = prName.toStdString().c_str();
    atl::String string7 = offsetFilename.toStdString().c_str();
    atl::String string8 = searchHoleFilename.toStdString().c_str();

    atl::Conditional<avl::GrayModel> grayModel1;
    atl::Conditional<avl::Vector2D> vector2D1;
    atl::Conditional<avl::CircleFittingField> circleFittingField1;

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

        avs::LoadObject<atl::Conditional<avl::GrayModel>>(prName.toStdString().c_str(), avl::StreamMode::Binary, L"GrayModel?", grayModel1);
        avs::LoadObject<avl::Region>(searchRegionFilename.toStdString().c_str(), avl::StreamMode::Binary, L"Region", region1);

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
            // Check score
            qInfo("Score: %f", object2D1.Get().Score());
            if (object2D1.Get().score < objectScore)
            {
                qWarning("Object score does not pass %f < %f", object2D1.Get().score, objectScore);
                return false;
            }
            qInfo("Finish Calculation: %d", timer.elapsed());
            timer.restart();

            prResult.theta = object2D1.Get().Angle();
            prResult.x = object2D1.Get().Point().X() * x_resize_ratio;
            prResult.y = object2D1.Get().Point().Y() * y_resize_ratio;

            prResult.ori_x = object2D1.Get().Point().X() * x_resize_ratio;
            prResult.ori_y = object2D1.Get().Point().Y() * y_resize_ratio;

            prResultImageInfo->rect_angle = object2D1.Get().Angle();

            prResultImageInfo->rect_ori_x = object2D1.Get().Point().X() * x_resize_ratio;
            prResultImageInfo->rect_ori_y = object2D1.Get().Point().Y() * y_resize_ratio;

            prResultImageInfo->result_ori_x = object2D1.Get().Point().X() * x_resize_ratio;
            prResultImageInfo->result_ori_y = object2D1.Get().Point().Y() * y_resize_ratio;

            prResultImageInfo->result_x = object2D1.Get().Point().X() * x_resize_ratio;
            prResultImageInfo->result_y = object2D1.Get().Point().Y() * y_resize_ratio;

            prResultImageInfo->resultString = QString("Angle: ").append(QString::number(prResult.theta));
        }
        else
        {
            qWarning("Object not found");
            return false;
        }
    }
    catch (const atl::Error &error)
    {
        qWarning("Throw Error: %s", error.Message());
        return false;
    }
    return true;
}
