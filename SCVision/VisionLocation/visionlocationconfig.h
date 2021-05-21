#ifndef VISIONLOCATIONCONFIG_H
#define VISIONLOCATIONCONFIG_H

#include "configManager/configobject.h"
#include "renameManager/renamemanager.h"
#include "scvision_global.h"
#include <QFileInfo>

class SCVISIONSHARED_EXPORT VisionLocationConfig : public ConfigObject
{
    Q_OBJECT

    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(QString calibrationName READ calibrationName WRITE setCalibrationName NOTIFY calibrationNameChanged)
    Q_PROPERTY(QString cameraName READ cameraName WRITE setCameraName NOTIFY cameraNameChanged)
    Q_PROPERTY(int lightBrightness READ lightBrightness WRITE setLightBrightness NOTIFY lightBrightnessChanged)
    Q_PROPERTY(int secondLightBrightness READ secondLightBrightness WRITE setSecondLightBrightness NOTIFY secondLightBrightnessChanged)
    Q_PROPERTY(bool checkXy READ checkXy WRITE setCheckXy NOTIFY checkXyChanged)
    Q_PROPERTY(bool reverseTheta READ reverseTheta WRITE setReverseTheta NOTIFY reverseThetaChanged)
    Q_PROPERTY(double maxXOffset READ maxXOffset WRITE setMaxXOffset NOTIFY maxXOffsetChanged)
    Q_PROPERTY(double maxYOffset READ maxYOffset WRITE setMaxYOffset NOTIFY maxYOffsetChanged)
    Q_PROPERTY(double maximunAngle READ maximunAngle WRITE setMaximunAngle NOTIFY maximunAngleChanged)
    Q_PROPERTY(double waitImageDelay READ waitImageDelay WRITE setWaitImageDelay NOTIFY waitImageDelayChanged)
    Q_PROPERTY(bool saveImageAfterGrab READ saveImageAfterGrab WRITE setSaveImageAfterGrab NOTIFY saveImageAfterGrabChanged)
    Q_PROPERTY(bool saveResultImage READ saveResultImage WRITE setSaveResultImage NOTIFY saveResultImageChanged)
    Q_PROPERTY(bool savePrFailedImage READ savePrFailedImage WRITE setSavePrFailedImage NOTIFY savePrFailedImageChanged)
    Q_PROPERTY(bool showPrImage READ showPrImage WRITE setShowPrImage NOTIFY showPrImageChanged)
    Q_PROPERTY(int continuoullyPrDelay READ continuoullyPrDelay WRITE setContinuoullyPrDelay NOTIFY continuoullyPrDelayChanged)
    Q_PROPERTY(int continuoullySaveImgInterval READ continuoullySaveImgInterval WRITE setContinuoullySaveImgInterval NOTIFY
                   continuoullySaveImgIntervalChanged)

public:
    Q_INVOKABLE VisionLocationConfig(QObject *parent = nullptr) : ConfigObject(parent)
    {
        setReadOnlyProperty("locationName");
        setReadOnlyProperty("calibrationName");
        setIdentityProperty("locationName");
        QStringList args;
        args << "cameraName"
             << "checkXy"
             << "reverseTheta"
             << "maximunAngle";
        setEngineerAuthorities(args);
        init();
        RenameManager::getIns().subscribeCameraNameChanged(this, "cameraName");
        RenameManager::getIns().subscribePrNameChanged(this, "locationName");
    }

    void setOptionalCamera(QStringList cameraNames)
    {
        setOptionalProperty("cameraName", toVariantList(cameraNames));
    }

    QString cameraName() const
    {
        return m_cameraName;
    }

    int lightBrightness() const
    {
        return m_lightBrightness;
    }

    double maximunAngle() const
    {
        return m_maximunAngle;
    }

    QString locationName() const
    {
        return m_locationName;
    }

    QString calibrationName() const
    {
        return m_calibrationName;
    }

    double waitImageDelay() const
    {
        return m_waitImageDelay;
    }

    virtual void resetPrResultImage() {}

    void setPrResultImage(QString prResultImage)
    {
        m_prResultImage = prResultImage;
    }

    Q_INVOKABLE QString prResultImage() const
    {
        return m_prResultImage;
    }

    bool saveImageAfterGrab() const
    {
        return m_saveImageAfterGrab;
    }

    int secondLightBrightness() const
    {
        return m_secondLightBrightness;
    }

    double maxXOffset() const
    {
        return m_maxXOffset;
    }

    double maxYOffset() const
    {
        return m_maxYOffset;
    }

    bool saveResultImage() const
    {
        return m_saveResultImage;
    }

    int continuoullyPrDelay() const
    {
        return m_continuoullyPrDelay;
    }

    int continuoullySaveImgInterval() const
    {
        return m_continuoullySaveImgInterval;
    }

    bool savePrFailedImage() const
    {
        return m_savePrFailedImage;
    }

    bool showPrImage() const
    {
        return m_showPrImage;
    }

    bool checkXy() const
    {
        return m_checkXy;
    }

    bool reverseTheta() const
    {
        return m_reverseTheta;
    }

public slots:
    void setCameraName(QString cameraName)
    {
        if (m_cameraName == cameraName)
            return;

        m_cameraName = cameraName;
        emit cameraNameChanged(m_cameraName);
    }

    void setLightBrightness(int lightBrightness)
    {
        if (m_lightBrightness == lightBrightness)
            return;

        m_lightBrightness = lightBrightness;
        emit lightBrightnessChanged(m_lightBrightness);
    }

