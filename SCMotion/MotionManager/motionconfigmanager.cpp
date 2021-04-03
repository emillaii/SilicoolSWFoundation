#include "motionconfigmanager.h"

MotionConfigManager::MotionConfigManager(BasicElementFactory *basicElementFactory, QString configFileDir, QObject *parent)
    : QObject(parent), basicElementFactory(basicElementFactory), dutRelatedConfigFileDir(configFileDir)
{
    if (!dutRelatedConfigFileDir.endsWith("/") && !dutRelatedConfigFileDir.endsWith("\\"))
    {
        dutRelatedConfigFileDir += "/";
    }
    m_axisConfigs = new ConfigObjectArray(basicElementFactory->axisConfigMetaObj(), this);
    m_extendedAxisConfigs = new ConfigObjectArray(basicElementFactory->extendedAxisConfigMetaObj(), this);
    m_extendedAxis2Configs = new ConfigObjectArray(basicElementFactory->extendedAxis2ConfigMetaObj(), this);
    m_extendedAxis3Configs = new ConfigObjectArray(basicElementFactory->extendedAxis3ConfigMetaObj(), this);
    m_extendedAxis4Configs = new ConfigObjectArray(basicElementFactory->extendedAxis4ConfigMetaObj(), this);
    m_diConfigs = new ConfigObjectArray(basicElementFactory->ioConfigMetaObj(), this);
    m_doConfigs = new ConfigObjectArray(basicElementFactory->ioConfigMetaObj(), this);
    m_cylConfigs = new ConfigObjectArray(&CylinderConfig::staticMetaObject, this);
    m_vacuumConfigs = new ConfigObjectArray(&VacuumConfig::staticMetaObject, this);
    m_singleAxisModuleConfigs = new ConfigObjectArray(&SingleAxisConfig::staticMetaObject, this);
    m_xyModuleConfigs = new ConfigObjectArray(&XYModuleConfig::staticMetaObject, this);
    m_xyzModuleConfigs = new ConfigObjectArray(&XYZModuleConfig::staticMetaObject, this);
    m_softLandingConfigs = new ConfigObjectArray(&SoftLandingPos::staticMetaObject, this);

    axisConfigsFile = new ConfigFile("axisConfigs", m_axisConfigs, motionConfigFileDir + axisConfigsFileName);
    extendedAxisConfigsFile = new ConfigFile("extendedAxisConfigs", m_extendedAxisConfigs, motionConfigFileDir + extendedAxisConfigsFileName);
    extendedAxis2ConfigsFile = new ConfigFile("extendedAxis2Configs", m_extendedAxis2Configs, motionConfigFileDir + extendedAxis2ConfigsFileName);
    extendedAxis3ConfigsFile = new ConfigFile("extendedAxis3Configs", m_extendedAxis3Configs, motionConfigFileDir + extendedAxis3ConfigsFileName);
    extendedAxis4ConfigsFile = new ConfigFile("extendedAxis4Configs", m_extendedAxis4Configs, motionConfigFileDir + extendedAxis4ConfigsFileName);
    diConfigsFile = new ConfigFile("diConfigs", m_diConfigs, motionConfigFileDir + diConfigsFileName);
    doConfigsFile = new ConfigFile("doConfigs", m_doConfigs, motionConfigFileDir + doConfigsFileName);
    cylConfigsFile = new ConfigFile("cylinderConfigs", m_cylConfigs, motionConfigFileDir + cylConfigsFileName);
    vacuumConfigsFile = new ConfigFile("vacuumConfigs", m_vacuumConfigs, motionConfigFileDir + vacuumConfigsFileName);
    singleAxisModuleConfigsFile
        = new ConfigFile("singleAxisModuleConfigs", m_singleAxisModuleConfigs, motionConfigFileDir + singleAxisModuleConfigsFileName);
    xyModuleConfigsFile = new ConfigFile("xyModuleConfigs", m_xyModuleConfigs, motionConfigFileDir + xyModuleConfigsFileName);
    xyzModuleConfigsFile = new ConfigFile("xyzModuleConfigs", m_xyzModuleConfigs, motionConfigFileDir + xyzModuleConfigsFileName);

    axisConfigsFile->populate();
    extendedAxisConfigsFile->populate(true);
    extendedAxis2ConfigsFile->populate(true);
    extendedAxis3ConfigsFile->populate(true);
    extendedAxis4ConfigsFile->populate(true);
    diConfigsFile->populate();
    doConfigsFile->populate();
    cylConfigsFile->populate();
    vacuumConfigsFile->populate();
    singleAxisModuleConfigsFile->populate();
    xyModuleConfigsFile->populate();
    xyzModuleConfigsFile->populate();

    m_axisConfigs->setUIAddRemovable(false);
    m_extendedAxisConfigs->setUIAddRemovable(false);
    m_extendedAxis2Configs->setUIAddRemovable(false);
    m_extendedAxis3Configs->setUIAddRemovable(false);
    m_extendedAxis4Configs->setUIAddRemovable(false);
    m_diConfigs->setUIAddRemovable(false);
    m_doConfigs->setUIAddRemovable(false);
    m_cylConfigs->setUIAddRemovable(false);
    m_vacuumConfigs->setUIAddRemovable(false);
    m_singleAxisModuleConfigs->setUIAddRemovable(false);
    m_xyModuleConfigs->setUIAddRemovable(false);
    m_xyzModuleConfigs->setUIAddRemovable(false);
    m_softLandingConfigs->setUIAddRemovable(false);

    config2Dic();

    m_collisionGroupConfigs = new ConfigObjectArray(&CollisionGroup::staticMetaObject, this);

    collisionGroupConfigsFile = new ConfigFile("collisionGroups", m_collisionGroupConfigs, motionConfigFileDir + collisionGroupConfigsFileName);
}

