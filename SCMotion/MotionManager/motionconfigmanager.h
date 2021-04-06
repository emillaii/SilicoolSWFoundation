#ifndef MOTIONCONFIGMANAGER_H
#define MOTIONCONFIGMANAGER_H

#include "AxisModule/singleaxis.h"
#include "AxisModule/xymodule.h"
#include "AxisModule/xyzmodule.h"
#include "BasicElement/basicelementfactory.h"
#include "BasicElement/ioconfig.h"
#include "CompositeElement/cylinderconfig.h"
#include "CompositeElement/vacuumconfig.h"
#include "MoveProtection/collisiongroup.h"
#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "configManager/configshareserver.h"
#include "globalmotionelements.h"
#include "motionelementdefinition.h"
#include "scmotion_global.h"

class SCMOTIONSHARED_EXPORT MasterMotionManager;

class SCMOTIONSHARED_EXPORT MotionConfigManager : public QObject
{
    Q_OBJECT

public:
    MotionConfigManager(BasicElementFactory *basicElementFactory, QString configFileDir, QObject *parent = nullptr);

    void createLostConfig(MotionElementDefinition *med);

    void setConfigOptions(MotionElementDefinition *med);

    void removeRedundantConfig(ConfigObjectArray *meds);

    void initMoveProtectionConfig();

    void shareAllMotionConfig();

    void extractSoftLandingPos();

    void setGlobalVelocityRatio(double globalVelocityRatio);

    void setAxisVelocityRatio(QString axisName, double ratio);

    QList<QString> diNames() const
    {
        return diConfigMap.keys();
    }

    QList<QString> doNames() const
    {
        return doConfigMap.keys();
    }

    QList<QString> cylNames() const
    {
        return cylConfigMap.keys();
    }

    QList<QString> vacuumNames() const
    {
        return vacuumConfigMap.keys();
    }

    QList<QString> axisNames() const
    {
        return axisConfigMap.keys();
    }

    SingleAxisPos *getSAxisModulePos(QString moduleName, QString posName) const;
    XYModulePos *getXyModulePos(QString moduleName, QString posName) const;
    XYZModulePos *getXyzModulePos(QString moduleName, QString posName) const;

    AxisModuleConfig *getModuleConfig(QString moduleName, MotionElement::Type moduleType);

    Q_INVOKABLE QObject *sAxisModulePos(QString moduleName, QString posName) const;
    Q_INVOKABLE QObject *xyModulePos(QString moduleName, QString posName) const;
    Q_INVOKABLE QObject *xyzModulePos(QString moduleName, QString posName) const;
    Q_INVOKABLE QObject *softLandingPos(QString axisName, QString softLandingPosName) const;

private:
    void config2Dic();
    void createLostAxisModuleConfig(MotionElementDefinition *med);

private:
    friend class MasterMotionManager;

    BasicElementFactory *basicElementFactory = nullptr;

    ConfigObjectArray *m_axisConfigs;
    ConfigObjectArray *m_extendedAxisConfigs;
    ConfigObjectArray *m_extendedAxis2Configs;
    ConfigObjectArray *m_extendedAxis3Configs;
    ConfigObjectArray *m_extendedAxis4Configs;
    ConfigObjectArray *m_diConfigs;
    ConfigObjectArray *m_doConfigs;
    ConfigObjectArray *m_cylConfigs;
    ConfigObjectArray *m_vacuumConfigs;
    ConfigObjectArray *m_singleAxisModuleConfigs;
    ConfigObjectArray *m_xyModuleConfigs;
    ConfigObjectArray *m_xyzModuleConfigs;
    ConfigObjectArray *m_softLandingConfigs;

    ConfigFile *axisConfigsFile;
    ConfigFile *extendedAxisConfigsFile;
    ConfigFile *extendedAxis2ConfigsFile;
    ConfigFile *extendedAxis3ConfigsFile;
    ConfigFile *extendedAxis4ConfigsFile;
    ConfigFile *diConfigsFile;
    ConfigFile *doConfigsFile;
    ConfigFile *cylConfigsFile;
    ConfigFile *vacuumConfigsFile;
    ConfigFile *singleAxisModuleConfigsFile;
    ConfigFile *xyModuleConfigsFile;
    ConfigFile *xyzModuleConfigsFile;

    const QString motionConfigFileDir = "./config/motionConfig/";
    const QString axisConfigsFileName = "axisConfigs.json";
    const QString extendedAxisConfigsFileName = "extendedAxisConfigs.json";
    const QString extendedAxis2ConfigsFileName = "extendedAxis2Configs.json";
    const QString extendedAxis3ConfigsFileName = "extendedAxis3Configs.json";
    const QString extendedAxis4ConfigsFileName = "extendedAxis4Configs.json";
    const QString diConfigsFileName = "diConfigs.json";
    const QString doConfigsFileName = "doConfigs.json";
    const QString cylConfigsFileName = "cylinderConfigs.json";
    const QString vacuumConfigsFileName = "vacuumConfigs.json";

    QString dutRelatedConfigFileDir;

    const QString singleAxisModuleConfigsFileName = "singleAxisModuleConfigs.json";
    const QString xyModuleConfigsFileName = "xyModuleConfigs.json";
    const QString xyzModuleConfigsFileName = "xyzModuleConfigs.json";

    QMap<QString, AxisConfig *> axisConfigMap;
    QMap<QString, IOConfig *> diConfigMap;
    QMap<QString, IOConfig *> doConfigMap;
    QMap<QString, CylinderConfig *> cylConfigMap;
    QMap<QString, VacuumConfig *> vacuumConfigMap;
    QMap<QString, SingleAxisConfig *> sAxisConfigMap;
    QMap<QString, XYModuleConfig *> xyModuleConfigMap;
    QMap<QString, XYZModuleConfig *> xyzModuleConfigMap;

    ConfigObjectArray *m_collisionGroupConfigs;
    ConfigFile *collisionGroupConfigsFile;
    const QString collisionGroupConfigsFileName = "moveProtectionConfig.json";
};

#endif    // MOTIONCONFIGMANAGER_H
