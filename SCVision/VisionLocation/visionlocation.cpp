#include "visionlocation.h"
#include <QThread>
#include <qelapsedtimer.h>

#define PI 3.1415926535898

VisionLocation::VisionLocation(SCVision *vision, SCCamera *camera, VisionLocationConfig *config) : vision(vision), m_camera(camera), m_config(config)
{
    connect(config, &VisionLocationConfig::lightBrightnessChanged, this, &VisionLocation::onVisionLocationConfigLightBrightnessChanged,
            Qt::DirectConnection);
    connect(config, &VisionLocationConfig::secondLightBrightnessChanged, this, &VisionLocation::onVisionLocationConfigSecondLightBrightnessChanged,
            Qt::DirectConnection);
}

void VisionLocation::setCalibrationResult(CalibrationResult *calibrationResult)
{
    this->calibration = calibrationResult;
}

bool VisionLocation::performPR(PrOffset &offset)
{
    QImage img = getImage();
    return performPR(img, offset);
}

bool VisionLocation::performPR(PrOffset &offset, int roiRowIndex, int roiColIndex)
{
    QImage image = getImage();
    PRResultStruct prResult;
    offset.ReSet();

    if (isDisabled)
    {
        showPrImage(image);
        return true;
    }

    PRResultImageInfo *resultImageInfo = nullptr;
    bool res = vision->performPr(image, m_config, &resultImageInfo, prResult, roiRowIndex, roiColIndex);
    reverseTheta(prResult);

    if (res)
    {
        qCInfo(visionCate()) << m_config->locationName() << "pr result:" << prResult.toString();
        bool res = result2Offset(prResult, offset);
        QString imgProcessingRes;
        if (res)
        {
            imgProcessingRes = QString("Offset x: %1 y: %2 t: %3").arg(offset.X, 0, 'g', 4).arg(offset.Y, 0, 'g', 4).arg(offset.Theta, 0, 'g', 2);
        }
        drawSaveShowResultImage(image, resultImageInfo, imgProcessingRes);
        return res;
    }
    else
    {
        showPrImage(image);
        savePrFailedImage(image);
        qCInfo(visionCate()) << m_config->locationName() << "perform pr failed!";
        return false;
    }
}

void VisionLocation::startContinuallyPerformPr()
{
    if (isContinuallyCalculation)
    {
        return;
    }
    isContinuallyCalculation = true;
    AutoResetBool b(&isContinuallyCalculation, false);

    QString continuallyPrLogFileName
        = QString("./continuallyPrData/%1_%2.csv").arg(m_config->locationName()).arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz"));
    PrOffset prOffset;
    PRResultStruct prResult;
    int imgCount = 0;
    AutoResetProperty p1(m_config, "saveImageAfterGrab", m_config->saveImageAfterGrab());
    m_config->setSaveImageAfterGrab(false);
    AutoResetProperty p2(m_config, "saveResultImage", m_config->saveResultImage());
    m_config->setSaveResultImage(false);

    HighPrecisionTimer timer;
    double timeCost;

    while (isContinuallyCalculation)
    {
        QImage img = getImage();
        if (m_config->continuoullySaveImgInterval() > 0 && imgCount % m_config->continuoullySaveImgInterval() == 0)
        {
            camera()->saveImage(img);
        }
        imgCount++;
        if (imgCount >= 100 * m_config->continuoullySaveImgInterval())
        {
            imgCount = 0;
        }

        timer.start();
        performPR(img, prOffset, prResult);
        timeCost = timer.elapsedMs();

        QFile logFile(continuallyPrLogFileName);
        QDir dir("./continuallyPrData");
        if (!dir.exists())
        {
            dir.mkpath(dir.absolutePath());
        }
        bool fileExist = logFile.exists();

        logFile.open(QIODevice::Append);
        if (!fileExist)
        {
            QString title = "timeStamp,offsetX_mm,offsetY_mm,offsetTheta_degree,offsetX_pixel,offsetY_pixel,timecost\r\n";
            logFile.write(title.toUtf8());
        }
        QString line = QString("%1,%2,%3,%4,%5,%6,%7\r\n")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
                           .arg(prOffset.X)
                           .arg(prOffset.Y)
                           .arg(prOffset.Theta)
                           .arg(prResult.x)
                           .arg(prResult.y)
                           .arg(timeCost);
        logFile.write(line.toUtf8());
        logFile.close();

        if (m_config->continuoullyPrDelay() > 0)
        {
            QThread::msleep(m_config->continuoullyPrDelay());
        }
    }
}

void VisionLocation::stopContinuallyCalculation()
{
    isContinuallyCalculation = false;
}

bool VisionLocation::performPR(PRResultStruct &prResult)
{
    QImage image = getImage();
    return performPR(image, prResult);
}