void MotionConfigManager::createLostConfig(MotionElementDefinition *med)
{
    for (int i = 0; i < med->axisDefinitions()->count(); i++)
    {
        AxisDefinition *axisDfn = med->axisDefinitions()->getConfig<AxisDefinition>(i);
        auto axisType = axisDfn->axisType();
        if (!axisConfigMap.contains(axisDfn->axisName()))
        {
            AxisConfig *axisConfig = basicElementFactory->createAxisConfig(axisDfn->axisType(), m_axisConfigs);
            axisConfig->setName(axisDfn->axisName());
            axisConfigMap[axisDfn->axisName()] = axisConfig;
            switch (axisType)
            {
                case AxisDefinition::GeneralAxis:
                {
                    m_axisConfigs->executeAddConfigObject(0, axisConfig);
                    break;
                }
                case AxisDefinition::ExtendedAxis:
                {
                    m_extendedAxisConfigs->executeAddConfigObject(0, axisConfig);
                    break;
                }
                case AxisDefinition::ExtendedAxis2:
                {
                    m_extendedAxis2Configs->executeAddConfigObject(0, axisConfig);
                    break;
                }
                case AxisDefinition::ExtendedAxis3:
                {
                    m_extendedAxis3Configs->executeAddConfigObject(0, axisConfig);
                    break;
                }
                case AxisDefinition::ExtendedAxis4:
                {
                    m_extendedAxis4Configs->executeAddConfigObject(0, axisConfig);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < med->diNames()->count(); i++)
    {
        QString diName = med->diNames()->at(i).toString();
        if (!diConfigMap.contains(diName))
        {
            IOConfig *diConfig = basicElementFactory->createIOConfig(m_diConfigs);
            diConfig->setName(diName);
            diConfigMap[diName] = diConfig;
            m_diConfigs->executeAddConfigObject(0, diConfig);
        }
    }
    for (int i = 0; i < med->doNames()->count(); i++)
    {
        QString doName = med->doNames()->at(i).toString();
        if (!doConfigMap.contains(doName))
        {
            IOConfig *doConfig = basicElementFactory->createIOConfig(m_doConfigs);
            doConfig->setName(doName);
            doConfigMap[doName] = doConfig;
            m_doConfigs->executeAddConfigObject(0, doConfig);
        }
    }
    for (int i = 0; i < med->cylNames()->count(); i++)
    {
        QString cylName = med->cylNames()->at(i).toString();
        if (!cylConfigMap.contains(cylName))
        {
            CylinderConfig *cylConfig = new CylinderConfig;
            cylConfig->setName(cylName);
            cylConfigMap[cylName] = cylConfig;
            m_cylConfigs->executeAddConfigObject(0, cylConfig);
        }
    }
    for (int i = 0; i < med->vacuumNames()->count(); i++)
    {
        QString vacuumName = med->vacuumNames()->at(i).toString();
        if (!vacuumConfigMap.contains(vacuumName))
        {
            VacuumConfig *vacuumConfig = new VacuumConfig;
            vacuumConfig->setName(vacuumName);
            vacuumConfigMap[vacuumName] = vacuumConfig;
            m_vacuumConfigs->executeAddConfigObject(0, vacuumConfig);
        }
    }
    for (int i = 0; i < med->softlandingDefinition()->count(); i++)
    {
        auto softlandingDefinition = med->softlandingDefinition()->getConfig<SoftlandingDefinition>(i);
        if (axisConfigMap.contains(softlandingDefinition->axisName()))
        {
            axisConfigMap[softlandingDefinition->axisName()]->createLostPos(softlandingDefinition);
        }
        else
        {
            qFatal("Undefined axis name: %s", softlandingDefinition->axisName().toUtf8().data());
        }
    }
    createLostAxisModuleConfig(med);
}

void MotionConfigManager::setConfigOptions(MotionElementDefinition *med)
{
    for (int i = 0; i < med->cylNames()->count(); i++)
    {
        QString cylName = med->cylNames()->at(i).toString();
        cylConfigMap[cylName]->setOptionalIO(med->diNames()->members(), med->doNames()->members());
    }
    for (int i = 0; i < med->vacuumNames()->count(); i++)
    {
        QString vacuumName = med->vacuumNames()->at(i).toString();
        vacuumConfigMap[vacuumName]->setOptionalIO(med->diNames()->members(), med->doNames()->members());
    }
    for (int i = 0; i < med->axisDefinitions()->count(); i++)
    {
        QString axisName = med->axisDefinitions()->getConfig<AxisDefinition>(i)->axisName();
        axisConfigMap[axisName]->setOptionalIOName(med->diNames()->members(), med->doNames()->members());
    }
    QVariantList tmp_axisNames;
    for (int i = 0; i < med->axisDefinitions()->count(); i++)
    {
        tmp_axisNames.append(med->axisDefinitions()->getConfig<AxisDefinition>(i)->axisName());
    }
    for (int i = 0; i < med->axisModules()->count(); i++)
    {
        AxisModuleDefinition *amd = med->axisModules()->getConfig<AxisModuleDefinition>(i);
        MotionElement::Type axisModuleType = MotionElement::TypeEnumInfo().nameToEnum(amd->moduleType());
        getModuleConfig(amd->moduleName(), axisModuleType)->setOptionalAxis(tmp_axisNames);
    }
}

void MotionConfigManager::removeRedundantConfig(ConfigObjectArray *meds)
{
#define REMOVE_REDUNDANT_CONFIG(configs, configType, definedNames, configMap, key)                                                                   \
    {                                                                                                                                                \
        for (int i = configs->count() - 1; i >= 0; i--)                                                                                              \
        {                                                                                                                                            \
            auto config = configs->getConfig<configType>(i);                                                                                         \
            if (!definedNames.contains(config->key()))                                                                                               \
            {                                                                                                                                        \
                configs->remove(i);                                                                                                                  \
                if (configMap.contains(config->key()))                                                                                               \
                {                                                                                                                                    \
                    configMap.remove(config->key());                                                                                                 \
                }                                                                                                                                    \
            }                                                                                                                                        \
        }                                                                                                                                            \
    }

    QStringList definedAxisNames;
    QStringList definedExtendedAxisNames;
    QStringList definedExtendedAxis2Names;
    QStringList definedExtendedAxis3Names;
    QStringList definedExtendedAxis4Names;
    QStringList definedDiNames;
    QStringList definedDoNames;
    QStringList definedCylNames;
    QStringList definedVacuumNames;
    QStringList definedSAxisModuleNames;
    QStringList definedXyModuleNames;
    QStringList definedXyzModuleNames;
    for (int i = 0; i < meds->count(); i++)
    {
        MotionElementDefinition *med = meds->getConfig<MotionElementDefinition>(i);

        auto axisDefinitions = med->axisDefinitions();
        for (int j = 0; j < axisDefinitions->count(); j++)
        {
            auto axisDefinition = axisDefinitions->getConfig<AxisDefinition>(j);
            switch (axisDefinition->axisType())
            {
                case AxisDefinition::GeneralAxis:
                {
                    definedAxisNames.append(axisDefinition->axisName());
                    break;
                }
                case AxisDefinition::ExtendedAxis:
                {
                    definedExtendedAxisNames.append(axisDefinition->axisName());
                    break;
                }
                case AxisDefinition::ExtendedAxis2:
                {
                    definedExtendedAxis2Names.append(axisDefinition->axisName());
                    break;
                }
                case AxisDefinition::ExtendedAxis3:
                {
                    definedExtendedAxis3Names.append(axisDefinition->axisName());
                    break;
                }
                case AxisDefinition::ExtendedAxis4:
                {
                    definedExtendedAxis4Names.append(axisDefinition->axisName());
                    break;
                }
            }
        }

        definedDiNames.append(unpackVariantList<QString>(med->diNames()->members()));
        definedDoNames.append(unpackVariantList<QString>(med->doNames()->members()));
        definedVacuumNames.append(unpackVariantList<QString>(med->vacuumNames()->members()));
        definedCylNames.append(unpackVariantList<QString>(med->cylNames()->members()));

        auto axisModules = med->axisModules();
        for (int j = 0; j < axisModules->count(); j++)
        {
            auto axisModule = axisModules->getConfig<AxisModuleDefinition>(j);
            if (axisModule->moduleType() == MotionElement::TypeEnumInfo().enumToName(MotionElement::SingleAxis))
            {
                definedSAxisModuleNames.append(axisModule->moduleName());
            }
            if (axisModule->moduleType() == MotionElement::TypeEnumInfo().enumToName(MotionElement::XYModule))
            {
                definedXyModuleNames.append(axisModule->moduleName());
            }
            if (axisModule->moduleType() == MotionElement::TypeEnumInfo().enumToName(MotionElement::XYZModule))
            {
                definedXyzModuleNames.append(axisModule->moduleName());
            }
        }
    }

    REMOVE_REDUNDANT_CONFIG(m_axisConfigs, AxisConfig, definedAxisNames, axisConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_extendedAxisConfigs, AxisConfig, definedExtendedAxisNames, axisConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_extendedAxis2Configs, AxisConfig, definedExtendedAxis2Names, axisConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_extendedAxis3Configs, AxisConfig, definedExtendedAxis3Names, axisConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_extendedAxis4Configs, AxisConfig, definedExtendedAxis4Names, axisConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_diConfigs, IOConfig, definedDiNames, diConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_doConfigs, IOConfig, definedDoNames, doConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_vacuumConfigs, VacuumConfig, definedVacuumNames, vacuumConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_cylConfigs, CylinderConfig, definedCylNames, cylConfigMap, name);
    REMOVE_REDUNDANT_CONFIG(m_singleAxisModuleConfigs, SingleAxisConfig, definedSAxisModuleNames, sAxisConfigMap, moduleName);
    REMOVE_REDUNDANT_CONFIG(m_xyModuleConfigs, XYModuleConfig, definedXyModuleNames, xyModuleConfigMap, moduleName);
    REMOVE_REDUNDANT_CONFIG(m_xyzModuleConfigs, XYZModuleConfig, definedXyzModuleNames, xyzModuleConfigMap, moduleName);

    for (int i = 0; i < meds->count(); i++)
    {
        MotionElementDefinition *med = meds->getConfig<MotionElementDefinition>(i);
        for (int j = 0; j < med->softlandingDefinition()->count(); j++)
        {
            auto softlandingDefinition = med->softlandingDefinition()->getConfig<SoftlandingDefinition>(j);
            if (axisConfigMap.contains(softlandingDefinition->axisName()))
            {
                axisConfigMap[softlandingDefinition->axisName()]->removeRedundantPos(softlandingDefinition);
            }
        }

        for (int j = 0; j < med->axisModules()->count(); j++)
        {
            AxisModuleDefinition *amd = med->axisModules()->getConfig<AxisModuleDefinition>(j);
            MotionElement::Type axisModuleType = MotionElement::TypeEnumInfo().nameToEnum(amd->moduleType());
            switch (axisModuleType)
            {
                case MotionElement::SingleAxis:
                {
                    if (sAxisConfigMap.contains(amd->moduleName()))
                    {
                        sAxisConfigMap[amd->moduleName()]->removeRedundantPos(amd);
                    }
                    break;
                }
                case MotionElement::XYModule:
                {
                    if (xyModuleConfigMap.contains(amd->moduleName()))
                    {
                        xyModuleConfigMap[amd->moduleName()]->removeRedundantPos(amd);
                    }
                    break;
                }
                case MotionElement::XYZModule:
                {
                    if (xyzModuleConfigMap.contains(amd->moduleName()))
                    {
                        xyzModuleConfigMap[amd->moduleName()]->removeRedundantPos(amd);
                    }
                    break;
                }
                default:
                {
                    qFatal("Unknown axis module type: %s", amd->moduleType().toUtf8().data());
                }
            }
        }
    }
}

void MotionConfigManager::initMoveProtectionConfig()
{
    GlobalMotionElements::getIns().setAxisNames(toVariantList(axisConfigMap.keys()));
    GlobalMotionElements::getIns().setCylNames(toVariantList(cylConfigMap.keys()));
    GlobalMotionElements::getIns().setDiNames(toVariantList(diConfigMap.keys()));

    collisionGroupConfigsFile->populate();
}

void MotionConfigManager::shareAllMotionConfig()
{
    foreach (auto axisConfig, axisConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("AxisConfig_%1").arg(axisConfig->name()), axisConfig);
    }
    foreach (auto diConfig, diConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("DIConfig_%1").arg(diConfig->name()), diConfig);
    }
    foreach (auto doConfig, doConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("DOConfig_%1").arg(doConfig->name()), doConfig);
    }
    foreach (auto cylConfig, cylConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("CylConfig_%1").arg(cylConfig->name()), cylConfig);
    }
    foreach (auto vacuumConfig, vacuumConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("VacuumConfig_%1").arg(vacuumConfig->name()), vacuumConfig);
    }
    foreach (auto sAxisConfig, sAxisConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("SingleAxisConfig_%1").arg(sAxisConfig->moduleName()), sAxisConfig);
    }
    foreach (auto xyModuleConfig, xyModuleConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("XYModuleConfig_%1").arg(xyModuleConfig->moduleName()), xyModuleConfig);
    }
    foreach (auto xyzModuleConfig, xyzModuleConfigMap.values())
    {
        ConfigShareServer::getIns().share(QString("XYZModuleConfig_%1").arg(xyzModuleConfig->moduleName()), xyzModuleConfig);
    }

    ConfigShareServer::getIns().share("CollisionGroupConfigs", m_collisionGroupConfigs);
}

