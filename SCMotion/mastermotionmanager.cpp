#include "mastermotionmanager.h"

MasterMotionManager::MasterMotionManager(BasicElementFactory *basicElementFactory, QString dutRelatedConfigFileDir, QObject *parent)
    : ObjectLivedThreadInstructionExecutor(parent)
{
    m_motionConfigManager = new MotionConfigManager(basicElementFactory, dutRelatedConfigFileDir, this);
    m_meds = new ConfigObjectArray(&MotionElementDefinition::staticMetaObject, this);
    medsConfigFile = new ConfigFile("MotionElementDefinitions", m_meds, medsConfigFileName, false);
    medsConfigFile->populate(true);
    m_motionConfigManager->removeRedundantConfig(m_meds);
    for (int i = 0; i < m_meds->count(); i++)
    {
        MotionElementDefinition *med = m_meds->getConfig<MotionElementDefinition>(i);
        m_motionConfigManager->createLostConfig(med);
        m_motionConfigManager->setConfigOptions(med);

        auto node = new QRemoteObjectNode();
        node->connectToNode(med->address());
        auto replica = node->acquire<MotionManagerReplica>();
        replica->setProperty("serverAddress", med->address());
        nodes.append(node);
        motionManagerReplicas.append(replica);
    }
    XYZRDebuggerManager::getIns().setOptionalAxisNames(toVariantList(m_motionConfigManager->axisNames()));
    m_motionConfigManager->extractSoftLandingPos();
    m_motionConfigManager->initMoveProtectionConfig();
    m_motionConfigManager->shareAllMotionConfig();
    m_motionStatePublisher = new MotionStatePublisher(m_motionConfigManager->cylNames(), m_motionConfigManager->vacuumNames(),
                                                      m_motionConfigManager->diNames(), m_motionConfigManager->doNames(), this);
    subscribePosReq();

    meUILayouts = new ConfigObjectArray(&MotionElementUILayout::staticMetaObject, this);
    meUILayoutsConfigFile = new ConfigFile("motionElementUILayout", meUILayouts, meUILayoutsConfigFileName, false);
    meUILayoutsConfigFile->populate(true);
    checkMotionElementUILayoutConfig();
}

MasterMotionManager::~MasterMotionManager()
{
    while (motionManagerReplicas.count() > 0)
    {
        auto replica = motionManagerReplicas.takeFirst();
        delete replica;
    }
    while (nodes.count() > 0)
    {
        auto node = nodes.takeFirst();
        delete node;
    }
}

void MasterMotionManager::setContextProperty(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("masterMotionManager", this);
    engine.rootContext()->setContextProperty("motionConfigManager", m_motionConfigManager);
    engine.rootContext()->setContextProperty("motionStatePublisher", m_motionStatePublisher);
    engine.rootContext()->setContextProperty("motionElementDefs", m_meds);
    engine.rootContext()->setContextProperty("motionManager", &MotionManager::getIns());
    engine.rootContext()->setContextProperty("collisionGroupConfigs", m_motionConfigManager->m_collisionGroupConfigs);
    engine.rootContext()->setContextProperty("diConfigs", m_motionConfigManager->m_diConfigs);
    engine.rootContext()->setContextProperty("doConfigs", m_motionConfigManager->m_doConfigs);
    engine.rootContext()->setContextProperty("cylConfigs", m_motionConfigManager->m_cylConfigs);
    engine.rootContext()->setContextProperty("vacuumConfigs", m_motionConfigManager->m_vacuumConfigs);
    engine.rootContext()->setContextProperty("axisConfigs", m_motionConfigManager->m_axisConfigs);
    engine.rootContext()->setContextProperty("extendedAxisConfigs", m_motionConfigManager->m_extendedAxisConfigs);
    engine.rootContext()->setContextProperty("extendedAxis2Configs", m_motionConfigManager->m_extendedAxis2Configs);
    engine.rootContext()->setContextProperty("extendedAxis3Configs", m_motionConfigManager->m_extendedAxis3Configs);
    engine.rootContext()->setContextProperty("extendedAxis4Configs", m_motionConfigManager->m_extendedAxis4Configs);
    engine.rootContext()->setContextProperty("singleAxisModuleConfigs", m_motionConfigManager->m_singleAxisModuleConfigs);
    engine.rootContext()->setContextProperty("xyModuleConfigs", m_motionConfigManager->m_xyModuleConfigs);
    engine.rootContext()->setContextProperty("xyzModuleConfigs", m_motionConfigManager->m_xyzModuleConfigs);
    engine.rootContext()->setContextProperty("softLandingConfigs", m_motionConfigManager->m_softLandingConfigs);
}

