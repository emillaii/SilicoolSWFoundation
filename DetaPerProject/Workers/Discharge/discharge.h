#ifndef DISCHARGE_H
#define DISCHARGE_H

#include "AxisModule/singleaxis.h"
#include "AxisModule/xymodule.h"
#include "CompositeElement/sccylinder.h"
#include "CompositeElement/scvacuum.h"

#include "VisionLocation/visionlocation.h"
#include "VisionManager/visionmanager.h"

#include "namedefinition.h"
#include "systemCore/worker.h"

#include "trayMap/traymap.h"
#include "trayMap/trayconfig.h"
#include "dischargeconfig.h"
#include "utilities_global.h"
#include <QObject>
#include "Tray/DischargeMap.h"

struct DischargeMapInitIndex
{
    int RowIndex = 0;
    int ColumnIndex = 0;
    DischargeMapData *pDischargeMapData = nullptr;
};

struct DischargeMapOffset
{
    bool IsMovePaX = false;
    double OffsetX = 0;
    double OffsetY = 0;
    DischargeMapData *pDischargeMapData = nullptr;
};

class Discharge : public Worker
{

Q_OBJECT

public:
    Discharge(TrayConfig *nDischargeTrayConfig,
              DischargeMap *nDischargeMap,
              DischargeConfig *nDischargeConfig,
              QObject *parent = nullptr);
    //
public slots:
    void StartAutoRun();
    void DischargeFuntion();
    void GetTray(int nIndex);
    void OutTray(int nIndex);
    void ElevatorMove(bool IsMoveUp);
    void GetTrayFomElevatorFloorNum(int nFloorNum);
    void ClampTray();
    void ReleaseTay();

    //
public slots:
    void MoveToGetTrayPos();
    void MoveToOutTrayPos();

    QPointF CalculateOutTrayOffset(QPointF nPrOffset, double nPrTheta, QPointF TrayRowColumnOffset);
    QPointF CalculateOutTrayOffset(double nPrTheta, QPointF TrayRowColumnOffset);
public:
    void ResetGotDut();
    bool GotDut();
    QPoint LastDutIndex() const;
    const PrOffset &LastDutPrOffset() ;
    bool IsNeedPAX_Move();
    DischargeMapOffset TrayMoveToPlaceMaterialPos(double CenterOffset);

private:
    //
    SingleAxis *pTrackX;//盤子在軌道上移動
    SingleAxis *pTrackZ;//轨道和夹子整体升降

    //DI
    SCDI *pTraySNR;//检测客户料盒有无Tray盘

    //DO
    SCDO *pTrackStop_Z;//电机刹车（出料轨道上下电机）
    SCDO *pTrayGripper;//夹料盘夹子


    QPoint currentDutIndex;

public:
    TrayConfig *pDischargeTrayConfig;
    DischargeMap *pDischargeMap;
    DischargeConfig *pDischargeConfig;
    ConfigFile *pDischargeConfigFile;

public:
    bool GetDischargeState();

    // Worker interface
    DischargeMapInitIndex GetPickArmMapInitIndex();
    DischargeMapOffset GetDischargeOffset();
    void MoveToPlaceMaterialTrackZ();
protected:
    void initEventHandle();
    void startEventHandle(const WorkerEvent &event);
    void stopEventHandle();
    void allWorkerStopped();
    void purgeEventHandle();
    void resetEventHandle();
    void homeWorker(bool moveToReadyPosAfterHome);


    bool m_gotDut = false;
    PrOffset m_lastDutOffset;
    QPoint m_lastDutIndex;
    int m_nContinuallyPrFailed = 0;

    bool m_IsReadyToPlaceMaterial = false;

};

#endif // DISCHARGE_H
