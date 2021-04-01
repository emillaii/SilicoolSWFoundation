#ifndef PICKARM_H
#define PICKARM_H

#include "AxisModule/xymodule.h"
#include "CompositeElement/scvacuum.h"
#include "Tray/PickArmMap.h"
#include "Vision/detapervisionfactory.h"
#include "Workers/Discharge/discharge.h"
#include "Workers/Feeding/feeding.h"
#include "loghelper.h"
#include "namedefinition.h"
#include "pickarmconfig.h"
#include "systemCore/worker.h"
#include "uphhelper.h"
#include <QObject>
//#include "Workers/ServerAndClient/tcpserver.h"
#include "Workers/ServerAndClient/tcpclient.h"
#include <math.h>

SILICOOL_DECLARE_LOGGING_CATEGORY(paTimeCost, )

class ErrorHandlerCls : public SilicolError
{
public:
    ErrorHandlerCls(const QString &nModuleName, const QString &nErrorMsg, QVariantList nOption) : SilicolError(nModuleName, nErrorMsg)
    {
        for (int i = 0; i < nOption.size(); i++)
        {
            appendErrorHandlingOption(nOption.at(i).toString());
        }
    }

protected:
    virtual void implemented() override {}
};

struct PickArmMapInitIndex
{
    int RowIndex = 0;
    int ColumnIndex = 0;
    PickArmMapData *pPickArmMapData = nullptr;
    bool IsIndexEnd = false;
};

class PickArm : public Worker
{
    Q_OBJECT
public:
    PickArm(Feeding *nFeeding,
            Discharge *nDischarge,
            PickArmMap *nPickArmMap,
            TrayConfig *nPickArmTrayConfig,
            PickArmConfig *nPickArmConfig,
            UPHHelper *uphHelper,
            QObject *parent = nullptr);
    ~PickArm() override;
    PickArmConfig *getPaConfig() const
    {
        return pPAConfig;
    }
    // 自动运行逻辑
public slots:
    void StartAutoRun();
    void WaitTrayOk(bool IsLeftNozzle);
    void PickMterial(bool IsLeftNozzle);
    void PlaceMaterial(bool IsLeftNozzle);

    // Manual
public slots:
    void MoveToStandbyPos(bool IsManual);
    void MoveToPickMaterialPos(bool IsLeftNozzle, QPointF nPickMaterialPos, double nDelta);
    void MoveToPlaceMaterialPos(bool IsLeftNozzle, QPointF nPlaceMaterialPos);
    void OpenVacuum(bool IsSuctionA);
    void CloseVacuum(bool IsSuctionA);
    void MoveToTakeMaterialPicturePos();
    void PickMaterialPosUseCamera(bool IsLeftNozzle, QPointF &nPickMaterialPos, double &nTheta);
    void PlaceMaterialPosUseCamera(bool IsLeftNozzle, QPointF &nPlaceMaterialPos);

    void MoveToTakeOutTrayPicturePos();
    void MoveToCutCarrierBoard_XY();
    void GetVisionResult(POINT &nPoint, double &nAngle);
    void TakePicture(bool IsLeft);
    void AskFeedCarrierBoard();
    void CalibrateCameraToNozzleOffset(bool IsLeftNozzle);
    void PickGlassWithOffset(bool IsLeftNozzle);
    void PlaceGlassWithOffset(bool IsLeftNozzle);

    void AskFeedCarrierBoardTest();
    // Worker interface
    void AskFeedingCutCarrierBoard();
    void AskFeedingOutCarrierBoard();
    void AskFeedingTakeMaterialAwayWithOffset();
    void AskFeedingTakeMaterialAwayWithOffsetFirst();
    void WaitAxisStop();
    void VisionTest();

    void AutoRunTest();

    void StopTest();
    void Test();
    void XY_MoveToTakePictureForGlassPos();
    void XY_MoveToGetGlassPos(bool IsLeftNozzle);
    void testAdd();
    void PickAndPlace();
    void PickMaterialTest(bool IsLeftNozzle);
    void PlaceMaterialTest(bool IsLeftNozzle);
    void sendData();

    QPointF PrPosA(bool nIsMaterial);
    QPointF PrPosB(bool nIsMaterial);
    QPointF CalculateSpacePosAToPosB(QPointF nPosA, QPointF nPosB);

    void testGetSpace1(bool nIsMaterial);
    void testGetSpace2(bool nIsMaterial, QString nCalculateType);