bool VisionLocation::performPR(QImage &image, PRResultStruct &prResult)
{
    if (isDisabled)
    {
        showPrImage(image);
        return true;
    }

    PRResultImageInfo *resultImageInfo = nullptr;
    bool res = vision->performPr(image, m_config, &resultImageInfo, prResult);
    reverseTheta(prResult);

    if (res)
    {
        drawSaveShowResultImage(image, resultImageInfo);
        qCInfo(visionCate()) << m_config->locationName() << "pr result:" << prResult.toString();
    }
    else
    {
        savePrFailedImage(image);
        showPrImage(image);
        qCCritical(visionCate()) << m_config->locationName() << "perform pr failed!";
    }
    return res;
}

PrOffset VisionLocation::getAvePrOffset(int times)
{
    QList<PrOffset> offsets;
    return getAvePrOffset(times, offsets);
}

PrOffset VisionLocation::getAvePrOffset(int times, QList<PrOffset> &processData)
{
    PrOffset tmp;
    for (int i = 0; i < times; i++)
    {
        if (!performPR(tmp))
        {
            throw SilicolAbort(tr("%1 perform pr failed!").arg(m_config->locationName()));
        }
        processData.append(tmp);
    }
    return average(processData);
}

void VisionLocation::openLight()
{
    m_camera->setTwoChanndelBrightness(m_config->lightBrightness(), m_config->secondLightBrightness(), true);
}

void VisionLocation::closeLight()
{
    m_camera->setTwoChanndelBrightness(0, 0, false);
}

QImage VisionLocation::getImage()
{
    return ErrorHandler::tryToHandleGeneralError<QImage>([this] {
        openLight();
        if (m_config->waitImageDelay() > 0)
        {
            QThread::msleep(m_config->waitImageDelay());
        }
        QImage image = m_camera->getImage(false, m_config->saveImageAfterGrab());
        if (m_camera->config()->closeLightAfterPr())
        {
            closeLight();
        }
        return image;
    });
}

QPointF VisionLocation::getMechDistanceFromImageCenter(QPointF pixelPoint) const
{
    if (!calibration->isValid())
    {
        throw SilicolAbort(tr("%1 Pixel to mech failed: Invalid calibration!").arg(m_config->locationName()), EX_LOCATION);
    }
    return calibration->getDistanceFromImageCenter(pixelPoint.x(), pixelPoint.y(), m_camera->config());
}

bool VisionLocation::performPR(QImage &image, PrOffset &offset)
{
    PRResultStruct prResult;
    return performPR(image, offset, prResult);
}

bool VisionLocation::performPR(QImage &image, PrOffset &offset, PRResultStruct &prResult)
{
    offset.ReSet();

    if (isDisabled)
    {
        showPrImage(image);
        return true;
    }

    PRResultImageInfo *resultImageInfo = nullptr;
    bool res = vision->performPr(image, m_config, &resultImageInfo, prResult);
    reverseTheta(prResult);

    if (res)
    {
        qCInfo(visionCate()) << m_config->locationName() << "pr result:" << prResult.toString();
        bool res = result2Offset(prResult, offset);
        QString imgProcessingRes;
        if (res)
        {
            imgProcessingRes = QString("Offset x: %1 y: %2 t: %3").arg(offset.X, 0, 'g', 4).arg(offset.Y, 0, 'g', 4).arg(offset.Theta, 0, 'g', 2);
        }
        drawSaveShowResultImage(image, resultImageInfo, imgProcessingRes);
        return res;
    }
    else
    {
        showPrImage(image);
        savePrFailedImage(image);
        qCInfo(visionCate()) << m_config->locationName() << "perform pr failed!";
        return false;
    }
}

void VisionLocation::updatePrResultImage()
{
    QImage img = getImage();
    PRResultStruct prResult;
    PRResultImageInfo *resultImageInfo = nullptr;
    if (vision->performPr(img, m_config, &resultImageInfo, prResult))
    {
        vision->drawResultImage(img, resultImageInfo);
        QString resultImageName = VisionConfigDir::getIns().prConfigDir() + m_config->locationName() + "_resultImage.jpg";
        img.save(resultImageName);
        m_config->setPrResultImage(QString("file:///") + resultImageName);
        emit m_config->prResultImageChanged();
    }
    else
    {
        qCCritical(visionCate()) << locationName() << "perform pr failed!";
    }
}

