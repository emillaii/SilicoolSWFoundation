#ifndef MOTIONMANAGER_H
#define MOTIONMANAGER_H

#include "AxisModule/singleaxis.h"
#include "AxisModule/xymodule.h"
#include "AxisModule/xyzmodule.h"
#include "BasicElement/basicelementfactory.h"
#include "BasicElement/dummyaxis.h"
#include "BasicElement/globalstate.h"
#include "CompositeElement/scvacuum.h"
#include "TaskEngine/instruction.h"
#include "TowerLightBuzzer/scbuzzer.h"
#include "TowerLightBuzzer/sctowerlight.h"
#include "configManager/configobjectarray.h"
#include "configManager/configshareclient.h"
#include "distatemonitor.h"
#include "motionelementdefinition.h"
#include "motionmanagerconfig.h"
#include "motionstategetter.h"
#include "motionstatereporter.h"
#include "rep_motionManager_replica.h"
#include "rep_motionManager_source.h"
#include "scmotion_global.h"
#include <QObject>
#include <QRemoteObjectNode>
#include <QtConcurrent>

struct ElementLocation
{
public:
    ElementLocation() {}
    ElementLocation(bool isLocal, QString address) : isLocal(isLocal), address(address) {}

    bool isLocal;
    QString address;
};

struct CacheState
{
public:
    qint64 lastUpdateTimeStamp = 0;
    double state = 0;
};

class SCMOTIONSHARED_EXPORT MotionManager;

class InstructionExecutor : public QRunnable
{
public:
    InstructionExecutor(MotionManager *motionManager,
                        QString uuid,
                        MotionElement::Type elementType,
                        QString elementName,
                        QString cmd,
                        QVariantList args,
                        bool sendbackResult);

    // QRunnable interface
public:
    virtual void run() override;

private:
    MotionManager *motionManager;
    QString uuid;
    MotionElement::Type elementType;
    QString elementName;
    QString cmd;
    QVariantList args;
    bool sendbackResult;
};

class SCMOTIONSHARED_EXPORT MotionManager : public MotionManagerSimpleSource
{
    Q_OBJECT

private:
    explicit MotionManager(QObject *parent = nullptr);

public:
    ~MotionManager() override;

    static MotionManager &getIns()
    {
        static MotionManager instance;
        return instance;
    }

    void dispose();

    bool isInit() const
    {
        return m_isInit;
    }

    void setBasicElementFactory(BasicElementFactory *basicElementFactory);

    void attachBuzzerTowerLight(SCBuzzer *buzzer, SCTowerLight *towerLight);

    ///
    /// \brief waitInstructionExecutionDone
    /// You can only call 'waitInstructionExecutionDone' or 'getInstructionExecutionResult' once
    /// for a task, for the reason that MotionManager will remove the task once you
    /// call 'waitInstructionExecutionDone' or 'getInstructionExecutionResult'.
    /// \param uuid task id returned by 'executeInstruction'
    ///
    void waitInstructionExecutionDone(QString uuid)
    {
        getInstructionExecutionResult(uuid);
    }

    ///
    /// \brief getInstructionExecutionResult
    /// You can only call 'waitInstructionExecutionDone' or 'getInstructionExecutionResult' once
    /// for a task, for the reason that MotionManager will remove the task once you
    /// call 'waitInstructionExecutionDone' or 'getInstructionExecutionResult'.
    /// \param uuid task id returned by 'executeInstruction'
    /// \return
    ///
    QVariant getInstructionExecutionResult(QString uuid);

    template <typename T>
    T getInstructionExecutionResult(QString uuid)
    {
        return getInstructionExecutionResult(uuid).value<T>();
    }

    ///
    /// \brief runInstruction   executeInstruction and return the result
    /// \param elementType MotionElement::Type
    ///
    Q_INVOKABLE QVariant runInstruction(int elementType, QString elementName, QString cmd, QVariantList args = QVariantList());

