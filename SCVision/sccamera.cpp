#include "sccamera.h"

SCCamera::SCCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent)
    : QThread(parent), QQuickImageProvider(QQuickImageProvider::Image), m_cameraName(cameraName), m_config(cameraConfig)
{
    setHasSecondLightSourceChannel(cameraConfig->secondLightSourceChannel() >= 0);
    connect(cameraConfig, &CameraConfig::secondLightSourceChannelChanged, this, &SCCamera::onSecondLightSourceChannelChanged);
}

SCCamera::~SCCamera()
{
    close();
}

void SCCamera::setLsc(LightSourceController *lsc)
{
    this->lsc = lsc;
}

void SCCamera::setTwoChanndelBrightness(int brightness1, int brightness2, bool waitStable)
{
    if (lsc == nullptr)
    {
        throw SilicolAbort(tr("Light source controller was not set! Camera: %1, LSC name: %2").arg(m_cameraName).arg(m_config->lscName()),
                           EX_LOCATION);
    }

    bool anyBrightnessChanged = false;
    if (m_lightSourceBrightness != brightness1)
    {
        try
        {
            lsc->setBrightness(m_config->lightSourceChannel(), brightness1);
        }
        catch (SilicoolException &se)
        {
            emit lightSourceBrightnessChanged(m_lightSourceBrightness);
            throw se;
        }

        m_lightSourceBrightness = brightness1;
        emit lightSourceBrightnessChanged(m_lightSourceBrightness);
        anyBrightnessChanged = true;
    }

    if (hasSecondLightSourceChannel() && m_secondLightSourceBrightness != brightness2)
    {
        try
        {
            lsc->setBrightness(m_config->secondLightSourceChannel(), brightness2);
        }
        catch (SilicoolException &se)
        {
            emit lightSourceBrightnessChanged(m_secondLightSourceBrightness);
            throw se;
        }

        m_secondLightSourceBrightness = brightness2;
        emit secondLightSourceBrightnessChanged(m_secondLightSourceBrightness);
        anyBrightnessChanged = true;
    }

    if (anyBrightnessChanged && waitStable && m_config->delayAfterChangeBrightness() > 0)
    {
        QThread::msleep(m_config->delayAfterChangeBrightness());
    }
}

void SCCamera::startShowRealtimeImage(bool useIntervalFromConfig)
{
    if (isShowRealtimeImage())
    {
        return;
    }
    this->useIntervalFromConfig = useIntervalFromConfig;
    setIsShowRealtimeImage(true);
    start();
}

void SCCamera::stopShowRealtimeImage()
{
    if (isShowRealtimeImage())
    {
        setIsShowRealtimeImage(false);
        wait();
    }
}

int SCCamera::pixelValue(int x, int y)
{
    if (imgForDisplay.valid(x, y))
    {
        if (imgForDisplay.depth() > 8)
        {
            QColor c = imgForDisplay.pixelColor(x, y);
            return (c.red() * 38 + c.green() * 75 + c.blue() * 15) >> 7;
        }
        else
        {
            return imgForDisplay.pixelIndex(x, y);
        }
    }
    else
    {
        return -1;
    }
}

void SCCamera::open()
{
    if (!isOpened())
    {
        openImpl();
        setIsOpened(true);
    }
}

void SCCamera::close()
{
    stopShowRealtimeImage();
    if (isOpened())
    {
        setIsOpened(false);
        closeImpl();
    }
}

QImage SCCamera::getImage(bool showThisImage, bool saveThisImage)
{
    if (!isOpened())
    {
        throw GeneralError("Vision", tr("Camera %1 is not opened!").arg(cameraName()));
    }

    QImage img;
    {
        AutoResetBool arb(&m_gotImage, true);
        QMutexLocker tmpLocker(&getImagelocker);
        SCTimer t(QString("%1 get image.").arg(cameraName()), visionCate(), config()->logGetImageTime());
        img = getImageImpl();
    }

    if (config()->horizontalMirror() || config()->verticalMirror())
    {
        img = img.mirrored(config()->horizontalMirror(), config()->verticalMirror());
    }
    if (showThisImage)
    {
        showImage(img);
    }
    if (saveThisImage)
    {
        saveImage(img);
    }
    return img;
}

void SCCamera::resetGotImage()
{
    m_gotImage = false;
}

bool SCCamera::gotImage() const
{
    return m_gotImage;
}

void SCCamera::showImage(QImage &image, QString imgProcessingResult)
{
    {
        QMutexLocker t(&lastImageLocker);
        lastImage = image;
    }
    emit imageChanged(imgProcessingResult);
}