bool VisionLocation::result2Offset(const PRResultStruct &prResult, PrOffset &prOffset)
{
    prOffset.ReSet();

    if (isDisabled)
    {
        return true;
    }

    QPointF xy = getMechDistanceFromImageCenter(QPointF(prResult.x, prResult.y));
    QPointF oriXy = getMechDistanceFromImageCenter(QPointF(prResult.ori_x, prResult.ori_y));

    prOffset.W = prResult.width;
    prOffset.H = prResult.height;
    prOffset.X = xy.x();
    prOffset.Y = xy.y();
    prOffset.O_X = oriXy.x();
    prOffset.O_Y = oriXy.y();

    bool exceededLimit = false;
    if (m_config->checkXy())
    {
        if (qAbs(prOffset.X) > m_config->maxXOffset() || qAbs(prOffset.Y) > m_config->maxYOffset())
        {

            exceededLimit = true;
        }
    }
    else
    {
        if (qAbs(prOffset.O_X) > m_config->maxXOffset() || qAbs(prOffset.O_Y) > m_config->maxYOffset())
        {
            exceededLimit = true;
        }
    }
    if (exceededLimit)
    {
        QString errMsg = QObject::tr("Pr offset exceeded limit! Offset: %1, OffsetX limit: %2, OffsetY limit:%3")
                             .arg(prOffset.toString())
                             .arg(m_config->maxXOffset())
                             .arg(m_config->maxYOffset());
        qCCritical(visionCate()) << m_config->locationName() << errMsg;
        return false;
    }

    if (abs(prResult.theta) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta;
    }
    else if (abs(prResult.theta - 90) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta - 90;
    }
    else if (abs(prResult.theta + 90) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta + 90;
    }
    else if (abs(prResult.theta - 180) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta - 180;
    }
    else if (abs(prResult.theta + 180) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta + 180;
    }
    else if (abs(prResult.theta - 270) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta - 270;
    }
    else if (abs(prResult.theta + 270) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta + 270;
    }
    else if (abs(prResult.theta - 360) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta - 360;
    }
    else if (abs(prResult.theta + 360) < m_config->maximunAngle())
    {
        prOffset.Theta = prResult.theta + 360;
    }
    else
    {
        QString errMsg = QObject::tr("Pr offset theta exceeded limit! Theta: %1, Theta limit: %2").arg(prResult.theta).arg(m_config->maximunAngle());
        qCCritical(visionCate()) << m_config->locationName() << errMsg;
        return false;
    }
    qCInfo(visionCate()) << m_config->locationName() << "pr offset:" << prOffset.toString();
    return true;
}

void VisionLocation::getXyPixelOffset(QPointF &xyPixel, QPointF &xyOffset)
{
    PRResultStruct prResult;
    if (!performPR(prResult))
    {
        throw SilicolAbort(QString("%1 perform pr failed!").arg(m_config->locationName()));
    }
    PrOffset prOffset;
    if (!result2Offset(prResult, prOffset))
    {
        throw SilicolAbort(QString("%1 result to offset failed!").arg(m_config->locationName()));
    }
    xyPixel.setX(prResult.x);
    xyPixel.setY(prResult.y);
    xyOffset.setX(prOffset.X);
    xyOffset.setY(prOffset.Y);
}

SCCamera *VisionLocation::camera() const
{
    return m_camera;
}

void VisionLocation::onVisionLocationConfigLightBrightnessChanged(int lightBrightnessChanged)
{
    try
    {
        m_camera->setLightSourceBrightness(lightBrightnessChanged);
    }
    catch (SilicoolException &se)
    {
        qCCritical(visionCate()) << se.what();
    }
}

void VisionLocation::onVisionLocationConfigSecondLightBrightnessChanged(int lightBrightnessChanged)
{
    try
    {
        m_camera->setSecondLightSourceBrightness(lightBrightnessChanged);
    }
    catch (SilicoolException &se)
    {
        qCCritical(visionCate()) << se.what();
    }
}

void VisionLocation::reverseTheta(PRResultStruct &prResult)
{
    if (m_config->reverseTheta())
    {
        prResult.theta *= -1;
    }
}

void VisionLocation::showPrImage(QImage &image, const QString &imgProcessingResult)
{
    if (m_config->showPrImage())
    {
        m_camera->showImage(image, imgProcessingResult);
    }
}

void VisionLocation::savePrFailedImage(QImage &image)
{
    if (m_config->savePrFailedImage())
    {
        m_camera->savePrFailedImage(image, m_config->locationName());
    }
}

void VisionLocation::drawSaveShowResultImage(QImage image, PRResultImageInfo *prResultImageInfo, QString imgProcessingResult)
{
    if (m_config->showPrImage() || m_config->saveResultImage())
    {
        QtConcurrent::run([image, prResultImageInfo, imgProcessingResult, this] {
            QImage tmpImage = image;
            vision->drawResultImage(tmpImage, prResultImageInfo);
            showPrImage(tmpImage, imgProcessingResult);
            if (m_config->saveResultImage())
            {
                m_camera->saveImage(
                    tmpImage, "",
                    QString("%1_%2_resultImage.jpg").arg(m_config->locationName()).arg(QDateTime::currentDateTime().toString("hh_mm_ss_zzz")));
            }
        });
    }
}
