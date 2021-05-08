#include "motionmanager.h"

InstructionExecutor::InstructionExecutor(MotionManager *motionManager,
                                         QString uuid,
                                         MotionElement::Type elementType,
                                         QString elementName,
                                         QString cmd,
                                         QVariantList args,
                                         bool sendbackResult)
    : motionManager(motionManager),
      uuid(uuid),
      elementType(elementType),
      elementName(elementName),
      cmd(cmd),
      args(args),
      sendbackResult(sendbackResult)
{
    setAutoDelete(true);
}

void InstructionExecutor::run()
{
    try
    {
        QObject *target = MotionElementContainer::getIns()->getItem(elementType, elementName);
        Instruction instruction(target, cmd, args);
        QVariant result = instruction.execute();
        if (sendbackResult)
        {
            emit motionManager->executeDone(uuid, "", result);
        }
    }
    catch (SilicoolException &se)
    {
        motionManager->handleError(uuid, se.what(), sendbackResult);
    }
}

MotionManager::MotionManager(QObject *parent) : MotionManagerSimpleSource(parent), motionStateReporter(200, this)
{
    connect(&MotionStateGetter::getIns(), &MotionStateGetter::reqGetAxisFeedbackPos, this, &MotionManager::getAxisFeedbackPos, Qt::DirectConnection);
    connect(&MotionStateGetter::getIns(), &MotionStateGetter::reqGetAxisTargetPos, this, &MotionManager::getAxisTargetPos, Qt::DirectConnection);
    connect(&MotionStateGetter::getIns(), &MotionStateGetter::reqGetCylinderCurrentState, this, &MotionManager::getCylinderCurrentState,
            Qt::DirectConnection);
    connect(&MotionStateGetter::getIns(), &MotionStateGetter::reqGetDiCurrentState, this, &MotionManager::getDiCurrentState, Qt::DirectConnection);

    collisionGroupsConfig = new ConfigObjectArray(&CollisionGroup::staticMetaObject, this);
    m_moveProtection = new MoveProtection(collisionGroupsConfig);

    connect(this, &MotionManager::executeDone, this, &MotionManager::instructionExecuteDone);
    connect(this, &MotionManager::instructionExecuteDone, this, &MotionManager::onInstructionExecuteDone);
    connect(&motionStateReporter, &MotionStateReporter::reportMotionState, this, &MotionManager::reportMotionState);
    threadPool.setMaxThreadCount(30);

    motionElementDefinitions = new ConfigObjectArray(&MotionElementDefinition::staticMetaObject, this);
    motionManagerConfig = new MotionManagerConfig;
    motionManagerConfigFile = new ConfigFile("MotionManagerConfig", motionManagerConfig, "config/motionManagerConfig.json", false);
    if (!motionManagerConfigFile->populate(true))
    {
        qFatal("Read motion manager config file failed! Software can not startup.");
    }
    host.setHostUrl(motionManagerConfig->localMotionManagerAddress());
    host.enableRemoting(this);
}

MotionManager::~MotionManager()
{
    dispose();
}

void MotionManager::dispose()
{
    emit beforeDisposing();
    DiStateMonitor::getIns().stopMonitor();
    motionStateReporter.stopReport();
    m_isInit = false;
    host.disableRemoting(this);

    foreach (auto cacheState, remoteDiStateCache.values())
    {
        delete cacheState;
    }
    remoteDiStateCache.clear();
    foreach (auto cacheState, remoteCylStateCache.values())
    {
        delete cacheState;
    }
    remoteCylStateCache.clear();
    foreach (auto cacheState, remoteAxisTargetPosCache.values())
    {
        delete cacheState;
    }
    remoteAxisTargetPosCache.clear();
    foreach (auto cacheState, remoteAxisFeedbackPosCache.values())
    {
        delete cacheState;
    }
    remoteAxisFeedbackPosCache.clear();

    deleteMotionNodeReplica();
    if (controlCard != nullptr)
    {
        controlCard->dispose();
        delete controlCard;
        controlCard = nullptr;
    }
}

void MotionManager::setBasicElementFactory(BasicElementFactory *basicElementFactory)
{
    this->basicElementFactory = basicElementFactory;
}

void MotionManager::attachBuzzerTowerLight(SCBuzzer *buzzer, SCTowerLight *towerLight)
{
    this->buzzer = buzzer;
    this->towerLight = towerLight;
}