void SCCamera::saveImage(QString dir, QString name)
{
    QImage image;
    if (isShowRealtimeImage() && m_config->showRealTimeImageInterval() < 200)
    {
        QMutexLocker t(&lastImageLocker);
        image = lastImage.copy();
    }
    else
    {
        image = getImage(true, false);
    }
    saveImage(image, dir, name);
}

void SCCamera::saveImage(QImage image, QString dir, QString name)
{
    if (dir.isEmpty())
    {
        dir = QString("./images/%1/%2/").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd")).arg(m_cameraName);
    }

    if (name.isEmpty())
    {
        name = QString("%1_%2.%3")
                   .arg(m_cameraName)
                   .arg(QDateTime::currentDateTime().toString("hh_mm_ss_zzz"))
                   .arg(m_config->defaultImageFormatForSaving());
    }

    ImageSaver::getIns().saveImage(image, dir, name);
}

void SCCamera::savePrFailedImage(QImage image, QString prName)
{
    QString dir = QString("./images/%1/%2/PrFailed/").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd")).arg(m_cameraName);
    QString name
        = QString("%1_%2.%3").arg(prName).arg(QDateTime::currentDateTime().toString("hh_mm_ss_zzz")).arg(m_config->defaultImageFormatForSaving());
    saveImage(image, dir, name);
}

void SCCamera::setLightSourceBrightness(int lightSourceBrightness)
{
    if (m_lightSourceBrightness == lightSourceBrightness)
        return;

    if (lsc == nullptr)
    {
        throw SilicolAbort(tr("Light source controller was not set! Camera: %1, LSC name: %2").arg(m_cameraName).arg(m_config->lscName()),
                           EX_LOCATION);
    }
    try
    {
        lsc->setBrightness(m_config->lightSourceChannel(), lightSourceBrightness);
    }
    catch (SilicoolException &se)
    {
        emit lightSourceBrightnessChanged(m_lightSourceBrightness);
        throw se;
    }

    m_lightSourceBrightness = lightSourceBrightness;
    emit lightSourceBrightnessChanged(m_lightSourceBrightness);
}

void SCCamera::setSecondLightSourceBrightness(int secondLightSourceBrightness)
{
    if (m_secondLightSourceBrightness == secondLightSourceBrightness)
        return;
    if (!hasSecondLightSourceChannel())
        return;

    if (lsc == nullptr)
    {
        throw SilicolAbort(tr("Light source controller was not set! Camera: %1, LSC name: %2").arg(m_cameraName).arg(m_config->lscName()),
                           EX_LOCATION);
    }
    try
    {
        lsc->setBrightness(m_config->secondLightSourceChannel(), secondLightSourceBrightness);
    }
    catch (SilicoolException &se)
    {
        emit lightSourceBrightnessChanged(m_secondLightSourceBrightness);
        throw se;
    }

    m_secondLightSourceBrightness = secondLightSourceBrightness;
    emit secondLightSourceBrightnessChanged(m_secondLightSourceBrightness);
}

void SCCamera::run()
{
    try
    {
        while (isShowRealtimeImage())
        {
            QImage img = getImage(false, false);
            QString resultString;
            if (m_calcObjectSharpness)
            {
                double sharpness = getObjectSharpness(img);
                resultString = QString("Sharpness: %1").arg(sharpness, 0, 'g', 2);
            }
            showImage(img, resultString);
            QElapsedTimer timer;
            timer.start();
            int interval = useIntervalFromConfig ? m_config->showRealTimeImageInterval() : 100;
            while (timer.elapsed() < interval && isShowRealtimeImage())
            {
                QThread::msleep(10);
            }
        }
    }
    catch (SilicoolException &se)
    {
        qCCritical(visionCate()) << se.what();
    }
    setIsShowRealtimeImage(false);
}

QImage SCCamera::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
    Q_UNUSED(size)
    Q_UNUSED(requestedSize)
    QMutexLocker t(&lastImageLocker);
    imgForDisplay = lastImage.copy();
    return imgForDisplay;
}

double SCCamera::getObjectSharpness(QImage &image)
{
    cv::Mat srcGray(image.height(), image.width(), CV_8UC1, image.bits());
    double result = 0;
    for (int i = srcGray.rows / 4; i < 3 * srcGray.rows / 4; i++)
    {
        uchar *data = srcGray.ptr<uchar>(i);
        for (int j = srcGray.cols / 4; j < 3 * srcGray.cols / 4; j++)
        {
            result += (data[j + 2] - data[j]) * (data[j + 2] - data[j]);
        }
    }
    result = 4 * result / srcGray.total();

    return result;
}
