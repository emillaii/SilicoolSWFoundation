#ifndef SCCAMERACONFIG_H
#define SCCAMERACONFIG_H

#include "configManager/configobject.h"
#include "enumhelper.h"
#include "scvision_global.h"

class SCVISIONSHARED_EXPORT CameraConfig : public ConfigObject
{
    Q_OBJECT

public:
    enum CameraVendor
    {
        Hik,
        Basler,
        DVP
    };
    Q_ENUM(CameraVendor)

    enum CameraType
    {
        GigE,
        USB
    };
    Q_ENUM(CameraType)

    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(CameraVendor cameraVendor READ cameraVendor WRITE setCameraVendor NOTIFY cameraVendorChanged)
    Q_PROPERTY(bool isDummyCamera READ isDummyCamera WRITE setIsDummyCamera NOTIFY isDummyCameraChanged)
    Q_PROPERTY(CameraType cameraType READ cameraType WRITE setCameraType NOTIFY cameraTypeChanged)
    Q_PROPERTY(QString cameraAddress READ cameraAddress WRITE setCameraAddress NOTIFY cameraAddressChanged)
    Q_PROPERTY(double exposureTime READ exposureTime WRITE setExposureTime NOTIFY exposureTimeChanged)
    Q_PROPERTY(QString lscName READ lscName WRITE setLscName NOTIFY lscNameChanged)
    Q_PROPERTY(int lightSourceChannel READ lightSourceChannel WRITE setLightSourceChannel NOTIFY lightSourceChannelChanged)
    Q_PROPERTY(int secondLightSourceChannel READ secondLightSourceChannel WRITE setSecondLightSourceChannel NOTIFY secondLightSourceChannelChanged)
    Q_PROPERTY(
        int delayAfterChangeBrightness READ delayAfterChangeBrightness WRITE setDelayAfterChangeBrightness NOTIFY delayAfterChangeBrightnessChanged)
    Q_PROPERTY(bool closeLightAfterPr READ closeLightAfterPr WRITE setCloseLightAfterPr NOTIFY closeLightAfterPrChanged)
    Q_PROPERTY(int imageWidth READ imageWidth WRITE setImageWidth NOTIFY imageWidthChanged)
    Q_PROPERTY(int imageHeight READ imageHeight WRITE setImageHeight NOTIFY imageHeightChanged)
    Q_PROPERTY(bool horizontalMirror READ horizontalMirror WRITE setHorizontalMirror NOTIFY horizontalMirrorChanged)
    Q_PROPERTY(bool verticalMirror READ verticalMirror WRITE setVerticalMirror NOTIFY verticalMirrorChanged)
    Q_PROPERTY(int showRealTimeImageInterval READ showRealTimeImageInterval WRITE setShowRealTimeImageInterval NOTIFY showRealTimeImageIntervalChanged)
    Q_PROPERTY(QString defaultImageFormatForSaving READ defaultImageFormatForSaving WRITE setDefaultImageFormatForSaving NOTIFY
                   defaultImageFormatForSavingChanged)
    Q_PROPERTY(bool logGetImageTime READ logGetImageTime WRITE setLogGetImageTime NOTIFY logGetImageTimeChanged)
    Q_PROPERTY(QString calibrationForMoveCamera READ calibrationForMoveCamera WRITE setCalibrationForMoveCamera NOTIFY calibrationForMoveCameraChanged)

public:
    Q_INVOKABLE CameraConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("cameraName");
        setIdentityProperty("cameraName");
        QVariantList imgFormats;
        imgFormats << "jpg"
                   << "png"
                   << "bmp";
        setOptionalProperty("defaultImageFormatForSaving", imgFormats);
        QStringList args;
        args << "cameraVendor"
             << "cameraType"
             << "cameraAddress"
             << "lscName"
             << "lightSourceChannel"
             << "secondLightSourceChannel"
             << "imageWidth"
             << "imageHeight"
             << "horizontalMirror"
             << "verticalMirror"
             << "defaultImageFormatForSaving"
             << "calibrationForMoveCamera";
        setEngineerAuthorities(args);
        init();
    }

    void setOptionalLscName(QVariantList optionalLscNames)
    {
        setOptionalProperty("lscName", optionalLscNames);
    }

    void setOptionalXYZR(QVariantList optionalXYZR)
    {
        setOptionalProperty("relatedXYZR", optionalXYZR);
    }

    int lightSourceChannel() const
    {
        return m_lightSourceChannel;
    }

    QString cameraName() const
    {
        return m_cameraName;
    }

    int imageWidth() const
    {
        return m_imageWidth;
    }

    int imageHeight() const
    {
        return m_imageHeight;
    }

    int delayAfterChangeBrightness() const
    {
        return m_delayAfterChangeBrightness;
    }

    int showRealTimeImageInterval() const
    {
        return m_showRealTimeImageInterval;
    }

    QString lscName() const
    {
        return m_lscName;
    }

    int secondLightSourceChannel() const
    {
        return m_secondLightSourceChannel;
    }

    bool closeLightAfterPr() const
    {
        return m_closeLightAfterPr;
    }

    bool horizontalMirror() const
    {
        return m_horizontalMirror;
    }

    bool verticalMirror() const
    {
        return m_verticalMirror;
    }

    CameraVendor cameraVendor() const
    {
        return m_cameraVendor;
    }

    QString calibrationForMoveCamera() const
    {
        return m_calibrationForMoveCamera;
    }

    bool isDummyCamera() const
    {
        return m_isDummyCamera;
    }

    QString cameraAddress() const
    {
        return m_cameraAddress;
    }

    CameraType cameraType() const
    {
        return m_cameraType;
    }

    bool logGetImageTime() const
    {
        return m_logGetImageTime;
    }

    double exposureTime() const
    {
        return m_exposureTime;
    }

    QString defaultImageFormatForSaving() const
    {
        return m_defaultImageFormatForSaving;
    }