    ///
    /// \brief executeInstruction
    /// Create an task with an uuid,
    /// publish this task then return the uuid.
    /// You can wait this task done or get the task result by the uuid next.
    /// \ref waitInstructionExecutionDone
    /// \ref getInstructionExecutionResult
    ///
    Q_INVOKABLE QString
    executeInstruction(int elementType, QString elementName, QString cmd, QVariantList args = QVariantList(), bool waitDone = false);

    void moveMultiAxes(QList<QString> axisNames, QList<double> targetPos, bool isAbsMove = true);

    MoveProtection *moveProtection() const
    {
        return m_moveProtection;
    }

signals:
    void executeDone(QString uuid, QString errMsg, QVariant result);
    void inited();
    void beforeDisposing();

    // MotionManagerSource interface
public slots:
    virtual bool initMotionManager(QByteArray motionElementDefinition) override;

    virtual void setIsPaused(bool isPaused) override;
    virtual bool waitPaused() override;
    virtual void handleAbort() override;
    virtual void handleReset() override;

    virtual void setMotionStateReporter(bool enable) override;

    virtual void setMoveProtectionEnable(bool enable) override;

    virtual void updateMoveProtection() override;

    virtual double getAxisCurrentPos(QString axisName) override;

    virtual double getAxisFeedbackPos(QString axisName, int cacheInvalidationTime = 2) override;

    virtual double getAxisTargetPos(QString axisName, int cacheInvalidationTime = 2) override;

    virtual int getCylinderCurrentState(QString cylName, int cacheInvalidationTime = 2) override;

    virtual bool getDiCurrentState(QString diName, int cacheInvalidationTime = 2) override;

    virtual void
    onExecuteInstruction(QString uuid, int elementType, QString elementName, QString cmd, QVariantList args, bool sendbackResult = false) override;

    void stopAllAxis();
    void clearAllAxisAlarm();

private slots:
    void onInstructionExecuteDone(QString uuid, QString errMsg, QVariant result);

private:
    void createMotionElement(MotionElementDefinition *med);

    void createAxisModule(MotionElementDefinition *med);

    void setMotionElementLocation(MotionElementDefinition *med);

    void deleteMotionNodeReplica();

    void handleError(const QString &uuid, const QString &errMsg, bool sendbackResult);

private:
    friend class InstructionExecutor;

    BasicElementFactory *basicElementFactory = nullptr;
    ControlCard *controlCard = nullptr;
    QMap<QString, ElementLocation> elementLocations;
    QMap<QString, MotionManagerReplica *> motionManagerReplicas;
    QList<QRemoteObjectNode *> nodes;
    QRemoteObjectHost host;

    ConfigObjectArray *motionElementDefinitions;
    ConfigFile *motionManagerConfigFile;
    MotionManagerConfig *motionManagerConfig;

    SCBuzzer *buzzer = nullptr;
    SCTowerLight *towerLight = nullptr;

    QMap<QString, SCAxis *> axisMap;
    QMap<QString, SCDI *> diMap;
    QMap<QString, SCDO *> doMap;
    QMap<QString, SCCylinder *> cylMap;
    QMap<QString, SCVacuum *> vacuumMap;
    QMap<QString, AxisModule *> axisModuleMap;

    QThreadPool threadPool;
    QMap<QString, InstructionExecuteResult> instructionExecuteResults;

    MotionStateReporter motionStateReporter;

    ConfigObjectArray *collisionGroupsConfig;
    MoveProtection *m_moveProtection;

    bool startMotionStateReporter = false;
    bool m_isInit = false;

    QMap<QString, CacheState *> remoteAxisFeedbackPosCache;
    QMap<QString, CacheState *> remoteAxisTargetPosCache;
    QMap<QString, CacheState *> remoteCylStateCache;
    QMap<QString, CacheState *> remoteDiStateCache;
};

#endif    // MOTIONMANAGER_H