void MotionConfigManager::extractSoftLandingPos()
{
#define EXTRACT_SOFTLANDING_POS(configsObj)                                                                                                          \
    for (int i = 0; i < configsObj->count(); i++)                                                                                                    \
    {                                                                                                                                                \
        auto axisConfig = configsObj->getConfig<AxisConfig>(i);                                                                                      \
        for (int j = 0; j < axisConfig->softLandingPoses()->count(); j++)                                                                            \
        {                                                                                                                                            \
            auto softLandingPos = axisConfig->softLandingPoses()->getConfig<SoftLandingPos>(j);                                                      \
            m_softLandingConfigs->executeAddConfigObject(m_softLandingConfigs->count(), softLandingPos);                                             \
        }                                                                                                                                            \
    }

    EXTRACT_SOFTLANDING_POS(m_axisConfigs);
    EXTRACT_SOFTLANDING_POS(m_extendedAxisConfigs);
    EXTRACT_SOFTLANDING_POS(m_extendedAxis2Configs);
    EXTRACT_SOFTLANDING_POS(m_extendedAxis3Configs);
    EXTRACT_SOFTLANDING_POS(m_extendedAxis4Configs);
}

void MotionConfigManager::setGlobalVelocityRatio(double globalVelocityRatio)
{
    for (auto axisConfig : axisConfigMap.values())
    {
        if (axisConfig->advancedAxisConfig()->respondToGlobalVelRatioChange())
        {
            axisConfig->setVelocityRatio(globalVelocityRatio);
        }
    }
}