void MasterMotionManager::generateMotionElementUILayoutConfigFile()
{
    if (!UIOperation::getIns()->okCancelConfirm(tr("此操作将自动生成%1. \r\n若该文件已存在，将被覆盖. \r\n请确认！").arg(meUILayoutsConfigFileName)))
    {
        return;
    }
    QStringList t_diNames, t_doNames, t_axisNames, t_vacuumNames, t_cylNames;
    for (int i = 0; i < m_meds->count(); i++)
    {
        MotionElementDefinition *med = m_meds->getConfig<MotionElementDefinition>(i);
        t_diNames.append(unpackVariantList<QString>(med->diNames()->members()));
        t_doNames.append(unpackVariantList<QString>(med->doNames()->members()));
        t_vacuumNames.append(unpackVariantList<QString>(med->vacuumNames()->members()));
        t_cylNames.append(unpackVariantList<QString>(med->cylNames()->members()));
        auto axisDefs = med->axisDefinitions();
        for (int j = 0; j < axisDefs->count(); j++)
        {
            auto axisDef = axisDefs->getConfig<AxisDefinition>(j);
            t_axisNames.append(axisDef->axisName());
        }
    }

    ConfigObjectArray *t_meUILayouts = new ConfigObjectArray(&MotionElementUILayout::staticMetaObject);
    ConfigFile *t_meUILayoutsConfigFile = new ConfigFile("motionElementUILayout", t_meUILayouts, meUILayoutsConfigFileName, false);
    t_meUILayoutsConfigFile->setAutoSave(false);
    addPage("DI", "DI", t_diNames, t_meUILayouts);
    addPage("DO", "DO", t_doNames, t_meUILayouts);
    addPage("Vacuum", "Vacuum", t_vacuumNames, t_meUILayouts);
    addPage("Cylinder", "Cylinder", t_cylNames, t_meUILayouts);
    const int MaxMotorForEachPage = 7;
    if (t_axisNames.count() <= MaxMotorForEachPage)
    {
        addPage("Axis", "Motor", t_axisNames, t_meUILayouts);
    }
    else
    {
        int motorPageCount = (t_axisNames.count() - 1) / MaxMotorForEachPage + 1;
        for (int i = 0; i < motorPageCount; i++)
        {
            addPage("Axis", QString("Motor%1").arg(i + 1),
                    t_axisNames.mid(i * MaxMotorForEachPage, qMin(MaxMotorForEachPage, t_axisNames.count() - i * MaxMotorForEachPage)),
                    t_meUILayouts);
        }
    }
    t_meUILayoutsConfigFile->save();
}

void MasterMotionManager::setAxisVelocityRatio(QString axisName, double ratio)
{
    m_motionConfigManager->setAxisVelocityRatio(axisName, ratio);
}

void MasterMotionManager::pause()
{
    runIns("pauseImpl");
}

void MasterMotionManager::unPause()
{
    runIns("unPauseImpl", QVariantList(), false);
}

void MasterMotionManager::handleAbort()
{
    runIns("abortImpl", QVariantList(), false);
}

void MasterMotionManager::handleReset()
{
    runIns("resetImpl", QVariantList(), false);
}

void MasterMotionManager::initMotionManagers()
{
    runIns("initMotionManagersImpl");
}

void MasterMotionManager::updateMoveProtections()
{
    foreach (auto motionManagerReplica, motionManagerReplicas)
    {
        if (!motionManagerReplica->isReplicaValid())
        {
            qCritical()
                << QString("Connection to motion manager server %1 is invalid!").arg(motionManagerReplica->property("serverAddress").toString());
            continue;
        }
        motionManagerReplica->updateMoveProtection();
    }
}