QVariant MotionManager::getInstructionExecutionResult(QString uuid)
{
    if (!instructionExecuteResults.contains(uuid))
    {
        throw SilicolAbort(tr("Instruction with uuid %1 was not watched!").arg(uuid), EX_LOCATION);
    }
    InstructionExecuteResult *pier = &instructionExecuteResults[uuid];
    silicoolWait(
        -1, [pier] { return pier->isExecutionDone; }, 1);

    auto ier = instructionExecuteResults.take(uuid);
    if (!ier.errMsg.isEmpty())
    {
        throw SilicolAbort(ier.errMsg);
    }
    else
    {
        return ier.result;
    }
}

QVariant MotionManager::runInstruction(int elementType, QString elementName, QString cmd, QVariantList args)
{
    QString uuid = executeInstruction(elementType, elementName, cmd, args, true);
    return getInstructionExecutionResult(uuid);
}

QString MotionManager::executeInstruction(int elementType, QString elementName, QString cmd, QVariantList args, bool waitDone)
{
    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    if (waitDone)
    {
        instructionExecuteResults[uuid] = InstructionExecuteResult();
    }
    onExecuteInstruction(uuid, elementType, elementName, cmd, args, waitDone);
    return uuid;
}

void MotionManager::moveMultiAxes(QList<QString> axisNames, QList<double> targetPos, bool isAbsMove)
{
    QString cmd = isAbsMove ? "absMove" : "relMove";
    if (axisNames.count() != targetPos.count())
    {
        throw SilicolAbort("Error parameter!", EX_LOCATION);
    }
    QList<QString> uuids;
    for (int i = 0; i < axisNames.count(); i++)
    {
        QVariantList args;
        args << targetPos[i];
        uuids.append(executeInstruction(MotionElement::Axis, axisNames[i], cmd, args, true));
    }
    foreach (auto uuid, uuids)
    {
        getInstructionExecutionResult(uuid);
    }
}

void MotionManager::homeMotors(QStringList homeSeq)
{
    foreach (auto motors, homeSeq)
    {
        auto motorList = motors.split(",");
        QStringList uuids;
        foreach (auto motor, motorList)
        {
            uuids.append(executeInstruction(MotionElement::Axis, motor, "home", QVariantList(), true));
        }
        foreach (auto uuid, uuids)
        {
            getInstructionExecutionResult(uuid);
        }
    }
}

bool MotionManager::getLocalVacuumState(QString vacuumName)
{
    if(vacuumMap.contains(vacuumName))
    {
        return vacuumMap[vacuumName]->get();
    }
    qCCritical(motionCate()) << "Unknown local vacuum..." << vacuumName;
    return false;
}

bool MotionManager::getLocalDOState(QString doName)
{
    if(doMap.contains(doName))
    {
        return doMap[doName]->get();
    }
    qCCritical(motionCate()) << "Unknown local DO..." << doName;
    return false;
}

