#ifndef VISIONMANAGER_H
#define VISIONMANAGER_H

#include "Calibration/calibration.h"
#include "VisionLocation/visionlocation.h"
#include "dummycamera.h"
#include "scvision_global.h"
#include "visionconfigmanager.h"
#include "visionfactory.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class SCVISIONSHARED_EXPORT VisionManager : public QObject
{
    Q_OBJECT

private:
    explicit VisionManager() {}

public:
    static VisionManager &getIns()
    {
        static VisionManager instance;
        return instance;
    }

    ~VisionManager();

    void dispose();

    void setVisionFactory(VisionFactory *visionFactory)
    {
        this->visionFactory = visionFactory;
    }

    void setVisionConfigManager(VisionConfigManager *value)
    {
        this->visionConfigManager = value;
    }

    void setContextProperty(QQmlApplicationEngine &engine);

    void preInit();

    void postInit(bool checkDongle = true);

    void stopAllCameraShowRealtimeImage();

    Q_INVOKABLE void handleSingleCameraLiveView(QString cameraName);

    QStringList cameraNames() const;

    VisionLocation *getVisionLocation(QString visionLocationName);

    Calibration *getCalibration(QString calibrationName);

    Q_INVOKABLE QObject *getCamera(QString cameraName) const;

    Q_INVOKABLE void calibrate(QString calibrationName);

    Q_INVOKABLE void performPr(QString visionLocationName);

    Q_INVOKABLE void startContinuallyPerformPr(QString visionLocationName);

    Q_INVOKABLE void startContinuallyPerformObjSharpnessCalc(QString visionLocationName);

    Q_INVOKABLE void stopContinuallyCalculation(QString visionLocationName);

    Q_INVOKABLE void performPrResult(QString calibrationName, QString visionLocationName, bool perfomOriginPrResult);

    Q_INVOKABLE void moveCamera(QString cameraName, double px, double py);

    Q_INVOKABLE void startMoveAndPr();

    Q_INVOKABLE void stopMoveAndPr();

    SCVision *vision() const
    {
        return m_vision;
    }

private:
    void initCamera(SCCamera *camera);

private:
    VisionConfigManager *visionConfigManager = nullptr;
    VisionFactory *visionFactory = nullptr;
    SCVision *m_vision = nullptr;
    QMap<QString, LightSourceController *> lscs;
    QMap<QString, SCCamera *> cameraMap;
    QMap<QString, VisionLocation *> visionLocations;
    QMap<QString, Calibration *> calibrations;
    QMap<QString, bool> isContinuallyPerformPr;
    bool runMoveAndPr = false;
};

#endif    // VISIONMANAGER_H