void MasterMotionManager::setMoveProtectionsEnable(bool enable)
{
    if (m_moveProtectionsEnable == enable)
    {
        return;
    }

    if (!enable && !UIOperation::getIns()->yesNoConfirm(tr("Disable move protection may cause dengerous behavior!\r\nAre you sure to do that?")))
    {
        emit moveProtectionsEnableChanged(m_moveProtectionsEnable);
        return;
    }
    foreach (auto motionManagerReplica, motionManagerReplicas)
    {
        if (!motionManagerReplica->isReplicaValid())
        {
            qCritical()
                << QString("Connection to motion manager server %1 is invalid!").arg(motionManagerReplica->property("serverAddress").toString());
            continue;
        }
        motionManagerReplica->setMoveProtectionEnable(enable);
    }
    m_moveProtectionsEnable = enable;
    emit moveProtectionsEnableChanged(m_moveProtectionsEnable);
    if (m_moveProtectionsEnable)
    {
        qCInfo(motionCate()) << tr("Enable move protection!");
    }
    else
    {
        qCWarning(motionCate()) << tr("Disable move protection!");
    }
}

void MasterMotionManager::setMotionStateReporters(bool enable)
{
    foreach (auto motionManagerReplica, motionManagerReplicas)
    {
        if (!motionManagerReplica->isReplicaValid())
        {
            qCritical()
                << QString("Connection to motion manager server %1 is invalid!").arg(motionManagerReplica->property("serverAddress").toString());
            continue;
        }
        motionManagerReplica->setMotionStateReporter(enable);
    }
}

void MasterMotionManager::onUpdatePosReq(QString moduleName, MotionElement::Type moduleType, QString posName)
{
    try
    {
        auto axesCurrentPos = MotionManager::getIns().runInstruction(moduleType, moduleName, "getModuleCurrentPos");
        motionConfigManager()->getModuleConfig(moduleName, moduleType)->getPos<ModulePos>(posName)->updatePosWithCurrentPos(axesCurrentPos.toMap());
    }
    catch (SilicoolException &se)
    {
        qCritical() << se.what();
    }
}

void MasterMotionManager::onMoveToReq(QString moduleName, MotionElement::Type moduleType, QString posName)
{
    QVariantList args;
    args.append(posName);
    MotionManager::getIns().executeInstruction(moduleType, moduleName, "moveTo", args, false);
}

void MasterMotionManager::onMeasureHeightReq(QString axisName, QString softLandingPosName, double vel, double force, int holdTime)
{
    QtConcurrent::run([this, axisName, softLandingPosName, vel, force, holdTime] {
        try
        {
            QVariantList args;
            args << vel << force << holdTime;
            auto pos = MotionManager::getIns().runInstruction(MotionElement::Axis, axisName, "measureHeight", args);
            if (UIOperation::getIns()->getUIResponse("Confirm",
                                                     tr("Are you sure to apply this config?\r\naxisName: %1, softLandingPosName: %2, targetPos: %3")
                                                         .arg(axisName)
                                                         .arg(softLandingPosName)
                                                         .arg(pos.toDouble()),
                                                     MsgBoxIcon::Question, YesNoBtns)
                == YesBtn)
            {
                auto axisConfig = (motionConfigManager()->axisConfigMap[axisName]);
                axisConfig->getPos(softLandingPosName)->setConfig("targetPos", pos);
            }
        }
        catch (SilicoolException &se)
        {
            qCritical() << se.what();
        }
    });
}

void MasterMotionManager::subscribeMotionState(QQuickItem *subscriber, QString functionName)
{
    for (int i = 0; i < subscriber->metaObject()->methodCount(); i++)
    {
        QMetaMethod slotMetaMethod = subscriber->metaObject()->method(i);
        if (slotMetaMethod.name() == functionName)
        {
            QMetaMethod signalMetaMethod = QMetaMethod::fromSignal(&MotionManagerReplica::reportMotionState);
            for (auto replica : motionManagerReplicas)
            {
                connect(replica, signalMetaMethod, subscriber, slotMetaMethod);
            }
            return;
        }
    }
    qCritical() << "Did not find function:" << functionName;
}