bool MotionManager::initMotionManager(QByteArray motionElementDefinition)
{
    if (m_isInit)
    {
        return true;
    }

    motionStateReporter.stopReport();
    if (basicElementFactory == nullptr)
    {
        qCritical() << "Please set basicElementFactory first!";
        return false;
    }
    if (controlCard == nullptr)
    {
        controlCard = basicElementFactory->createControlCard();
    }
    if (!motionElementDefinitions->fromJsonBinaryData(motionElementDefinition, "motionElementDefinitions"))
    {
        return false;
    }

    ConfigShareClient::getIns().bindToRemoteConfig("CollisionGroupConfigs", collisionGroupsConfig);
    deleteMotionNodeReplica();
    try
    {
        volatile bool controlCardInitDone = false;
        QString controlCardInitErrorMsg;
        QtConcurrent::run([this, &controlCardInitDone, &controlCardInitErrorMsg] {
            try
            {
                controlCard->preInit();
                controlCardInitDone = true;
            }
            catch (SilicoolException &se)
            {
                controlCardInitErrorMsg = se.what();
                controlCardInitDone = true;
            }
        });
        while (!controlCardInitDone)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
        }
        if (!controlCardInitErrorMsg.isEmpty())
        {
            throw SilicolAbort(controlCardInitErrorMsg);
        }

        for (int i = 0; i < motionElementDefinitions->count(); i++)
        {
            MotionElementDefinition *med = motionElementDefinitions->getConfig<MotionElementDefinition>(i);
            if (med->address() == motionManagerConfig->localMotionManagerAddress())
            {
                createMotionElement(med);
            }
            else
            {
                auto node = new QRemoteObjectNode(this);
                node->connectToNode(med->address());
                nodes.append(node);
                auto replica = node->acquire<MotionManagerReplica>();
                motionManagerReplicas[med->address()] = replica;
                connect(replica, &MotionManagerReplica::instructionExecuteDone, this, &MotionManager::onInstructionExecuteDone);
            }
            setMotionElementLocation(med);
        }

        ConfigShareClient::getIns().requestSynchronizeAllConfig();
        if (!silicoolWait(
                2000, [] { return ConfigShareClient::getIns().isSyncAllConfigRequestExecuteDone(); }, 20))
        {
            qCritical(motionCate()) << "Wait all config synchronization timeout!";
            return false;
        }
        if (!ConfigShareClient::getIns().syncAllConfigRequestExecuteResult())
        {
            return false;
        }

        controlCardInitDone = false;
        controlCardInitErrorMsg.clear();
        QtConcurrent::run([this, &controlCardInitDone, &controlCardInitErrorMsg] {
            try
            {
                controlCard->postInit();
                controlCardInitDone = true;
            }
            catch (SilicoolException &se)
            {
                controlCardInitErrorMsg = se.what();
                controlCardInitDone = true;
            }
        });
        while (!controlCardInitDone)
        {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
        }
        if (!controlCardInitErrorMsg.isEmpty())
        {
            throw SilicolAbort(controlCardInitErrorMsg);
        }

        foreach (auto scdi, diMap.values())
        {
            scdi->init();
        }
        foreach (auto scdo, doMap.values())
        {
            scdo->init();
        }
        foreach (auto cyl, cylMap.values())
        {
            cyl->init();
        }
        foreach (auto vacuum, vacuumMap.values())
        {
            vacuum->init();
        }
        foreach (auto axis, axisMap.values())
        {
            axis->init();
            axis->config()->pos2Dic();
        }

        HighPrecisionTimer::eventLoopSleep(4000);    // Wait motor first enable.

        foreach (auto axisModule, axisModuleMap.values())
        {
            axisModule->init();
        }

        for (int i = 0; i < motionElementDefinitions->count(); i++)
        {
            MotionElementDefinition *med = motionElementDefinitions->getConfig<MotionElementDefinition>(i);
            if (med->address() == motionManagerConfig->localMotionManagerAddress())
            {
                for (int j = 0; j < med->axisDefinitions()->count(); j++)
                {
                    auto axisDefinition = med->axisDefinitions()->getConfig<AxisDefinition>(j);
                    if (!axisDefinition->masterAxisName().isEmpty())
                    {
                        if (!axisMap.contains(axisDefinition->masterAxisName()))
                        {
                            throw SilicolAbort(tr("Try to bind axis %1 to %2, but %2 is undefined!")
                                                   .arg(axisDefinition->axisName())
                                                   .arg(axisDefinition->masterAxisName()),
                                               EX_LOCATION);
                        }
                        axisMap[axisDefinition->axisName()]->setMasterAxis(axisMap[axisDefinition->masterAxisName()]);
                        if (!axisMap[axisDefinition->axisName()]->bindToMasterAxis())
                        {
                            throw SilicolAbort(tr("Bind axis %1 to %2 failed!").arg(axisDefinition->axisName()).arg(axisDefinition->masterAxisName()),
                                               EX_LOCATION);
                        }
                    }
                }
            }
        }

        motionStateReporter.setElementToBeWatched(axisMap.values(), diMap.values(), doMap.values(), cylMap.values(), vacuumMap.values());
        m_moveProtection->update();
        if (startMotionStateReporter)
        {
            motionStateReporter.startReport();
        }
        DiStateMonitor::getIns().startMonitor();

        if (buzzer != nullptr)
        {
            buzzer->init();
        }
        if (towerLight != nullptr)
        {
            towerLight->init();
        }

        m_isInit = true;
        emit inited();
    }
    catch (SilicolAbort &sa)
    {
        qCritical() << sa.what();
    }
    return m_isInit;
}

void MotionManager::setIsPaused(bool isPaused)
{
    GlobalState::getIns().setIsPaused(isPaused);
}

