#ifndef DUMMYDISPATCHER_H
#define DUMMYDISPATCHER_H

#include "VisionManager/visionmanager.h"
#include "mastermotionmanager.h"
#include "systemCore/dispatcher.h"
#include <QObject>

class DummyDispatcher : public Dispatcher
{
    Q_OBJECT

public:
    explicit DummyDispatcher(MasterMotionManager *masterMotionManager, QObject *parent = nullptr);

    void setContextProperty(QQmlApplicationEngine &engine);

    // Dispatcher interface
protected:
    void handleInit() override;
    void handleStart() override;
    void handlePause() override;
    void handleContinue() override;
    void handleAbort() override;
    void handleReset() override;
    void preHome() override;
    virtual void postHome() override;

private:
    MasterMotionManager *masterMotionManager;
};

#endif    // DUMMYDISPATCHER_H