void MotionConfigManager::setAxisVelocityRatio(QString axisName, double ratio)
{
    axisConfigMap[axisName]->setVelocityRatio(ratio);
}

AxisModuleConfig *MotionConfigManager::getModuleConfig(QString moduleName, MotionElement::Type moduleType)
{
    AxisModuleConfig *axisModuleConfig = nullptr;
    switch (moduleType)
    {
        case MotionElement::SingleAxis:
        {
            axisModuleConfig = sAxisConfigMap[moduleName];
            break;
        }
        case MotionElement::XYModule:
        {
            axisModuleConfig = xyModuleConfigMap[moduleName];
            break;
        }
        case MotionElement::XYZModule:
        {
            axisModuleConfig = xyzModuleConfigMap[moduleName];
            break;
        }
        default:
        {
            throw SilicolAbort(
                tr("Unknown axis module, Name: %1, Type: %2").arg(moduleName).arg(MotionElement::TypeEnumInfo().enumToName(moduleType)), EX_LOCATION);
        }
    }
    return axisModuleConfig;
}

void MotionConfigManager::config2Dic()
{
#define GENERAL_CONFIG_TO_DIC(configsObj, ConfigType, dic)                                                                                           \
    for (int i = 0; i < configsObj->count(); i++)                                                                                                    \
    {                                                                                                                                                \
        ConfigType *config = configsObj->getConfig<ConfigType>(i);                                                                                   \
        dic[config->name()] = config;                                                                                                                \
    }

#define AXIS_MODULE_CONFIG_TO_DIC(configsObj, ConfigType, dic)                                                                                       \
    for (int i = 0; i < configsObj->count(); i++)                                                                                                    \
    {                                                                                                                                                \
        ConfigType *axisModuleConfig = configsObj->getConfig<ConfigType>(i);                                                                         \
        axisModuleConfig->modulePos2Dic();                                                                                                           \
        dic[axisModuleConfig->moduleName()] = axisModuleConfig;                                                                                      \
    }

    GENERAL_CONFIG_TO_DIC(m_axisConfigs, AxisConfig, axisConfigMap);
    GENERAL_CONFIG_TO_DIC(m_extendedAxisConfigs, AxisConfig, axisConfigMap);
    GENERAL_CONFIG_TO_DIC(m_extendedAxis2Configs, AxisConfig, axisConfigMap);
    GENERAL_CONFIG_TO_DIC(m_extendedAxis3Configs, AxisConfig, axisConfigMap);
    GENERAL_CONFIG_TO_DIC(m_extendedAxis4Configs, AxisConfig, axisConfigMap);

    foreach (auto axisConfig, axisConfigMap.values())
    {
        axisConfig->pos2Dic();
    }

    GENERAL_CONFIG_TO_DIC(m_diConfigs, IOConfig, diConfigMap);
    GENERAL_CONFIG_TO_DIC(m_doConfigs, IOConfig, doConfigMap);
    GENERAL_CONFIG_TO_DIC(m_cylConfigs, CylinderConfig, cylConfigMap);
    GENERAL_CONFIG_TO_DIC(m_vacuumConfigs, VacuumConfig, vacuumConfigMap);

    AXIS_MODULE_CONFIG_TO_DIC(m_singleAxisModuleConfigs, SingleAxisConfig, sAxisConfigMap);
    AXIS_MODULE_CONFIG_TO_DIC(m_xyModuleConfigs, XYModuleConfig, xyModuleConfigMap);
    AXIS_MODULE_CONFIG_TO_DIC(m_xyzModuleConfigs, XYZModuleConfig, xyzModuleConfigMap);
}