bool MotionManager::waitPaused()
{
    foreach (auto axis, axisMap.values())
    {
        if (axis->isRunning() && !axis->hasBindedToMaterAxis())
        {
            qCInfo(motionCate()) << tr("Waiting axis %1 paused...").arg(axis->name());
            silicoolWait(
                -1, [axis] { return !axis->isRunning(); }, 20);
        }
    }
    return true;
}

void MotionManager::handleAbort()
{
    GlobalState::getIns().setIsAborted(true);
}

void MotionManager::handleReset()
{
    GlobalState::getIns().setIsPaused(false);
    GlobalState::getIns().setIsAborted(false);
}

void MotionManager::setMotionStateReporter(bool enable)
{
    startMotionStateReporter = enable;
    if (enable)
    {
        if (m_isInit)
        {
            motionStateReporter.startReport();
        }
    }
    else
    {
        motionStateReporter.stopReport();
    }
}

void MotionManager::setMoveProtectionEnable(bool enable)
{
    m_moveProtection->setEnable(enable);
}

void MotionManager::updateMoveProtection()
{
    if (m_isInit)
    {
        m_moveProtection->update();
    }
}

double MotionManager::getAxisCurrentPos(QString axisName)
{
    QString nomination = nominate(MotionElement::Axis, axisName);
    if (!elementLocations.contains(nomination))
    {
        qCritical() << "Undefined axis:" << axisName;
        return UNKNOWN_AXIS_POS;
    }
    if (elementLocations[nomination].isLocal)
    {
        if (axisMap[axisName]->hasHome())
        {
            return axisMap[axisName]->getCurrentOutputPos();
        }
        else
        {
            return UNKNOWN_AXIS_POS;
        }
    }
    else
    {
        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            qCritical() << QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        auto rsp = replica->getAxisCurrentPos(axisName);
        if (!rsp.waitForFinished(500))
        {
            qCritical() << QString("Get response from motion manager located in %1 timeout!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        return rsp.returnValue();
    }
}

double MotionManager::getAxisFeedbackPos(QString axisName, int cacheInvalidationTime)
{
    QString nomination = nominate(MotionElement::Axis, axisName);
    if (!elementLocations.contains(nomination))
    {
        qCritical() << "Undefined axis:" << axisName;
        return UNKNOWN_AXIS_POS;
    }
    if (elementLocations[nomination].isLocal)
    {
        if (axisMap[axisName]->hasHome())
        {
            return axisMap[axisName]->getFeedbackPos(cacheInvalidationTime);
        }
        else
        {
            return UNKNOWN_AXIS_POS;
        }
    }
    else
    {
        if (!remoteAxisFeedbackPosCache.contains(axisName))
        {
            remoteAxisFeedbackPosCache[axisName] = new CacheState();
        }
        auto cacheState = remoteAxisFeedbackPosCache[axisName];
        if (QDateTime::currentMSecsSinceEpoch() - cacheState->lastUpdateTimeStamp < cacheInvalidationTime)
        {
            return cacheState->state;
        }

        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            qCritical() << QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        auto rsp = replica->getAxisFeedbackPos(axisName, -1);
        if (!rsp.waitForFinished(500))
        {
            qCritical() << QString("Get response from motion manager located in %1 timeout!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        double axisFeedbackPos = rsp.returnValue();
        cacheState->state = axisFeedbackPos;
        cacheState->lastUpdateTimeStamp = QDateTime::currentMSecsSinceEpoch();
        return axisFeedbackPos;
    }
}

double MotionManager::getAxisTargetPos(QString axisName, int cacheInvalidationTime)
{
    QString nomination = nominate(MotionElement::Axis, axisName);
    if (!elementLocations.contains(nomination))
    {
        qCritical() << "Undefined axis:" << axisName;
        return UNKNOWN_AXIS_POS;
    }
    if (elementLocations[nomination].isLocal)
    {
        return axisMap[axisName]->getCurrentTargetPos();
    }
    else
    {
        if (!remoteAxisTargetPosCache.contains(axisName))
        {
            remoteAxisTargetPosCache[axisName] = new CacheState();
        }
        auto cacheState = remoteAxisTargetPosCache[axisName];
        if (QDateTime::currentMSecsSinceEpoch() - cacheState->lastUpdateTimeStamp < cacheInvalidationTime)
        {
            return cacheState->state;
        }

        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            qCritical() << QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        auto rsp = replica->getAxisTargetPos(axisName, -1);
        if (!rsp.waitForFinished(500))
        {
            qCritical() << QString("Get response from motion manager located in %1 timeout!").arg(elementLocations[nomination].address);
            return UNKNOWN_AXIS_POS;
        }
        double axisTargetPos = rsp.returnValue();
        cacheState->state = axisTargetPos;
        cacheState->lastUpdateTimeStamp = QDateTime::currentMSecsSinceEpoch();
        return axisTargetPos;
    }
}

int MotionManager::getCylinderCurrentState(QString cylName, int cacheInvalidationTime)
{
    QString nomination = nominate(MotionElement::Cylinder, cylName);
    if (!elementLocations.contains(nomination))
    {
        qCritical() << "Undefined cylinder:" << cylName;
        return SCCylinder::Moving;
    }
    if (elementLocations[nomination].isLocal)
    {
        return cylMap[cylName]->get();
    }
    else
    {
        if (!remoteCylStateCache.contains(cylName))
        {
            remoteCylStateCache[cylName] = new CacheState();
        }
        auto cacheState = remoteCylStateCache[cylName];
        if (QDateTime::currentMSecsSinceEpoch() - cacheState->lastUpdateTimeStamp < cacheInvalidationTime)
        {
            return static_cast<int>(cacheState->state);
        }

        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            qCritical() << QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            return SCCylinder::Moving;
        }
        auto rsp = replica->getCylinderCurrentState(cylName, -1);
        if (!rsp.waitForFinished(500))
        {
            qCritical() << QString("Get response from motion manager located in %1 timeout!").arg(elementLocations[nomination].address);
            return SCCylinder::Moving;
        }
        int cylCurrentState = rsp.returnValue();
        cacheState->state = cylCurrentState;
        cacheState->lastUpdateTimeStamp = QDateTime::currentMSecsSinceEpoch();
        return cylCurrentState;
    }
}

bool MotionManager::getDiCurrentState(QString diName, int cacheInvalidationTime)
{
    QString nomination = nominate(MotionElement::DI, diName);
    if (!elementLocations.contains(nomination))
    {
        qCritical() << "Undefined DI:" << diName;
        return false;
    }
    if (elementLocations[nomination].isLocal)
    {
        return diMap[diName]->get();
    }
    else
    {
        if (!remoteDiStateCache.contains(diName))
        {
            remoteDiStateCache[diName] = new CacheState();
        }
        auto cacheState = remoteDiStateCache[diName];
        if (QDateTime::currentMSecsSinceEpoch() - cacheState->lastUpdateTimeStamp < cacheInvalidationTime)
        {
            return static_cast<bool>(cacheState->state);
        }

        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            qCritical() << QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            return false;
        }
        auto rsp = replica->getDiCurrentState(diName, -1);
        if (!rsp.waitForFinished(500))
        {
            qCritical() << QString("Get response from motion manager located in %1 timeout!").arg(elementLocations[nomination].address);
            return false;
        }
        bool diCurrentState = rsp.returnValue();
        cacheState->state = diCurrentState;
        cacheState->lastUpdateTimeStamp = QDateTime::currentMSecsSinceEpoch();
        return diCurrentState;
    }
}

void MotionManager::onExecuteInstruction(QString uuid, int elementType, QString elementName, QString cmd, QVariantList args, bool sendbackResult)
{
    if (!m_isInit)
    {
        handleError(uuid, tr("MotionManager is not inited!"), sendbackResult);
    }

    MotionElement::Type meType = static_cast<MotionElement::Type>(elementType);
    QString nomination = nominate(meType, elementName);
    if (!elementLocations.contains(nomination))
    {
        QString errMsg = QString("Undefined element, type: %1, name: %2").arg(meType).arg(elementName);
        handleError(uuid, errMsg, sendbackResult);
        return;
    }

    if (elementLocations[nomination].isLocal)
    {
        InstructionExecutor *ie = new InstructionExecutor(this, uuid, meType, elementName, cmd, args, sendbackResult);
        threadPool.start(ie);
    }
    else
    {
        auto replica = motionManagerReplicas[elementLocations[nomination].address];
        if (!replica->isReplicaValid())
        {
            QString errMsg = QString("Connection to motion manager located in %1 is lost!").arg(elementLocations[nomination].address);
            handleError(uuid, errMsg, sendbackResult);
            return;
        }
        replica->onExecuteInstruction(uuid, elementType, elementName, cmd, args, sendbackResult);
    }
}

void MotionManager::stopAllAxis()
{
    if (!m_isInit)
    {
        return;
    }
    foreach (auto axis, axisMap.values())
    {
        axis->stop();
    }
}

void MotionManager::clearAllAxisAlarm()
{
    if (!m_isInit)
    {
        return;
    }
    foreach (auto axis, axisMap.values())
    {
        if (axis->hasAlarm())
        {
            executeInstruction(MotionElement::Axis, axis->name(), "clearAlarm");
        }
    }
}

void MotionManager::onInstructionExecuteDone(QString uuid, QString errMsg, QVariant result)
{
    if (instructionExecuteResults.contains(uuid))
    {
        auto ier = &(instructionExecuteResults[uuid]);
        ier->errMsg = errMsg;
        ier->result = result;
        ier->isExecutionDone = true;
    }
}

void MotionManager::createMotionElement(MotionElementDefinition *med)
{
    for (int i = 0; i < med->axisDefinitions()->count(); i++)
    {
        AxisDefinition *axisDfn = med->axisDefinitions()->getConfig<AxisDefinition>(i);
        if (axisMap.contains(axisDfn->axisName()))
        {
            continue;
        }
        SCAxis *axis;
        if (axisDfn->isDummyAxis())
        {
            axis = new DummyAxis(axisDfn->axisName(), this);
        }
        else
        {
            axis = basicElementFactory->createAxis(axisDfn->axisName(), axisDfn->axisType(), this);
        }
        axis->setMoveProtection(m_moveProtection);
        axisMap[axisDfn->axisName()] = axis;
        auto axisConfig = basicElementFactory->createAxisConfig(axisDfn->axisType(), axis);
        axisConfig->setName(axisDfn->axisName());
        axis->setConfig(axisConfig);
        ConfigShareClient::getIns().bindToRemoteConfig(QString("AxisConfig_%1").arg(axisDfn->axisName()), axisConfig);
    }
    for (int i = 0; i < med->diNames()->count(); i++)
    {
        QString diName = med->diNames()->at(i).toString();
        if (diMap.contains(diName))
        {
            continue;
        }
        auto scdi = basicElementFactory->createDI(diName, this);
        diMap[diName] = scdi;
        auto diConfig = basicElementFactory->createIOConfig(scdi);
        diConfig->setName(diName);
        scdi->setConfig(diConfig);
        ConfigShareClient::getIns().bindToRemoteConfig(QString("DIConfig_%1").arg(diName), diConfig);
    }
    for (int i = 0; i < med->doNames()->count(); i++)
    {
        QString doName = med->doNames()->at(i).toString();
        if (doMap.contains(doName))
        {
            continue;
        }
        auto scdo = basicElementFactory->createDO(doName, this);
        doMap[doName] = scdo;
        auto doConfig = basicElementFactory->createIOConfig(scdo);
        doConfig->setName(doName);
        scdo->setConfig(doConfig);
        ConfigShareClient::getIns().bindToRemoteConfig(QString("DOConfig_%1").arg(doName), doConfig);
    }
    for (int i = 0; i < med->cylNames()->count(); i++)
    {
        QString cylName = med->cylNames()->at(i).toString();
        if (cylMap.contains(cylName))
        {
            continue;
        }
        auto cyl = new SCCylinder(cylName, this);
        cyl->setMoveProtection(m_moveProtection);
        cylMap[cylName] = cyl;
        auto cylConfig = new CylinderConfig(cyl);
        cylConfig->setName(cylName);
        cyl->setConfig(cylConfig);
        ConfigShareClient::getIns().bindToRemoteConfig(QString("CylConfig_%1").arg(cylName), cylConfig);
    }
    for (int i = 0; i < med->vacuumNames()->count(); i++)
    {
        QString vacuumName = med->vacuumNames()->at(i).toString();
        if (vacuumMap.contains(vacuumName))
        {
            continue;
        }
        auto vacuum = new SCVacuum(vacuumName, this);
        vacuumMap[vacuumName] = vacuum;
        auto vacuumConfig = new VacuumConfig(vacuum);
        vacuumConfig->setName(vacuumName);
        vacuum->setConfig(vacuumConfig);
        ConfigShareClient::getIns().bindToRemoteConfig(QString("VacuumConfig_%1").arg(vacuumName), vacuumConfig);
    }
    createAxisModule(med);
}

void MotionManager::createAxisModule(MotionElementDefinition *med)
{
    for (int i = 0; i < med->axisModules()->count(); i++)
    {
        AxisModuleDefinition *axisModule = med->axisModules()->getConfig<AxisModuleDefinition>(i);
        MotionElement::Type moduleType = MotionElement::TypeEnumInfo().nameToEnum(axisModule->moduleType());
        QString moduleName = axisModule->moduleName();
        if (axisModuleMap.contains(moduleName))
        {
            continue;
        }
        switch (moduleType)
        {
            case MotionElement::SingleAxis:
            {
                auto sAxisConfig = new SingleAxisConfig();
                sAxisConfig->setModuleName(moduleName);
                auto sAxis = new SingleAxis(moduleName, sAxisConfig);
                axisModuleMap[moduleName] = sAxis;
                ConfigShareClient::getIns().bindToRemoteConfig(QString("SingleAxisConfig_%1").arg(moduleName), sAxisConfig);
                break;
            }
            case MotionElement::XYModule:
            {
                auto xyModuleConfig = new XYModuleConfig();
                xyModuleConfig->setModuleName(moduleName);
                auto xyModule = new XYModule(moduleName, xyModuleConfig);
                axisModuleMap[moduleName] = xyModule;
                ConfigShareClient::getIns().bindToRemoteConfig(QString("XYModuleConfig_%1").arg(moduleName), xyModuleConfig);
                break;
            }
            case MotionElement::XYZModule:
            {
                auto xyzModuleConfig = new XYZModuleConfig();
                xyzModuleConfig->setModuleName(moduleName);
                auto xyzModule = new XYZModule(moduleName, xyzModuleConfig);
                axisModuleMap[moduleName] = xyzModule;
                ConfigShareClient::getIns().bindToRemoteConfig(QString("XYZModuleConfig_%1").arg(moduleName), xyzModuleConfig);
                break;
            }
            default:
            {
                throw SilicolAbort(tr("Unknown axis module type: %1").arg(axisModule->moduleType()), EX_LOCATION);
            }
        }
    }
}

void MotionManager::setMotionElementLocation(MotionElementDefinition *med)
{
    bool isLocal = (med->address() == motionManagerConfig->localMotionManagerAddress());
    for (int i = 0; i < med->axisDefinitions()->count(); i++)
    {
        QString axisName = med->axisDefinitions()->getConfig<AxisDefinition>(i)->axisName();
        elementLocations[nominate(MotionElement::Axis, axisName)] = ElementLocation(isLocal, med->address());
    }
    for (int i = 0; i < med->diNames()->count(); i++)
    {
        QString diName = med->diNames()->at(i).toString();
        elementLocations[nominate(MotionElement::DI, diName)] = ElementLocation(isLocal, med->address());
    }
    for (int i = 0; i < med->doNames()->count(); i++)
    {
        QString doName = med->doNames()->at(i).toString();
        elementLocations[nominate(MotionElement::DO, doName)] = ElementLocation(isLocal, med->address());
    }
    for (int i = 0; i < med->cylNames()->count(); i++)
    {
        QString cylName = med->cylNames()->at(i).toString();
        elementLocations[nominate(MotionElement::Cylinder, cylName)] = ElementLocation(isLocal, med->address());
    }
    for (int i = 0; i < med->vacuumNames()->count(); i++)
    {
        QString vacuumName = med->vacuumNames()->at(i).toString();
        elementLocations[nominate(MotionElement::Vacuum, vacuumName)] = ElementLocation(isLocal, med->address());
    }
    for (int i = 0; i < med->axisModules()->count(); i++)
    {
        AxisModuleDefinition *axisModule = med->axisModules()->getConfig<AxisModuleDefinition>(i);
        MotionElement::Type moduleType = MotionElement::TypeEnumInfo().nameToEnum(axisModule->moduleType());
        elementLocations[nominate(moduleType, axisModule->moduleName())] = ElementLocation(isLocal, med->address());
    }
}

void MotionManager::deleteMotionNodeReplica()
{
    auto replicas = motionManagerReplicas.values();
    motionManagerReplicas.clear();
    while (!replicas.isEmpty())
    {
        delete replicas.takeFirst();
    }
    while (!nodes.isEmpty())
    {
        delete nodes.takeFirst();
    }
}

void MotionManager::handleError(const QString &uuid, const QString &errMsg, bool sendbackResult)
{
    if (sendbackResult)
    {
        emit executeDone(uuid, errMsg, QVariant());
    }
    else
    {
        qCritical(motionCate()) << errMsg;
    }
}
