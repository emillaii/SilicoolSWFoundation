#include "adaptvision.h"
#include "circleedgefitting.h"
#include "commonutils.h"
#include "edgefitting.h"
#include "genericncctemplatematching.h"
#include "glueinspection.h"

AdaptVision::AdaptVision() {}

bool AdaptVision::checkDongle()
{
    try
    {
        atl::Array<avl::Point2D> g_constData1;
        g_constData1.Reset(2);
        g_constData1[0] = avl::Point2D(0.0f, 0.0f);
        g_constData1[1] = avl::Point2D(399.0f, 412.0f);
        avl::Path path1;
        atl::Array<avl::Point2D> point2DArray1;

        avs::AvsFilter_MakePath(g_constData1, false, path1);

        avs::AvsFilter_AccessPath(path1, point2DArray1, atl::Dummy<bool>().Get());
    }
    catch (const atl::Error &error)
    {
        qWarning("Throw Error: %s", error.Message());
        return false;
    }
    return true;
}

bool AdaptVision::performPr(QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult)
{
    return performPr(image, prConfig, resultImageInfo, prResult, 0, 0);
}

bool AdaptVision::performPr(
    QImage &image, VisionLocationConfig *prConfig, PRResultImageInfo **resultImageInfo, PRResultStruct &prResult, int roiRowIndex, int rowColIndex)
{
    auto adPrConfig = qobject_cast<AdaptVisionLocationConfig *>(prConfig);
    if (adPrConfig == nullptr)
    {
        qFatal("Can not cast VisionLocationConfig to AdaptVisionLocationConfig! Source type name: %s", prConfig->metaObject()->className());
    }
    QString prFileName = VisionConfigDir::getIns().prConfigDir() + adPrConfig->prFileName();
    qInfo("Perform PR: %s", prFileName.toUtf8().data());

    auto adPrResultImageInfo = new AdaptVisionPRResultImageInfo();
    (*resultImageInfo) = adPrResultImageInfo;
    QVariantMap args;

    adPrResultImageInfo->prType = adPrConfig->prType();
    if (adPrConfig->prType() == AdaptVisionLocationConfig::Generic_NCC_Template_Matching)
    {
        qInfo("Going to perform NCC Template Mataching");
        args["col"] = rowColIndex;
        args["objectScore"] = adPrConfig->objectScore();
        return GenericNCCTemplateMatching::PR_Generic_NCC_Template_Matching(image, prFileName, prResult, args, adPrResultImageInfo);
    }
    else if (adPrConfig->prType() == AdaptVisionLocationConfig::Edge_Template_Matching)
    {
        qInfo("Going to perform edge fitting");
        args["col"] = rowColIndex;
        args["objectScore"] = adPrConfig->objectScore();
        double t;
        return EdgeFitting::PR_Edge_Fitting(image, prFileName, prResult, args, adPrResultImageInfo, t, false);
    }
    else if (adPrConfig->prType() == AdaptVisionLocationConfig::Circle_Edge_Template_Matching)
    {
        qInfo("Going to perform circle edge fitting");
        return CircleEdgeFitting::PR_Circle_Edge_Fitting(image, prFileName, prResult, args, adPrResultImageInfo);
    }
    qWarning("Invalid input pr type");
    return false;
}