    void keyBoardMove(QString nAxisName, int nDirection);
    void keyBoardStop(QString nAxisName);

protected:
    virtual void initEventHandle() override;
    virtual void startEventHandle(const WorkerEvent &event) override;
    virtual void stopEventHandle() override;
    virtual void allWorkerStopped() override;
    virtual void purgeEventHandle() override;
    virtual void resetEventHandle() override;
    virtual void homeWorker(bool moveToReadyPosAfterHome) override;

    bool WaitOKFlag(bool IsFeeding);
    void UpdateMapAfterDetaper(int nRowIndex);

private:
    enum PrType
    {
        CalibrationPr,
        MaterialPr,
        TrayPr
    };
    struct PrResult
    {
        PrOffset mPrOffset;
        bool IsOk = false;
    };

private:
    PrResult GetPrResult(PrType nPrType);
    void PickCalibrationGlass(bool isLeftPA);
    void PlaceCalibrationGlass(bool isLeftPA);

    void MoveAllAxisUp();

    void MoveToPosWithRowCol(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex, bool IsMoveXY, bool IsNeedAddUnitDeltaFlag);
    void MoveToPosWithRowCol(int rowIndex, int columnIndex, bool IsMoveXY, bool IsNeedAddUnitDeltaFlag);
    PickArmMapInitIndex GetPickArmMapInitIndex(bool IsInit);
    void PAr_GoToStandby(bool IsLeftNozzle);
    void PAr_GoToStandbyOK(bool IsLeftNozzle);

private slots:
    void ReadData123(int clientID, QString IP, int Port, QByteArray data);

private:
    TcpClient *mTcpClient;
    // TcpServer nServer;
    // TcpClient nClient;
    //机头
    SingleAxis *pPACZ = nullptr;    //切料Z

    XYModule *pPAXY = nullptr;
    SingleAxis *pPAZL = nullptr;
    SingleAxis *pPAZR = nullptr;
    SingleAxis *pPAθL = nullptr;
    SingleAxis *pPAθR = nullptr;

    // DI
    SCDI *pPaVacSNR_L = nullptr;
    SCDI *pPaVacSNR_R = nullptr;

    // DO
    SCVacuum *pPickVac_L = nullptr;
    SCVacuum *pPickVac_R = nullptr;
    SCDO *pSensorVac = nullptr;

    UPHHelper *uphHelper = nullptr;

    Discharge *pDischarge = nullptr;
    Feeding *pFeeding = nullptr;

    TrayConfig *pPickArmTrayConfig = nullptr;
    PickArmMap *pPickArmMap = nullptr;
    PickArmConfig *pPAConfig = nullptr;
    ConfigFile *pPickArmConfigFile = nullptr;

    VisionLocation *pPACalibOffset_Location = nullptr;
    VisionLocation *pGetMaterialLocation = nullptr;
    VisionLocation *pPlaceMaterialLocation = nullptr;

    QPoint currentDutIndex;

    bool IsStop = false;
    bool IsFirstDetaper = true;
    int OldUnitRowIndex = 0;

    int NowMapRowIndex = 0;
    int NowMapColumnIndex = 0;

    QPointF LeftNozzlePickMaterial;
    QPointF RightNozzlePickMaterial;
    double LeftPickMaterialTheta = 0;
    double RightPickMaterialTheta = 0;

    QPointF LeftNozzlePlaceMaterial;
    QPointF RightNozzlePlaceMaterial;

    int RunStep = 0;
    bool IsFeedingTakeMaterialAway = false;

    DischargeMapOffset m_DischargeMapOffset;
    double PlaceOffsetFromPickX = 0;
    double PlaceOffsetFromPickY = 0;

    int PickNum = 0;             //取料计数器
    QPointF nSpaceTemp;          //间距缓存
    bool PosAPrFirst = false;    //一定要先对点A进行PR

    void Add(QPointF &nQpointF, double &nTheta);
};

namespace ErrorHandlingOption
{
    const QString FinishHandle_ = QObject::tr("FinishHandle_");
    const QString UseCurrent_ = QObject::tr("UseCurrent_");
}    // namespace ErrorHandlingOption
class NuzzleHasMaterial : public SilicolError
{
public:
    NuzzleHasMaterial()
        : SilicolError(
            WorkerName::PickArm,
            QObject::tr("Nuzzle has material please check and handle it than continual. \r\nOption explantion: \r\n"
                        "FinishHandle: make sure finish handle than continual. \r\nUseCurrent: Use current material. \r\nAbort: System abort."))
    {
        appendErrorHandlingOption(ErrorHandlingOption::FinishHandle_);
        appendErrorHandlingOption(ErrorHandlingOption::UseCurrent_);
        appendErrorHandlingOption(Abort_);
    }

protected:
    virtual void implemented() override {}
};
#endif    // PICKARM_H
