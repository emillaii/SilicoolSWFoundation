#ifndef PICKARM_H
#define PICKARM_H

#include "AxisModule/xyzmodule.h"
#include "CompositeElement/scvacuum.h"
#include "Tray/duttraymap.h"
#include "Workers/TrayLoader/trayloader.h"
#include "loghelper.h"
#include "namedefinition.h"
#include "systemCore/worker.h"
#include "uphhelper.h"
#include <QObject>

SILICOOL_DECLARE_LOGGING_CATEGORY(paTimeCost, )

class PickArm : public Worker
{
    Q_OBJECT
public:
    explicit PickArm(DutTrayMap *trayMap, TrayLoader *tl, UPHHelper *uphHelper, QObject *parent = nullptr);

    // 自动运行逻辑
public slots:
    void waitDut();
    void moveFromStandbyToPickPos();
    void pickDut();
    void placeDut();
    void moveToStandbyPos();

    // Worker interface
protected:
    virtual void initEventHandle() override;
    virtual void startEventHandle(const WorkerEvent &event) override;
    virtual void resetEventHandle() override;
    virtual void homeWorker(bool moveToReadyPosAfterHome) override;

private:
    XYZModule *paXyz;
    SCVacuum *paVacuum;
    SingleAxis *paZ;

    DutTrayMap *trayMap;
    TrayLoader *tl;
    UPHHelper *uphHelper;

    QPoint currentDutIndex;
};

#endif    // PICKARM_H