    void setMaximunAngle(double maximunAngle)
    {
        if (qFuzzyCompare(m_maximunAngle, maximunAngle))
            return;

        m_maximunAngle = maximunAngle;
        emit maximunAngleChanged(m_maximunAngle);
    }

    void setLocationName(QString lcationName)
    {
        if (m_locationName == lcationName)
            return;

        m_locationName = lcationName;
        emit locationNameChanged(m_locationName);
    }

    void setCalibrationName(QString calibrationName)
    {
        if (m_calibrationName == calibrationName)
            return;

        m_calibrationName = calibrationName;
        emit calibrationNameChanged(m_calibrationName);
    }

    void setWaitImageDelay(double waitImageDelay)
    {
        if (qFuzzyCompare(m_waitImageDelay, waitImageDelay))
            return;

        m_waitImageDelay = waitImageDelay;
        emit waitImageDelayChanged(m_waitImageDelay);
    }

    void setSaveImageAfterGrab(bool saveImageAfterGrab)
    {
        if (m_saveImageAfterGrab == saveImageAfterGrab)
            return;

        m_saveImageAfterGrab = saveImageAfterGrab;
        emit saveImageAfterGrabChanged(m_saveImageAfterGrab);
    }

    void setSecondLightBrightness(int secondLightBrightness)
    {
        if (m_secondLightBrightness == secondLightBrightness)
            return;

        m_secondLightBrightness = secondLightBrightness;
        emit secondLightBrightnessChanged(m_secondLightBrightness);
    }

    void setMaxXOffset(double maxXOffset)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxXOffset, maxXOffset))
            return;

        m_maxXOffset = maxXOffset;
        emit maxXOffsetChanged(m_maxXOffset);
    }

    void setMaxYOffset(double maxYOffset)
    {
        //        qWarning("Floating point comparison needs context sanity check");
        if (qFuzzyCompare(m_maxYOffset, maxYOffset))
            return;

        m_maxYOffset = maxYOffset;
        emit maxYOffsetChanged(m_maxYOffset);
    }

    void setSaveResultImage(bool saveResultImage)
    {
        if (m_saveResultImage == saveResultImage)
            return;

        m_saveResultImage = saveResultImage;
        emit saveResultImageChanged(m_saveResultImage);
    }

    void setContinuoullyPrDelay(int continuoullyPrDelay)
    {
        if (m_continuoullyPrDelay == continuoullyPrDelay)
            return;

        m_continuoullyPrDelay = continuoullyPrDelay;
        emit continuoullyPrDelayChanged(m_continuoullyPrDelay);
    }

    void setContinuoullySaveImgInterval(int continuoullySaveImgInterval)
    {
        if (m_continuoullySaveImgInterval == continuoullySaveImgInterval)
            return;

        m_continuoullySaveImgInterval = continuoullySaveImgInterval;
        emit continuoullySaveImgIntervalChanged(m_continuoullySaveImgInterval);
    }

    void setSavePrFailedImage(bool savePrFailedImage)
    {
        if (m_savePrFailedImage == savePrFailedImage)
            return;

        m_savePrFailedImage = savePrFailedImage;
        emit savePrFailedImageChanged(m_savePrFailedImage);
    }

    void setShowPrImage(bool showPrImage)
    {
        if (m_showPrImage == showPrImage)
            return;

        m_showPrImage = showPrImage;
        emit showPrImageChanged(m_showPrImage);
    }

    void setCheckXy(bool checkXy)
    {
        if (m_checkXy == checkXy)
            return;

        m_checkXy = checkXy;
        emit checkXyChanged(m_checkXy);
    }

    void setReverseTheta(bool reverseTheta)
    {
        if (m_reverseTheta == reverseTheta)
            return;

        m_reverseTheta = reverseTheta;
        emit reverseThetaChanged(m_reverseTheta);
    }

signals:
    void cameraNameChanged(QString cameraName);

    void lightBrightnessChanged(int lightBrightness);

    void maximunAngleChanged(double maximunAngle);

    void locationNameChanged(QString locationName);

    void calibrationNameChanged(QString calibrationName);

    void waitImageDelayChanged(double waitImageDelay);

    void saveImageAfterGrabChanged(bool saveImageAfterGrab);

    void secondLightBrightnessChanged(int secondLightBrightness);

    void maxXOffsetChanged(double maxXOffset);

    void maxYOffsetChanged(double maxYOffset);

    void saveResultImageChanged(bool saveResultImage);

    void continuoullyPrDelayChanged(int continuoullyPrDelay);

    void continuoullySaveImgIntervalChanged(int continuoullySaveImgInterval);

    void savePrFailedImageChanged(bool savePrFailedImage);

    void showPrImageChanged(bool showPrImage);

    void checkXyChanged(bool checkXy);

    void reverseThetaChanged(bool reverseTheta);

    void prResultImageChanged();

private:
    QString m_cameraName = "";
    int m_lightBrightness = 0;
    double m_maximunAngle = 44;
    QString m_locationName = "";
    QString m_calibrationName = "";
    double m_waitImageDelay = -1;
    QString m_prResultImage;
    bool m_saveImageAfterGrab = false;
    int m_secondLightBrightness = 0;
    double m_maxXOffset = 5;
    double m_maxYOffset = 5;
    bool m_saveResultImage = false;
    int m_continuoullyPrDelay = -1;
    int m_continuoullySaveImgInterval = -1;
    bool m_savePrFailedImage = true;
    bool m_showPrImage = true;
    bool m_checkXy = true;
    bool m_reverseTheta = true;
};

#endif    // VISIONLOCATIONCONFIG_H