public slots:
    void setLightSourceChannel(int lightSourceChannel)
    {
        if (m_lightSourceChannel == lightSourceChannel)
            return;

        m_lightSourceChannel = lightSourceChannel;
        emit lightSourceChannelChanged(m_lightSourceChannel);
    }

    void setCameraName(QString cameraName)
    {
        if (m_cameraName == cameraName)
            return;

        m_cameraName = cameraName;
        emit cameraNameChanged(m_cameraName);
    }

    void setImageWidth(int imageWidth)
    {
        if (m_imageWidth == imageWidth)
            return;

        m_imageWidth = imageWidth;
        emit imageWidthChanged(m_imageWidth);
    }

    void setImageHeight(int imageHeight)
    {
        if (m_imageHeight == imageHeight)
            return;

        m_imageHeight = imageHeight;
        emit imageHeightChanged(m_imageHeight);
    }

    void setDelayAfterChangeBrightness(int delayAfterChangeBrightness)
    {
        if (m_delayAfterChangeBrightness == delayAfterChangeBrightness)
            return;

        m_delayAfterChangeBrightness = delayAfterChangeBrightness;
        emit delayAfterChangeBrightnessChanged(m_delayAfterChangeBrightness);
    }

    void setShowRealTimeImageInterval(int showRealTimeImageInterval)
    {
        if (m_showRealTimeImageInterval == showRealTimeImageInterval)
            return;

        m_showRealTimeImageInterval = showRealTimeImageInterval;
        emit showRealTimeImageIntervalChanged(m_showRealTimeImageInterval);
    }

    void setLscName(QString lscName)
    {
        if (m_lscName == lscName)
            return;

        m_lscName = lscName;
        emit lscNameChanged(m_lscName);
    }

    void setSecondLightSourceChannel(int secondLightSourceChannel)
    {
        if (m_secondLightSourceChannel == secondLightSourceChannel)
            return;

        m_secondLightSourceChannel = secondLightSourceChannel;
        emit secondLightSourceChannelChanged(m_secondLightSourceChannel);
    }

    void setCloseLightAfterPr(bool closeLightAfterPr)
    {
        if (m_closeLightAfterPr == closeLightAfterPr)
            return;

        m_closeLightAfterPr = closeLightAfterPr;
        emit closeLightAfterPrChanged(m_closeLightAfterPr);
    }

    void setHorizontalMirror(bool horizontalMirror)
    {
        if (m_horizontalMirror == horizontalMirror)
            return;

        m_horizontalMirror = horizontalMirror;
        emit horizontalMirrorChanged(m_horizontalMirror);
    }

    void setVerticalMirror(bool verticalMirror)
    {
        if (m_verticalMirror == verticalMirror)
            return;

        m_verticalMirror = verticalMirror;
        emit verticalMirrorChanged(m_verticalMirror);
    }

    void setCameraVendor(CameraVendor cameraVendor)
    {
        if (m_cameraVendor == cameraVendor)
            return;

        m_cameraVendor = cameraVendor;
        emit cameraVendorChanged(m_cameraVendor);
    }

    void setCalibrationForMoveCamera(QString calibrationForMoveCamera)
    {
        if (m_calibrationForMoveCamera == calibrationForMoveCamera)
            return;

        m_calibrationForMoveCamera = calibrationForMoveCamera;
        emit calibrationForMoveCameraChanged(m_calibrationForMoveCamera);
    }

    void setIsDummyCamera(bool isDummyCamera)
    {
        if (m_isDummyCamera == isDummyCamera)
            return;

        m_isDummyCamera = isDummyCamera;
        emit isDummyCameraChanged(m_isDummyCamera);
    }

    void setCameraAddress(QString cameraAddress)
    {
        if (m_cameraAddress == cameraAddress)
            return;

        m_cameraAddress = cameraAddress;
        emit cameraAddressChanged(m_cameraAddress);
    }

    void setCameraType(CameraType cameraType)
    {
        if (m_cameraType == cameraType)
            return;

        m_cameraType = cameraType;
        emit cameraTypeChanged(m_cameraType);
    }

    void setLogGetImageTime(bool logGetImageTime)
    {
        if (m_logGetImageTime == logGetImageTime)
            return;

        m_logGetImageTime = logGetImageTime;
        emit logGetImageTimeChanged(m_logGetImageTime);
    }

    void setExposureTime(double exposureTime)
    {
        if (qFuzzyCompare(m_exposureTime, exposureTime))
            return;

        m_exposureTime = exposureTime;
        emit exposureTimeChanged(m_exposureTime);
    }

    void setDefaultImageFormatForSaving(QString defaultImageFormatForSaving)
    {
        if (m_defaultImageFormatForSaving == defaultImageFormatForSaving)
            return;

        m_defaultImageFormatForSaving = defaultImageFormatForSaving;
        emit defaultImageFormatForSavingChanged(m_defaultImageFormatForSaving);
    }

