#ifndef MASTERMOTIONMANAGER_H
#define MASTERMOTIONMANAGER_H

#include "MotionManager/motionconfigmanager.h"
#include "MotionManager/motionelementuilayout.h"
#include "MotionManager/motionmanager.h"
#include "MotionManager/motionstatepublisher.h"
#include "TaskEngine/objectlivedthreadinstructionexecutor.h"
#include "XYZRDebugger/xyzrdebuggermanager.h"
#include "configManager/configobjectarray.h"
#include "motionelementdefinition.h"
#include "rep_motionManager_replica.h"
#include "scmotion_global.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QRemoteObjectNode>

class SCMOTIONSHARED_EXPORT MasterMotionManager : public ObjectLivedThreadInstructionExecutor
{
    Q_OBJECT

    Q_PROPERTY(double globalVelocityRatio READ globalVelocityRatio WRITE setGlobalVelocityRatio NOTIFY globalVelocityRatioChanged)
    Q_PROPERTY(bool moveProtectionsEnable READ moveProtectionsEnable WRITE setMoveProtectionsEnable NOTIFY moveProtectionsEnableChanged)

public:
    explicit MasterMotionManager(BasicElementFactory *basicElementFactory, QString dutRelatedConfigFileDir, QObject *parent = nullptr);

    ~MasterMotionManager();

    void setContextProperty(QQmlApplicationEngine &engine);

    MotionConfigManager *motionConfigManager() const
    {
        return m_motionConfigManager;
    }

    Q_INVOKABLE int motionElementUIPageCount() const
    {
        return meUILayouts->count();
    }

    Q_INVOKABLE QVariant getMotionElementUIPageInfo(int index)
    {
        QJsonValue pageInfo;
        meUILayouts->getConfig<MotionElementUILayout>(index)->write(pageInfo);
        return pageInfo;
    }

    Q_INVOKABLE void setAxisVelocityRatio(QString axisName, double ratio);

    double globalVelocityRatio() const
    {
        return m_globalVelocityRatio;
    }

    bool moveProtectionsEnable() const
    {
        return m_moveProtectionsEnable;
    }

    void pause();
    void unPause();
    void handleAbort();
    void handleReset();

public slots:
    void initMotionManagers();
    void updateMoveProtections();
    void setMoveProtectionsEnable(bool enable);
    void setMotionStateReporters(bool enable);
    void onUpdatePosReq(QString moduleName, MotionElement::Type moduleType, QString posName);
    void onMoveToReq(QString moduleName, MotionElement::Type moduleType, QString posName);
    void onMeasureHeightReq(QString axisName, QString softLandingPosName, double vel, double force, int holdTime);
    void subscribeMotionState(QQuickItem *subscriber, QString functionName);
    void setGlobalVelocityRatio(double globalVelocityRatio);

signals:
    void globalVelocityRatioChanged(double globalVelocityRatio);

    void moveProtectionsEnableChanged(bool moveProtectionsEnable);

private slots:
    void initMotionManagersImpl();
    void pauseImpl();
    void abortImpl();
    void resetImpl();
    void unPauseImpl();

private:
    void checkMotionElementUILayoutConfig();
    void subscribePosReq();
    void subscribeModulePosReq(AxisModuleConfig *axisModuleConfig);
    void subscribeMeasureHeightReq(ConfigObjectArray *axisConfigs);

private:
    const QString medsConfigFileName = "./config/platformConfig/motionElementDefinition.json";
    ConfigObjectArray *m_meds;
    ConfigFile *medsConfigFile;

    const QString meUILayoutsConfigFileName = "./config/platformConfig/motionElementUILayout.json";
    ConfigObjectArray *meUILayouts;
    ConfigFile *meUILayoutsConfigFile;

    MotionConfigManager *m_motionConfigManager;
    MotionStatePublisher *m_motionStatePublisher;
    QList<QRemoteObjectNode *> nodes;
    QList<MotionManagerReplica *> motionManagerReplicas;
    double m_globalVelocityRatio;
    bool m_moveProtectionsEnable = true;
};

#endif    // MASTERMOTIONMANAGER_H
