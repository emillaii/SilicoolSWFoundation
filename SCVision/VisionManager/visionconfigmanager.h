#ifndef VISIONCONFIGMANAGER_H
#define VISIONCONFIGMANAGER_H

#include "Calibration/calibrationconfig.h"
#include "VisionLocation/visionlocationconfig.h"
#include "configManager/configfile.h"
#include "moveandprconfig.h"
#include "sccamera.h"
#include "scvision_global.h"
#include "visionelementdefinition.h"
#include <QMap>
#include <QObject>

class VisionManager;

class SCVISIONSHARED_EXPORT VisionConfigManager : public QObject
{
    Q_OBJECT
public:
    explicit VisionConfigManager(const QMetaObject &visionLocationConfigMetaObj, QString configDir, QObject *parent = nullptr);

    VisionElementDefinition *visionElementDefinition() const
    {
        return ved;
    }

    ConfigObjectArray *calibrationConfigs() const
    {
        return m_calibrationConfigs;
    }

    ConfigObjectArray *visionLocationConfigs() const
    {
        return m_visionLocationConfigs;
    }

    ConfigObjectArray *cameraConfigs() const
    {
        return m_cameraConfigs;
    }

    MoveAndPrConfig *moveAndPrConfig() const
    {
        return m_moveAndPrConfig;
    }

    void setOptionalMotors(QStringList motorNames)
    {
        QVariantList qvMotorNames = toVariantList(motorNames);
        foreach (auto calibration, calibrationConfigMap.values())
        {
            calibration->setOptionalMotorName(qvMotorNames);
        }
    }

    void setOptionalXYZR(QStringList xyzrNames)
    {
        QVariantList qvXyzrNames = toVariantList(xyzrNames);
        foreach (auto cameraConfig, cameraConfigMap.values())
        {
            cameraConfig->setOptionalXYZR(qvXyzrNames);
        }
    }

    Q_INVOKABLE QObject *getVisionLocationConfig(QString visionLocationName) const
    {
        if (!visionLocationConfigMap.contains(visionLocationName))
        {
            return nullptr;
        }
        return visionLocationConfigMap[visionLocationName];
    }

private:
    void config2Dic();

    void createLostConfig();

    void removeRedundantConfig();

    void setOptionalProperties();

private:
    friend class VisionManager;

    const QString visionConfigDir = "./config/visionConfig/";
    QString dutRelatedConfigDir;

    const QString vedConfigFileName = "./config/platformConfig/visionElementDefinition.json";
    VisionElementDefinition *ved;
    ConfigFile *vedConfigFile;

    const QString calibrationConfigsFileName = "calibrationConfig.json";
    ConfigObjectArray *m_calibrationConfigs;
    ConfigFile *calibrationConfigsFile;

    const QString visionLocationConfigsFileName = "visionLocationConfig.json";
    ConfigObjectArray *m_visionLocationConfigs;
    ConfigFile *visionLocationConfigsFile;

    const QString cameraConfigsFileName = "cameraConfig.json";
    ConfigObjectArray *m_cameraConfigs;
    ConfigFile *cameraConfigsFile;

    const QString moveAndPrConfigFileName = "moveAndPrConfig.json";
    MoveAndPrConfig *m_moveAndPrConfig;
    ConfigFile *moveAndPrConfigFile;

    QMap<QString, CalibrationConfig *> calibrationConfigMap;
    QMap<QString, VisionLocationConfig *> visionLocationConfigMap;
    QMap<QString, CameraConfig *> cameraConfigMap;
};

#endif    // VISIONCONFIGMANAGER_H