signals:
    void lightSourceChannelChanged(int lightSourceChannel);

    void cameraNameChanged(QString cameraName);

    void imageWidthChanged(int imageWidth);

    void imageHeightChanged(int imageHeight);

    void delayAfterChangeBrightnessChanged(int delayAfterChangeBrightness);

    void showRealTimeImageIntervalChanged(int showRealTimeImageInterval);

    void lscNameChanged(QString lscName);

    void secondLightSourceChannelChanged(int secondLightSourceChannel);

    void closeLightAfterPrChanged(bool closeLightAfterPr);

    void horizontalMirrorChanged(bool horizontalMirror);

    void verticalMirrorChanged(bool verticalMirror);

    void cameraVendorChanged(CameraVendor cameraVendor);

    void calibrationForMoveCameraChanged(QString calibrationForMoveCamera);

    void isDummyCameraChanged(bool isDummyCamera);

    void cameraAddressChanged(QString cameraAddress);

    void cameraTypeChanged(CameraType cameraType);

    void logGetImageTimeChanged(bool logGetImageTime);

    void exposureTimeChanged(double exposureTime);

    void defaultImageFormatForSavingChanged(QString defaultImageFormatForSaving);

private:
    int m_lightSourceChannel = 0;
    QString m_cameraName = "";
    int m_imageWidth = 0;
    int m_imageHeight = 0;
    int m_delayAfterChangeBrightness = 30;
    int m_showRealTimeImageInterval = 300;
    QString m_lscName;
    int m_secondLightSourceChannel = -1;
    bool m_closeLightAfterPr = false;
    bool m_horizontalMirror = false;
    bool m_verticalMirror = false;
    CameraVendor m_cameraVendor{ Hik };
    QString m_calibrationForMoveCamera;
    bool m_isDummyCamera = false;
    QString m_cameraAddress;
    CameraType m_cameraType{ GigE };
    bool m_logGetImageTime = false;
    double m_exposureTime = 1000;
    QString m_defaultImageFormatForSaving = "jpg";
};

#endif    // SCCAMERACONFIG_H
