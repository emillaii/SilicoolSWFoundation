#ifndef FEEDING_H
#define FEEDING_H

#include "AxisModule/singleaxis.h"
#include "AxisModule/xymodule.h"
#include "CompositeElement/sccylinder.h"
#include "Tray/FeedingMap.h"
#include "VisionLocation/visionlocation.h"
#include "VisionManager/visionmanager.h"
#include "feedingconfig.h"
#include "namedefinition.h"
#include "systemCore/worker.h"
#include "trayMap/trayconfig.h"
#include "uphhelper.h"
#include "utilities_global.h"
#include <QObject>

struct FeedingState
{
    bool IsCutSafe = false;
    bool IsReadyToPickMaterial = false;
};

class Feeding : public Worker
{
    Q_OBJECT
public:
    Feeding(TrayConfig *trayConfig, FeedingMap *feedingMap, FeedingConfig *pFeedingConfig, QObject *parent = nullptr);
    ~Feeding() override;
    FeedingConfig *getFeedingConfig() const
    {
        return pFeedingConfig;
    }

protected:
    // Worker interface
    void initEventHandle() override;
    void startEventHandle(const WorkerEvent &event) override;
    void stopEventHandle() override;
    void allWorkerStopped() override;
    void purgeEventHandle() override;
    void resetEventHandle() override;
    void homeWorker(bool moveToReadyPosAfterHome) override;

    //
    bool WaitForDiChange(SCDI *pDi);
    void SetSpeedAndAccForThreeAxis(double nSpeed, double nAcc);
    bool CheckCarrierBoardIsOnDetaperPlatform();
public slots:
    void StartAutoRun();
    void FeedingFuntion();
    void FeedCarrierBoard(int nIndex);

    FeedingState GetFeedingState();
    void AskPickArmMoveToCutPos();

    void CutCarrierBoard();
    void OutCarrierBoard();

    void TakeMaterialAwayWithOffset(double nOffset, bool IsFirstDetaper);

    void SearchForMag();
    //
public slots:
    void MoveToFeedCarrierBoardPos();
    void MoveToTakeMaterialPos();
    void BeginStrippingMaterial();
    void MoveToChoseClunm(int Column, bool IsFirstMag);
    void ChangeMag();

    // void PaFinish();
    // void CarrierBoardFinish();
    void FeedMag();
    void FeedCarrierBoardTest();
    void WaitAxisDone(SingleAxis *nAxis);

private:
    void MoveToPosWithRow(int rowIndex, int columnIndex);
    void MoveToPosWithRow(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex);
    QPointF CalculatePosFromColumn(int nColumn);

    //    void setGear(int core, int masterIndex, int slaverIndex, int ratio1, int ratio2)
    //    {
    //        checkResult1(GTN_PrfGear(core, slaverIndex));
    //        checkResult1(GTN_SetGearMaster(core, slaverIndex, masterIndex));
    //        checkResult1(GTN_SetGearRatio(core, slaverIndex, 1, 1, 0));
    //        checkResult1(GTN_GearStart(core, 1 << (slaverIndex - 1)));
    //        setMoveMode(GTAxis::Pos, false);
    //    }

private:
    SingleAxis *pElevatorX;
    SingleAxis *pElevatorZ;
    SingleAxis *pDetaperZ;
    SingleAxis *pContactRollerL;
    SingleAxis *pContactRollerR;
    SingleAxis *pPickArmC_Z;

    SCCylinder *pMagGripper;            //
    SCCylinder *pMagKicker;             //
    SCCylinder *pDtStopper;             //
    SCCylinder *pDtGripper;             //剥单夹子
    SCCylinder *pDtKicker;              //剥单退载板回弹夹
    SCCylinder *pCollectSNR;            //
    SCCylinder *pTrayClamp;             //压载板
    SCCylinder *pPreventOverTurnSNR;    //

    SCDI *pMagKickerIn;       //
    SCDI *pMagKickerOut;      //
    SCDI *pMagFullSNR;        //
    SCDI *pCobCarrierSNR;     //判断载板是否有
    SCDI *pDtStopperIn;       //
    SCDI *pDtStopperOut;      //
    SCDI *pDtGripperIn;       //
    SCDI *pDtGripperOut;      //
    SCDI *pDtKickerIn;        //
    SCDI *pDtKickerOut;       //
    SCDI *pVacPlatformIn;     //
    SCDI *pVacPlatformOut;    //
    SCDI *pIsExistMag;        //判断是否存在弹夹

    SCDO *pSensorVac = nullptr;

public:
    TrayConfig *pFeedTrayConfig;
    FeedingMap *pFeedingMap;
    FeedingConfig *pFeedingConfig;
    ConfigFile *pFeedingConfigFile;

private:
    bool m_gotDut = false;
    PrOffset m_lastDutOffset;
    QPoint m_lastDutIndex;
    int m_nContinuallyPrFailed = 0;

    FeedingState m_FeedingState;
    XYModule *demoModule;

    bool WaitForInputTrue(SCDI *nDI, long nTimeOut = 0);
    bool WaitForInputFalse(SCDI *nDI, long nTimeOut = 0);

    void DisableAllDi();
};

// class ErrorHandlerCls : public SilicolError
//{
// public:
//    ErrorHandlerCls(const QString &nModuleName, const QString &nErrorMsg, QVariantList nOption)
//        :SilicolError (nModuleName,nErrorMsg)
//    {
//        for (int i = 0; i<nOption.size();i++)
//        {
//            appendErrorHandlingOption(nOption.at(i).toString());
//        }
//    }
// protected:
//    virtual void implemented() override {}
//};

#endif    // FEEDING_H