void AdaptVision::drawResultImage(QImage &image, PRResultImageInfo *resultImageInfo)
{
    auto adPrResultImageInfo = qobject_cast<AdaptVisionPRResultImageInfo *>(resultImageInfo);
    if (adPrResultImageInfo == nullptr)
    {
        qCritical("Empty PRResultImageInfo!");
        return;
    }

    qInfo("Draw Result Image");
    int image_width = image.width();
    int image_height = image.height();
    float point_size = sqrt(image_width * image_height) / 10;

    if (adPrResultImageInfo->prType == AdaptVisionLocationConfig::Circle_Edge_Template_Matching)
    {
        qInfo("Circle_Edge_Template_Matching");
        avl::Image image1;
        avl::Image image2;
        avl::Image image3;
        avl::Image image4;

        avl::Point2D point2D1;
        avl::Point2D point2D2;

        point2D1 = avl::Point2D((int)adPrResultImageInfo->result_x, (int)adPrResultImageInfo->result_y);
        point2D2 = avl::Point2D((int)adPrResultImageInfo->result_ori_x, (int)adPrResultImageInfo->result_ori_y);

        avl::Circle2D circle2D2;

        avl::CreateCircle(point2D1, avl::Anchor2D::MiddleCenter, (float)adPrResultImageInfo->radius, circle2D2);

        if (image.depth() != 8)
            image = image.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput(image.width(), image.height(), image.bytesPerLine(), avl::PlainType::Type::UInt8, image.depth() / 8, image.bits());
        avl::RealignImagePitch(imageInput, image1);

        avs::DrawCircles_SingleColor(image1, atl::ToArray<atl::Conditional<avl::Circle2D>>(circle2D2), atl::NIL,
                                     avl::Pixel(255.0f, 255.0f, 0.0f, 0.0f),
                                     avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, 1.0f), true, image2);
        avs::DrawPoints_SingleColor(image2, atl::ToArray<atl::Conditional<avl::Point2D>>(point2D1), atl::NIL, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f),
                                    avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 5.0f, false, avl::PointShape::Cross, point_size), true,
                                    image3);

        QImage testImage((uchar *)image3.Data(), image3.Width(), image3.Height(), image3.Pitch(), QImage::Format_RGB888);
        image = testImage.copy();
    }
    else
    {

        atl::Array<atl::Conditional<atl::String>> g_constData2;
        g_constData2.Reset(1);
        g_constData2[0] = adPrResultImageInfo->resultString.toStdString().c_str();
        atl::Array<atl::Conditional<avl::Location>> g_constData3;

        g_constData3.Reset(1);
        g_constData3[0] = avl::Location(0, 0);
        avl::Rectangle2D rectangle2D1;
        avl::Point2D point2D1;
        avl::Point2D point2D2;
        avl::Image image1;
        avl::Image image2;
        avl::Image image3;
        avl::Image image4;
        avl::Image image5;
        if (image.depth() != 8)
            image = image.convertToFormat(QImage::Format_RGB888);
        avl::Image imageInput(image.width(), image.height(), image.bytesPerLine(), avl::PlainType::Type::UInt8, image.depth() / 8, image.bits());
        avl::RealignImagePitch(imageInput, image1);
        avl::CreateRectangle(avl::Point2D((int)adPrResultImageInfo->rect_x, (int)adPrResultImageInfo->rect_y), avl::Anchor2D::TopLeft,
                             adPrResultImageInfo->rect_angle, adPrResultImageInfo->rect_width, adPrResultImageInfo->rect_height, rectangle2D1);
        point2D1 = avl::Point2D((int)adPrResultImageInfo->result_x, (int)adPrResultImageInfo->result_y);
        point2D2 = avl::Point2D((int)adPrResultImageInfo->result_ori_x, (int)adPrResultImageInfo->result_ori_y);
        avs::DrawRectangles_SingleColor(image1, atl::ToArray<atl::Conditional<avl::Rectangle2D>>(rectangle2D1), atl::NIL,
                                        avl::Pixel(255.0f, 1.0f, 255.0f, 0.0f),
                                        avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 9.0f, false, atl::NIL, 1.0f), true, image2);
        avs::DrawPoints_SingleColor(image2, atl::ToArray<atl::Conditional<avl::Point2D>>(point2D1), atl::NIL, avl::Pixel(255.0f, 128.0f, 0.0f, 0.0f),
                                    avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 10.0f, false, avl::PointShape::Cross, point_size), true,
                                    image3);
        avs::DrawPoints_SingleColor(image3, atl::ToArray<atl::Conditional<avl::Point2D>>(point2D2), atl::NIL, avl::Pixel(0.0f, 255.0f, 0.0f, 0.0f),
                                    avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 10.0f, false, avl::PointShape::Cross, point_size), true,
                                    image4);
        // avs::DrawStrings_SingleColor( image4, g_constData2, g_constData3, atl::NIL, avl::Anchor2D::TopLeft, avl::Pixel(0.0f, 255.0f, 0.0f,
        // 0.0f), avl::DrawingStyle(avl::DrawingMode::HighQuality, 1.0f, 1.0f, false, atl::NIL, point_size), point_size, 0.0f, true, atl::NIL,
        // image5 );
        QImage testImage((uchar *)image4.Data(), image4.Width(), image4.Height(), image4.Pitch(), QImage::Format_RGB888);
        image = testImage.copy();
    }
}

QVector<PathEndPoint> AdaptVision::readDispensePath(QString fileName)
{
    QVector<PathEndPoint> result;
    atl::String g_constData1 = fileName.toStdString().c_str();
    atl::String g_constData2 = L"PathArray";

    atl::Array<avl::Path> pathArray1;
    atl::Array<avl::Point2D> point2DArray1;

    if (!checkFileExist(fileName))
        return result;

    avs::LoadObject<atl::Array<avl::Path>>(g_constData1, avl::StreamMode::Binary, g_constData2, pathArray1);

    try
    {
        for (int i = 0; i < pathArray1.Size(); ++i)
        {
            avs::AvsFilter_AccessPath(pathArray1[i], point2DArray1, atl::Dummy<bool>().Get());
            for (int j = 0; j < point2DArray1.Size(); ++j)
            {
                float x = point2DArray1[j].x;
                float y = point2DArray1[j].y;
                result.push_back(PathEndPoint(x, y));
            }
        }
    }
    catch (const atl::Error &error)
    {
        qWarning("Throw Error: %s", error.Message());
        return result;
    }
    return result;
}
