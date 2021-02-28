#ifndef SCCAMERA_H
#define SCCAMERA_H

#include "MotionManager/motionmanager.h"
#include "errorHandling/silicolerror.h"
#include "imagesaver.h"
#include "lightsourcecontroller.h"
#include "sccameraconfig.h"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QQuickImageProvider>
#include <QTimer>

class SCVISIONSHARED_EXPORT SCCamera : public QThread, public QQuickImageProvider
{
    Q_OBJECT

    Q_PROPERTY(int lightSourceBrightness READ lightSourceBrightness NOTIFY lightSourceBrightnessChanged)
    Q_PROPERTY(int secondLightSourceBrightness READ secondLightSourceBrightness WRITE setSecondLightSourceBrightness NOTIFY
                   secondLightSourceBrightnessChanged)
    Q_PROPERTY(bool hasSecondLightSourceChannel READ hasSecondLightSourceChannel WRITE setHasSecondLightSourceChannel NOTIFY
                   hasSecondLightSourceChannelChanged)
    Q_PROPERTY(bool isOpened READ isOpened WRITE setIsOpened NOTIFY isOpenedChanged)
    Q_PROPERTY(bool isShowRealtimeImage READ isShowRealtimeImage WRITE setIsShowRealtimeImage NOTIFY isShowRealtimeImageChanged)

public:
    explicit SCCamera(QString cameraName, CameraConfig *cameraConfig, QObject *parent = nullptr);

    virtual ~SCCamera() override;

    void setLsc(LightSourceController *lsc);

    void setTwoChanndelBrightness(int brightness1, int brightness2, bool waitStable);

    Q_INVOKABLE void startShowRealtimeImage(bool useIntervalFromConfig = true);
    Q_INVOKABLE void stopShowRealtimeImage();
    Q_INVOKABLE int pixelValue(int x, int y);

    Q_INVOKABLE void open();
    Q_INVOKABLE void close();
    QImage getImage(bool showThisImage = false, bool saveThisImage = false);

    void resetGotImage();
    bool gotImage() const;

    void showImage(QImage &image, QString imgProcessingResult = "");

    Q_INVOKABLE void saveImage(QString dir = "", QString name = "");
    void saveImage(QImage image, QString dir = "", QString name = "");
    void savePrFailedImage(QImage image, QString prName);

    int lightSourceBrightness() const
    {
        return m_lightSourceBrightness;
    }

    bool isOpened() const
    {
        return m_isOpened;
    }

    bool isShowRealtimeImage() const
    {
        return m_isShowRealtimeImage;
    }

    int secondLightSourceBrightness() const
    {
        return m_secondLightSourceBrightness;
    }

    bool hasSecondLightSourceChannel() const
    {
        return m_hasSecondLightSourceChannel;
    }

    CameraConfig *config() const
    {
        return m_config;
    }

    QString cameraName() const
    {
        return m_cameraName;
    }

protected:
    virtual QImage getImageImpl() = 0;
    virtual void openImpl() = 0;
    virtual void closeImpl() = 0;

public slots:
    void setLightSourceBrightness(int lightSourceBrightness);

    void setIsOpened(bool isOpened)
    {
        if (m_isOpened == isOpened)
            return;

        m_isOpened = isOpened;
        emit isOpenedChanged(m_isOpened);
    }

    void setIsShowRealtimeImage(bool isShowRealtimeImage)
    {
        if (m_isShowRealtimeImage == isShowRealtimeImage)
            return;

        m_isShowRealtimeImage = isShowRealtimeImage;
        emit isShowRealtimeImageChanged(m_isShowRealtimeImage);
    }

    void setSecondLightSourceBrightness(int secondLightSourceBrightness);

    void setHasSecondLightSourceChannel(bool hasSecondLightSourceChannel)
    {
        if (m_hasSecondLightSourceChannel == hasSecondLightSourceChannel)
            return;

        m_hasSecondLightSourceChannel = hasSecondLightSourceChannel;
        emit hasSecondLightSourceChannelChanged(m_hasSecondLightSourceChannel);
    }

    void onSecondLightSourceChannelChanged(int channel)
    {
        setHasSecondLightSourceChannel(channel >= 0);
    }

signals:
    void lightSourceBrightnessChanged(int lightSourceBrightness);
    void imageChanged(QString imgProcessingResult);
    void isOpenedChanged(bool isOpened);
    void isShowRealtimeImageChanged(bool isShowRealtimeImage);
    void secondLightSourceBrightnessChanged(int secondLightSourceBrightness);
    void hasSecondLightSourceChannelChanged(bool hasSecondLightSourceChannel);

    // QThread interface
protected:
    virtual void run() override;

    // QQuickImageProvider interface
public:
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    QImage lastImage;
    QImage imgForDisplay;
    QString m_cameraName;
    QMutex getImagelocker;
    QMutex lastImageLocker;
    CameraConfig *m_config = nullptr;
    LightSourceController *lsc = nullptr;
    int m_lightSourceBrightness = 0;
    bool m_isOpened = false;
    bool m_isShowRealtimeImage = false;
    bool useIntervalFromConfig = true;
    int m_secondLightSourceBrightness = 0;
    bool m_hasSecondLightSourceChannel;

    bool m_gotImage = false;
};

#endif    // SCCAMERA_H