void MasterMotionManager::setGlobalVelocityRatio(double globalVelocityRatio)
{
    //        qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_globalVelocityRatio, globalVelocityRatio))
        return;

    m_globalVelocityRatio = globalVelocityRatio;
    emit globalVelocityRatioChanged(m_globalVelocityRatio);
    m_motionConfigManager->setGlobalVelocityRatio(globalVelocityRatio);
}

void MasterMotionManager::initMotionManagersImpl()
{
    QByteArray medsContent = m_meds->toJsonBinaryData("motionElementDefinitions");
    foreach (auto motionManagerReplica, motionManagerReplicas)
    {
        if (!motionManagerReplica->isReplicaValid())
        {
            throw SilicolAbort(
                tr("Connection to motion manager server %1 is invalid!").arg(motionManagerReplica->property("serverAddress").toString()));
        }
        auto reply = motionManagerReplica->initMotionManager(medsContent);
        if (!silicoolWait(
                30000, [&reply] { return reply.isFinished(); }, 30))
        {
            throw SilicolAbort(
                tr("Wait motion manager server response timeout! Server address: %1").arg(motionManagerReplica->property("serverAddress").toString()));
        }
        if (!reply.returnValue())
        {
            throw SilicolAbort(
                tr("Motion manager server init failed! Server address: %1").arg(motionManagerReplica->property("serverAddress").toString()));
        }
    }
}

void MasterMotionManager::abortImpl()
{
    foreach (auto motionManager, motionManagerReplicas)
    {
        motionManager->handleAbort();
    }
}

void MasterMotionManager::resetImpl()
{
    foreach (auto motionManager, motionManagerReplicas)
    {
        motionManager->handleReset();
    }
}

void MasterMotionManager::pauseImpl()
{
    foreach (auto motionManager, motionManagerReplicas)
    {
        motionManager->setIsPaused(true);
    }
    foreach (auto motionManager, motionManagerReplicas)
    {
        auto reply = motionManager->waitPaused();
        while (!silicoolWait(3000, [&reply] { return reply.isFinished(); }))
        {
            qCWarning(motionCate())
                << tr("Waiting motion manager paused... Server address: %1").arg(motionManager->property("serverAddress").toString());
        }
    }
}

void MasterMotionManager::unPauseImpl()
{
    foreach (auto motionManager, motionManagerReplicas)
    {
        motionManager->setIsPaused(false);
    }
}

void MasterMotionManager::checkMotionElementUILayoutConfig()
{
    for (int i = 0; i < meUILayouts->count(); i++)
    {
        auto meuil = meUILayouts->getConfig<MotionElementUILayout>(i);
        MotionElement::Type meType = MotionElement::TypeEnumInfo().nameToEnum(meuil->elementType());
        switch (meType)
        {
            case MotionElement::Axis:
            {
                auto axisNames = motionConfigManager()->axisNames();
                for (int j = meuil->pageElements()->count() - 1; j >= 0; j--)
                {
                    auto axisName = meuil->pageElements()->at(j).toString();
                    if (!axisNames.contains(axisName))
                    {
                        meuil->pageElements()->remove(j);
                        qCritical() << QString("Unknown axis name %s in MotionElementUILayout config file. Now auto "
                                               "delete it!")
                                           .arg(axisName);
                    }
                }
                break;
            }
            case MotionElement::DI:
            {
                auto diNames = motionConfigManager()->diNames();
                for (int j = meuil->pageElements()->count() - 1; j >= 0; j--)
                {
                    auto diName = meuil->pageElements()->at(j).toString();
                    if (!diNames.contains(diName))
                    {
                        meuil->pageElements()->remove(j);
                        qCritical() << QString("Unknown DI name %s in MotionElementUILayout config file. Now auto "
                                               "delete it!")
                                           .arg(diName);
                    }
                }
                break;
            }
            case MotionElement::DO:
            {
                auto doNames = motionConfigManager()->doNames();
                for (int j = meuil->pageElements()->count() - 1; j >= 0; j--)
                {
                    auto doName = meuil->pageElements()->at(j).toString();
                    if (!doNames.contains(doName))
                    {
                        meuil->pageElements()->remove(j);
                        qCritical() << QString("Unknown DO name %s in MotionElementUILayout config file. Now auto "
                                               "delete it!")
                                           .arg(doName);
                    }
                }
                break;
            }
            case MotionElement::Cylinder:
            {
                auto cylNames = motionConfigManager()->cylNames();
                for (int j = meuil->pageElements()->count() - 1; j >= 0; j--)
                {
                    auto cylName = meuil->pageElements()->at(j).toString();
                    if (!cylNames.contains(cylName))
                    {
                        meuil->pageElements()->remove(j);
                        qCritical() << QString("Unknown cylinder name %s in MotionElementUILayout config file. Now auto "
                                               "delete it!")
                                           .arg(cylName);
                    }
                }
                break;
            }
            case MotionElement::Vacuum:
            {
                auto vacuumNames = motionConfigManager()->vacuumNames();
                for (int j = meuil->pageElements()->count() - 1; j >= 0; j--)
                {
                    auto vacuumName = meuil->pageElements()->at(j).toString();
                    if (!vacuumNames.contains(vacuumName))
                    {
                        meuil->pageElements()->remove(j);
                        qCritical() << QString("Unknown vacuum name %s in MotionElementUILayout config file. Now auto "
                                               "delete it!")
                                           .arg(vacuumName);
                    }
                }
                break;
            }
            default:
            {
            }
        }
    }
}