void MotionConfigManager::createLostAxisModuleConfig(MotionElementDefinition *med)
{
    for (int i = 0; i < med->axisModules()->count(); i++)
    {
        AxisModuleDefinition *amd = med->axisModules()->getConfig<AxisModuleDefinition>(i);
        MotionElement::Type axisModuleType = MotionElement::TypeEnumInfo().nameToEnum(amd->moduleType());
        switch (axisModuleType)
        {
            case MotionElement::SingleAxis:
            {
                if (!sAxisConfigMap.contains(amd->moduleName()))
                {
                    SingleAxisConfig *sAxisConfig = new SingleAxisConfig(this);
                    sAxisConfig->setModuleName(amd->moduleName());
                    sAxisConfigMap[amd->moduleName()] = sAxisConfig;
                    m_singleAxisModuleConfigs->executeAddConfigObject(0, sAxisConfig);
                }
                sAxisConfigMap[amd->moduleName()]->createLostPos(amd);
                break;
            }
            case MotionElement::XYModule:
            {
                if (!xyModuleConfigMap.contains(amd->moduleName()))
                {
                    XYModuleConfig *xyModuleConfig = new XYModuleConfig(this);
                    xyModuleConfig->setModuleName(amd->moduleName());
                    xyModuleConfigMap[amd->moduleName()] = xyModuleConfig;
                    m_xyModuleConfigs->executeAddConfigObject(0, xyModuleConfig);
                }
                xyModuleConfigMap[amd->moduleName()]->createLostPos(amd);
                break;
            }
            case MotionElement::XYZModule:
            {
                if (!xyzModuleConfigMap.contains(amd->moduleName()))
                {
                    XYZModuleConfig *xyzModuleConfig = new XYZModuleConfig(this);
                    xyzModuleConfig->setModuleName(amd->moduleName());
                    xyzModuleConfigMap[amd->moduleName()] = xyzModuleConfig;
                    m_xyzModuleConfigs->executeAddConfigObject(0, xyzModuleConfig);
                }
                xyzModuleConfigMap[amd->moduleName()]->createLostPos(amd);
                break;
            }
            default:
            {
                qFatal("Unknown axis module type: %s", amd->moduleType().toUtf8().data());
            }
        }
    }
}