void MasterMotionManager::subscribePosReq()
{
    foreach (auto moduleConfig, motionConfigManager()->sAxisConfigMap.values())
    {
        subscribeModulePosReq(moduleConfig);
    }
    foreach (auto moduleConfig, motionConfigManager()->xyModuleConfigMap.values())
    {
        subscribeModulePosReq(moduleConfig);
    }
    foreach (auto moduleConfig, motionConfigManager()->xyzModuleConfigMap.values())
    {
        subscribeModulePosReq(moduleConfig);
    }
    subscribeMeasureHeightReq(motionConfigManager()->m_axisConfigs);
    subscribeMeasureHeightReq(motionConfigManager()->m_extendedAxisConfigs);
    subscribeMeasureHeightReq(motionConfigManager()->m_extendedAxis2Configs);
    subscribeMeasureHeightReq(motionConfigManager()->m_extendedAxis3Configs);
    subscribeMeasureHeightReq(motionConfigManager()->m_extendedAxis4Configs);
}

void MasterMotionManager::subscribeModulePosReq(AxisModuleConfig *axisModuleConfig)
{
    foreach (auto posName, axisModuleConfig->modulePosNames())
    {
        auto modulePos = axisModuleConfig->getPos<ModulePos>(posName);
        connect(modulePos, &ModulePos::updatePosReq, this, &MasterMotionManager::onUpdatePosReq);
        connect(modulePos, &ModulePos::moveToReq, this, &MasterMotionManager::onMoveToReq);
    }
}

void MasterMotionManager::subscribeMeasureHeightReq(ConfigObjectArray *axisConfigs)
{
    for (int i = 0; i < axisConfigs->count(); i++)
    {
        auto *axisConfig = axisConfigs->getConfig<AxisConfig>(i);
        foreach (QString softLandingPosName, axisConfig->softLandingPosNames())
        {
            SoftLandingPos *softLandingPos = axisConfig->getPos(softLandingPosName);
            connect(softLandingPos, &SoftLandingPos::measureHeightReq, this, &MasterMotionManager::onMeasureHeightReq);
        }
    }
}

void MasterMotionManager::addPage(QString elementType, QString pageName, const QStringList &names, ConfigObjectArray *uiLayouts)
{
    MotionElementUILayout *page = new MotionElementUILayout();
    page->setElementType(elementType);
    page->setPageName(pageName);
    auto pageElements = page->pageElements();
    for (int i = 0; i < names.count(); i++)
    {
        pageElements->add(i);
        pageElements->setConfig(i, names[i]);
    }
    uiLayouts->executeAddConfigObject(uiLayouts->count(), page);
}
