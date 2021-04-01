#include "pickarm.h"

SILICOOL_CREATE_LOGGING_CATEGORY(paTimeCost, "paTimeCost")

PickArm::PickArm(Feeding *nFeeding,
                 Discharge *nDischarge,
                 PickArmMap *nPickArmMap,
                 TrayConfig *nPickArmTrayConfig,
                 PickArmConfig *nPickArmConfig,
                 UPHHelper *uphHelper,
                 QObject *parent)
    : Worker(WorkerName::PickArm, parent),
      uphHelper(uphHelper),
      pDischarge(nDischarge),
      pFeeding(nFeeding),
      pPickArmTrayConfig(nPickArmTrayConfig),
      pPickArmMap(nPickArmMap),
      pPAConfig(nPickArmConfig)
{
    pPickArmConfigFile = new ConfigFile("PickArmConfig", pPAConfig, QString("%1/PickArmConfig.json").arg(Worker::getWorkerConfigDir()));
    pPickArmConfigFile->populate(true);

    mTcpClient = new TcpClient();
    QString str("127.0.0.1");
    mTcpClient->ClientInit(str, 8080);
    connect(mTcpClient, SIGNAL(ClientReadData(int, QString, int, QByteArray)), this,
            SLOT(ReadData123(int, QString, int, QByteArray)));    //挂接读取数据信号
}

PickArm::~PickArm() {}

void PickArm::initEventHandle()
{
    //
    pPACZ = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PACZ);

    pPAXY = MotionElementContainer::getIns()->getItem<XYModule>(XYModuleName::PAXY);

    pPAZL = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PAZL);
    pPAZR = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PAZR);
    pPAθL = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PAθL);
    pPAθR = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PAθR);

    // DI
    pPaVacSNR_L = MotionElementContainer::getIns()->getItem<SCDI>(DiName::PaVacSNR_L);
    pPaVacSNR_R = MotionElementContainer::getIns()->getItem<SCDI>(DiName::PaVacSNR_R);

    // DO
    pPickVac_L = MotionElementContainer::getIns()->getItem<SCVacuum>(DoName::PickVac_L);
    pPickVac_R = MotionElementContainer::getIns()->getItem<SCVacuum>(DoName::PickVac_R);
    pSensorVac = MotionElementContainer::getIns()->getItem<SCDO>(DoName::SensorVac);

    // vision
    pPACalibOffset_Location = VisionManager::getIns().getVisionLocation(VLName::VisionLocationForScale);
    pGetMaterialLocation = VisionManager::getIns().getVisionLocation(VLName::GetMaterialLocation);
    pPlaceMaterialLocation = VisionManager::getIns().getVisionLocation(VLName::PlaceMaterialLocation);
}

void PickArm::startEventHandle(const WorkerEvent &event)
{
    Q_UNUSED(event)

    IsStop = false;

    pushEvent(WorkerEvent("moveToStandbyPos"));
    pushEvent(WorkerEvent("waitDut"));
}

void PickArm::stopEventHandle()
{
    // Do something...
    IsStop = true;
}

void PickArm::allWorkerStopped()
{
    IsStop = true;
    // Do something...
}

void PickArm::purgeEventHandle()
{
    // Do something...//purge清除
    // purgeEventHandled();
}

void PickArm::resetEventHandle()
{
    IsStop = false;
    // Do something...
}

void PickArm::homeWorker(bool moveToReadyPosAfterHome)
{
    pPickVac_L->set(false);
    pPickVac_R->set(false);
    //其他轴在PrHome中回零了
    pPAXY->xAxis()->clearAlarm();
    pPAXY->yAxis()->clearAlarm();

    pPAXY->xAxis()->home();
    pPAXY->yAxis()->home();

    if (moveToReadyPosAfterHome)
    {
        pPAXY->moveTo(XYZPosName::StandbyPos, true);
    }
    OldUnitRowIndex = GetPickArmMapInitIndex(false).RowIndex / pPickArmTrayConfig->rowsInsideUnit();
}

// Pick Material region

void PickArm::testAdd()
{

    QString option
        = UIOperation::getIns()->getUIResponse("运行选择", "如有载板，选择继续吸放!\n如没有载板，选择上载板并继续!", MsgBoxIcon::Icon::Question,
                                               { QString("继续吸放"), QString("上载板并继续"), QString("取消运行") });
    if (option == "继续吸放")
    {
        qDebug() << "继续吸放";
        // pushEvent(WorkerEvent("PickAndPlace"));
        return;
    }
    else if (option == "上载板并继续")
    {
        qDebug() << "上载板并继续";
        //如果有载板在上边就直接去吸放不要重新回零了，回零太麻烦。
        return;
    }

    QPointF aaa;
    double bbb = 0;
    Add(aaa, bbb);

    // nServer.ServerInit(QHostAddress::Any, 5566);
}

void PickArm::sendData()
{
    // mTcpClient->SendData("qwe那你");
}

void PickArm::Add(QPointF &nQpointF, double &nTheta)
{
    QPointF A;

    nQpointF = QPointF(3, 4);
    nQpointF += A;
    nTheta = 4;
}

void PickArm::AutoRunTest()
{
    checkIsInit();
    try
    {
        //        qDebug()<<"AskFeedCarrierBoardTest";
        //        AskFeedCarrierBoardTest();
        //        qDebug()<<"MoveToCutCarrierBoard_XY";
        //        MoveToCutCarrierBoard_XY();
        //        qDebug()<<"AskFeedingCutCarrierBoard";
        //        AskFeedingCutCarrierBoard();
        //        qDebug()<<"AskFeedingTakeMaterialAwayWithOffset";
        //        AskFeedingTakeMaterialAwayWithOffset();

        PickMaterialPosUseCamera(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
        PickMaterialPosUseCamera(false, RightNozzlePickMaterial, RightPickMaterialTheta);
        MoveToPickMaterialPos(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
        MoveToPickMaterialPos(false, RightNozzlePickMaterial, RightPickMaterialTheta);

        PlaceMaterialPosUseCamera(true, LeftNozzlePlaceMaterial);
        PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);

        MoveToPlaceMaterialPos(true, LeftNozzlePlaceMaterial);
        MoveToPlaceMaterialPos(false, RightNozzlePlaceMaterial);
    }
    catch (SilicoolException &ex)
    {
        throw ex;
    }
}

void PickArm::StartAutoRun()
{
    try
    {
        checkIsInit();
        QString option = UIOperation::getIns()->getUIResponse(
            "运行选择", "如有物料待吸取，选择继续吸放!\n如没有载板，选择上载板并继续!\n如有载板但无物料待吸，选择剥料并继续!",
            MsgBoxIcon::Icon::Information, { QString("继续吸放"), QString("上载板并继续"), QString("剥料并继续"), QString("取消运行") });
        if (option == "继续吸放")
        {
            qDebug() << "继续吸放";
            pushEvent(WorkerEvent("PickAndPlace"));
            return;
        }
        else if (option == "上载板并继续")
        {
            qDebug() << "上载板并继续";
            qDebug() << "MoveToCutCarrierBoard_XY";
            pushEvent(WorkerEvent("MoveToCutCarrierBoard_XY"));

            qDebug() << "AskFeedCarrierBoardTest";
            pushEvent(WorkerEvent("AskFeedCarrierBoardTest"));

            qDebug() << "AskFeedingCutCarrierBoard";
            pushEvent(WorkerEvent("AskFeedingCutCarrierBoard"));

            qDebug() << "AskFeedingTakeMaterialAwayWithOffset";
            pushEvent(WorkerEvent("AskFeedingTakeMaterialAwayWithOffset"));

            qDebug() << "PickAndPlace";
            pushEvent(WorkerEvent("PickAndPlace"));
            return;
        }
        else if (option == "剥料并继续")
        {
            qDebug() << "AskFeedingTakeMaterialAwayWithOffset";
            pushEvent(WorkerEvent("AskFeedingTakeMaterialAwayWithOffset"));

            qDebug() << "PickAndPlace";
            pushEvent(WorkerEvent("PickAndPlace"));
            return;
        }
        else if (option == "取消运行")
        {
            qDebug() << "取消运行";
            return;
        }
    }
    catch (SilicoolException &ex)
    {
        throw ex;
    }
}

void PickArm::PickAndPlace()
{
    if (IsStop)
    {
        return;
    }

    if (pPickArmTrayConfig->columns() % 2 == 0)
    {
        for (int i = 0; i < pPickArmTrayConfig->columns() / 2; i++)
        {
            if (hasAbortOrAutoStopEvent())
            {
                return;
            }
            PickMaterialPosUseCamera(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
            PickMaterialPosUseCamera(false, RightNozzlePickMaterial, RightPickMaterialTheta);
            MoveToPickMaterialPos(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
            MoveToPickMaterialPos(false, RightNozzlePickMaterial, RightPickMaterialTheta);

            PlaceMaterialPosUseCamera(true, LeftNozzlePlaceMaterial);
            PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);

            if (fabs(LeftNozzlePlaceMaterial.x()) < 1e-6 && fabs(LeftNozzlePlaceMaterial.y()) < 1e-6)
            {
                ErrorHandlerCls err("PickArm", "盘子拍照返回的结果为0.\n继续拍照请点继续!\n返回请手动移除物料并点取消运行！",
                                    { QVariant("继续"), QVariant("取消运行") });
                QString option = ErrorHandler::getErrorHandlingOption(err);

                if (option == "继续")
                {
                    qDebug() << "继续";
                    PlaceMaterialPosUseCamera(true, LeftNozzlePlaceMaterial);
                }
                else if (option == "取消运行")
                {
                    qDebug() << "取消运行";
                    return;
                }
            }
            else
            {
                MoveToPlaceMaterialPos(true, LeftNozzlePlaceMaterial);
            }
            if (fabs(LeftNozzlePlaceMaterial.x()) < 1e-6 && fabs(LeftNozzlePlaceMaterial.y()) < 1e-6)
            {
                ErrorHandlerCls err("PickArm", "盘子拍照返回的结果为0.\n继续拍照请点继续!\n返回请手动移除物料并点取消运行！",
                                    { QVariant("继续"), QVariant("取消运行") });
                QString option = ErrorHandler::getErrorHandlingOption(err);

                if (option == "继续")
                {
                    qDebug() << "继续";
                    PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);
                }
                else if (option == "取消运行")
                {
                    qDebug() << "取消运行";
                    return;
                }
            }
            else
            {
                MoveToPlaceMaterialPos(false, RightNozzlePlaceMaterial);
            }
        }
        pushEvent(WorkerEvent("AskFeedingTakeMaterialAwayWithOffset"));
        if (IsFeedingTakeMaterialAway)
        {
            IsFeedingTakeMaterialAway = false;
            AskFeedingOutCarrierBoard();
            return;
        }
        else
        {
            pushEvent(WorkerEvent("PickAndPlace"));
        }
    }
    else
    {
        for (int i = 0; i < pPickArmTrayConfig->columns() / 2; i++)
        {
            if (IsStop)
            {
                return;
            }
            PickMaterialPosUseCamera(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
            PickMaterialPosUseCamera(false, RightNozzlePickMaterial, RightPickMaterialTheta);
            MoveToPickMaterialPos(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
            MoveToPickMaterialPos(false, RightNozzlePickMaterial, RightPickMaterialTheta);

            PlaceMaterialPosUseCamera(true, LeftNozzlePlaceMaterial);
            PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);

            MoveToPlaceMaterialPos(true, LeftNozzlePlaceMaterial);
            MoveToPlaceMaterialPos(false, RightNozzlePlaceMaterial);
        }

        PickMaterialPosUseCamera(false, RightNozzlePickMaterial, RightPickMaterialTheta);
        MoveToPickMaterialPos(false, RightNozzlePickMaterial, RightPickMaterialTheta);
        PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);
        MoveToPlaceMaterialPos(false, RightNozzlePlaceMaterial);

        pushEvent(WorkerEvent("AskFeedingTakeMaterialAwayWithOffset"));
        if (IsFeedingTakeMaterialAway)
        {
            IsFeedingTakeMaterialAway = false;
            AskFeedingOutCarrierBoard();
            return;
        }
        else
        {
            pushEvent(WorkerEvent("PickAndPlace"));
        }
    }
}

void PickArm::PickMaterialTest(bool IsLeftNozzle)
{
    if (IsLeftNozzle)
    {
        PickMaterialPosUseCamera(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
        MoveToPickMaterialPos(true, LeftNozzlePickMaterial, LeftPickMaterialTheta);
    }
    else
    {
        PickMaterialPosUseCamera(false, RightNozzlePickMaterial, RightPickMaterialTheta);
        MoveToPickMaterialPos(false, RightNozzlePickMaterial, RightPickMaterialTheta);
    }
}

void PickArm::PlaceMaterialTest(bool IsLeftNozzle)
{
    if (IsLeftNozzle)
    {
        PlaceMaterialPosUseCamera(true, LeftNozzlePlaceMaterial);
        MoveToPlaceMaterialPos(true, LeftNozzlePlaceMaterial);
    }
    else
    {
        PlaceMaterialPosUseCamera(false, RightNozzlePlaceMaterial);
        MoveToPlaceMaterialPos(false, RightNozzlePlaceMaterial);
    }
}

void PickArm::WaitTrayOk(bool IsLeftNozzle) {}

//待测试方法
void PickArm::MoveToStandbyPos(bool IsManual)
{
    double nPos = pPACZ->axis()->getFeedbackPos(-1);
    if (nPos < pPACZ->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPACZ->moveTo(SAxisPosName::StandbyPos, false);
    }

    nPos = pPAZL->axis()->getFeedbackPos(-1);
    if (nPos < pPAZL->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZL->moveTo(SAxisPosName::StandbyPos, false);
    }

    nPos = pPAZR->axis()->getFeedbackPos(-1);
    if (nPos < pPAZR->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZR->moveTo(SAxisPosName::StandbyPos, false);
    }
    pPACZ->axis()->waitArrivedPos();
    pPAZL->axis()->waitArrivedPos();
    pPAZR->axis()->waitArrivedPos();

    pPAθL->moveTo(SAxisPosName::StandbyPos, false);
    pPAθR->moveTo(SAxisPosName::StandbyPos, false);
    pPAXY->moveTo(XYPosName::StandbyPos, false);
    pPAθL->axis()->waitArrivedPos();
    pPAθR->axis()->waitArrivedPos();
    pPAXY->waitArrivedPos(XYPosName::StandbyPos);
}

void PickArm::OpenVacuum(bool IsSuctionA) {}

void PickArm::CloseVacuum(bool IsSuctionA) {}

#define PickMaterialRegionFoldIngStart {    // Pick Material region
void PickArm::PAr_GoToStandby(bool IsLeftNozzle)
{
    SingleAxis *rAxis = IsLeftNozzle ? pPAθL : pPAθR;
    rAxis->moveTo(SAxisPosName::StandbyPos, false);
}

void PickArm::PAr_GoToStandbyOK(bool IsLeftNozzle)
{
    if (IsLeftNozzle)
    {
        pPAθL->waitArrivedPos(SAxisPosName::StandbyPos);
    }
    else
    {
        pPAθR->waitArrivedPos(SAxisPosName::StandbyPos);
    }
}

void PickArm::ReadData123(int clientID, QString IP, int Port, QByteArray data)
{
    QByteArray buffer = data;
    QString msg = QTextCodec::codecForName("GBK")->toUnicode(buffer);
    qDebug() << "ReadData123321";
}

void PickArm::PickMaterialPosUseCamera(bool IsLeftNozzle, QPointF &nPickMaterialPos, double &nTheta)
{
    if (hasAbortOrAutoStopEvent())
    {
        return;
    }
    nPickMaterialPos.setX(0);
    nPickMaterialPos.setY(0);
    nTheta = 0;
    PAr_GoToStandby(IsLeftNozzle);
    MoveToTakeMaterialPicturePos();
    PAr_GoToStandbyOK(IsLeftNozzle);
    PrResult nPrResult = GetPrResult(PrType::MaterialPr);
    PrOffset nPickOffset;
    if (nPrResult.IsOk)
    {
        nPickOffset = nPrResult.mPrOffset;
        qDebug() << "H:" << nPickOffset.H;
        qDebug() << "W:" << nPickOffset.W;
        qDebug() << "X:" << nPickOffset.X;
        qDebug() << "Y:" << nPickOffset.Y;

        double X = pPAXY->xAxis()->getFeedbackPos();
        double Y = pPAXY->yAxis()->getFeedbackPos();

        double nAngle = nPickOffset.Theta * M_PI / 180;
        nPickMaterialPos = QPointF(-nPickOffset.X + X, -nPickOffset.Y + Y);
        nPickMaterialPos
            += IsLeftNozzle ? QPointF(
                   pPAConfig->PA_L_Offset_X() + pPAConfig->GetMaterialFixOffset_X() * cos(nAngle) + pPAConfig->GetMaterialFixOffset_Y() * sin(nAngle),
                   pPAConfig->PA_L_Offset_Y() + pPAConfig->GetMaterialFixOffset_Y() * cos(nAngle) + pPAConfig->GetMaterialFixOffset_X() * sin(nAngle))
                            : QPointF(pPAConfig->PA_R_Offset_X() + pPAConfig->GetMaterialFixOffset_X() * cos(nAngle)
                                          + pPAConfig->GetMaterialFixOffset_Y() * sin(nAngle),
                                      pPAConfig->PA_R_Offset_Y() + pPAConfig->GetMaterialFixOffset_Y() * cos(nAngle)
                                          + pPAConfig->GetMaterialFixOffset_X() * sin(nAngle));
        nTheta = nPickOffset.Theta;
        PickArmMapInitIndex qPickArmMapInitIndex = GetPickArmMapInitIndex(false);
        qPickArmMapInitIndex.pPickArmMapData->setMaterialState(PickArmMapData::Ok);
    }
    else
    {
        ErrorHandlerCls err("PickArm", "物料PR返回失败", { QVariant("重试"), QVariant("继续"), QVariant("终止") });
        QString option = ErrorHandler::getErrorHandlingOption(err);

        if (option == "重试")
        {
            qDebug() << "重试";
            PickMaterialPosUseCamera(IsLeftNozzle, nPickMaterialPos, nTheta);
            return;
        }
        else if (option == "继续")
        {
            qDebug() << "继续";
            PickArmMapInitIndex qPickArmMapInitIndex = GetPickArmMapInitIndex(false);
            qPickArmMapInitIndex.pPickArmMapData->setMaterialState(PickArmMapData::Ng);
            PickMaterialPosUseCamera(IsLeftNozzle, nPickMaterialPos, nTheta);
            return;
        }
        else if (option == "终止")
        {
            qDebug() << "终止";
            throw SilicolAbort(tr("Material Pr return fail"));
        }
        // throw SilicolAbort(tr("Pr return fail"));
    }
}

void PickArm::PlaceMaterialPosUseCamera(bool IsLeftNozzle, QPointF &nPlaceMaterialPos)
{
    if (hasAbortOrAutoStopEvent())
    {
        return;
    }

    SCDI *pPickVac = IsLeftNozzle ? pPaVacSNR_L : pPaVacSNR_R;
    //    if(!pPickVac->get(true))
    //    {
    //        qDebug()<<"no material on nozzle";
    //        return;
    //    }
    nPlaceMaterialPos.setX(0);
    nPlaceMaterialPos.setY(0);
    MoveToTakeOutTrayPicturePos();

    PrResult nPrResult = GetPrResult(PrType::TrayPr);
    PrOffset nPlaceOffset;
    if (nPrResult.IsOk)
    {
        nPlaceOffset = nPrResult.mPrOffset;
        qDebug() << "H:" << nPlaceOffset.H;
        qDebug() << "W:" << nPlaceOffset.W;
        qDebug() << "X:" << nPlaceOffset.X;
        qDebug() << "Y:" << nPlaceOffset.Y;

        double X = pPAXY->xAxis()->getFeedbackPos() + pPAConfig->GetMaterialFixOffset_Y();
        double Y = pPAXY->yAxis()->getFeedbackPos();

        nPlaceMaterialPos = QPointF(-nPlaceOffset.X + X, -nPlaceOffset.Y + Y);
        nPlaceMaterialPos += IsLeftNozzle ? QPointF(pPAConfig->PA_L_Offset_X() + pPAConfig->PlaceMaterialFixOffset_X(),
                                                    pPAConfig->PA_L_Offset_Y() + pPAConfig->PlaceMaterialFixOffset_Y())
                                          : QPointF(pPAConfig->PA_R_Offset_X() + pPAConfig->PlaceMaterialFixOffset_X(),
                                                    pPAConfig->PA_R_Offset_Y() + pPAConfig->PlaceMaterialFixOffset_Y());
        DischargeMapOffset nDischargeMapOffset = pDischarge->GetDischargeOffset();
        nDischargeMapOffset.pDischargeMapData->setMaterialState(DischargeMapData::Ok);
    }
    else
    {
        ErrorHandlerCls err("PickArm", "盘子PR返回失败", { QVariant("重试"), QVariant("继续"), QVariant("终止") });
        QString option = ErrorHandler::getErrorHandlingOption(err);

        if (option == "重试")
        {
            qDebug() << "重试";
            PlaceMaterialPosUseCamera(IsLeftNozzle, nPlaceMaterialPos);
            return;
        }
        else if (option == "继续")
        {
            qDebug() << "继续";
            DischargeMapOffset nDischargeMapOffset = pDischarge->GetDischargeOffset();
            nDischargeMapOffset.pDischargeMapData->setMaterialState(DischargeMapData::Ng);
            PlaceMaterialPosUseCamera(IsLeftNozzle, nPlaceMaterialPos);
            return;
        }
        else if (option == "终止")
        {
            qDebug() << "终止";
            throw SilicolAbort(tr("Tray Pr return fail"));
        }
    }
}

void PickArm::MoveToPickMaterialPos(bool IsLeftNozzle, QPointF nPickMaterialPos, double nDelta)
{
    if (hasAbortOrAutoStopEvent())
    {
        return;
    }
    SingleAxis *PAθ = IsLeftNozzle ? pPAθL : pPAθR;
    pPAXY->moveTo(nPickMaterialPos);
    PickMterial(IsLeftNozzle);
    PAθ->axis()->relMove(-nDelta + 90 + pPAConfig->PlaceMaterialFixOffset_Theta(), false);
}

void PickArm::MoveToPlaceMaterialPos(bool IsLeftNozzle, QPointF nPlaceMaterialPos)
{
    if (hasAbortOrAutoStopEvent())
    {
        return;
    }
    if (IsStop)
    {
        return;
    }
    //此处实现配合移动放料。
    // pDischarge->TrayMoveToPlaceMaterialPos( );

    pPAXY->moveTo(nPlaceMaterialPos);

    SingleAxis *PAθ = IsLeftNozzle ? pPAθL : pPAθR;
    PAθ->axis()->waitArrivedPos();
    PlaceMaterial(IsLeftNozzle);
}

void PickArm::PickMterial(bool IsLeftNozzle)
{
    QString nNozzleName = IsLeftNozzle ? tr("NozzleL") : tr("NozzleR");
    QString getMaterialPosDown = "";
    QString getMaterialPosUp = "";

    getMaterialPosDown = IsLeftNozzle ? SAxisPosName::LPaPickMaterialPosDown : SAxisPosName::RPaPickMaterialPosDown;
    getMaterialPosUp = IsLeftNozzle ? SAxisPosName::LPaPickMaterialPosUp : SAxisPosName::RPaPickMaterialPosUp;

    SCVacuum *vacuum = IsLeftNozzle ? pPickVac_L : pPickVac_R;
    SingleAxis *zAxis = IsLeftNozzle ? pPAZL : pPAZR;
    SCDI *pPickVac = IsLeftNozzle ? pPaVacSNR_L : pPaVacSNR_R;
    if (pPickVac->get())
    {
        throw SilicolAbort(tr("%1_VCM has material, please remove by hand first!").arg(nNozzleName));
    }

    zAxis->moveTo(getMaterialPosUp);
    vacuum->set(false, false);
    if (!vacuum->waitUntilTimeout(false))
    {
        MoveAllAxisUp();
        ErrorHandlerCls err("PickArm", "Wait Vacuum Timeout", { QVariant("重试"), QVariant("继续"), QVariant("终止") });
        QString option = ErrorHandler::getErrorHandlingOption(err);

        if (option == "重试")
        {
            qDebug() << "重试";
            PickMterial(IsLeftNozzle);
            return;
        }
        else if (option == "继续")
        {
            qDebug() << "继续";
            DischargeMapOffset nDischargeMapOffset = pDischarge->GetDischargeOffset();
            nDischargeMapOffset.pDischargeMapData->setMaterialState(DischargeMapData::Ng);
            return;
        }
        else if (option == "终止")
        {
            qDebug() << "终止";
            throw SilicolAbort(tr("Wait Vacuum Timeout"));
        }
    }

    zAxis->moveTo(getMaterialPosDown);
    vacuum->set(true, false);
    if (!vacuum->waitUntilTimeout(true))
    {
        vacuum->set(false);
        msleep(500);
        MoveAllAxisUp();
        ErrorHandlerCls err("PickArm", "Wait Vacuum Timeout", { QVariant("重试"), QVariant("继续"), QVariant("终止") });
        QString option = ErrorHandler::getErrorHandlingOption(err);

        if (option == "重试")
        {
            qDebug() << "重试";
            zAxis->moveTo(getMaterialPosDown);
            vacuum->set(true);
            zAxis->moveTo(getMaterialPosUp);
            // PickMterial(IsLeftNozzle);
            return;
        }
        else if (option == "继续")
        {
            qDebug() << "继续";
            return;
        }
        else if (option == "终止")
        {
            qDebug() << "终止";
            throw SilicolAbort(tr("Wait Vacuum Timeout"));
        }
    }
    if (pPAConfig->VacummDelay() > 0)
    {
        msleep(pPAConfig->VacummDelay());
    }
    zAxis->moveTo(getMaterialPosUp);

    if (!pPickVac->checkState(true))
    {
        vacuum->set(false);
        throw SilicolAbort(tr("物料未吸起"));
    }
}

void PickArm::PlaceMaterial(bool IsLeftNozzle)
{
    QString PlaceMaterialPosDown = IsLeftNozzle ? SAxisPosName::LPaPlaceMaterialPosDown : SAxisPosName::RPaPlaceMaterialPosDown;
    QString PlaceMaterialPosUp = IsLeftNozzle ? SAxisPosName::LPaPlaceMaterialPosUp : SAxisPosName::RPaPlaceMaterialPosUp;

    SCVacuum *vacuum = IsLeftNozzle ? pPickVac_L : pPickVac_R;
    SingleAxis *zAxis = IsLeftNozzle ? pPAZL : pPAZR;

    zAxis->moveTo(PlaceMaterialPosDown);
    vacuum->set(false);
    zAxis->moveTo(PlaceMaterialPosUp);
}

////////////////////////////////////////////////

void PickArm::MoveToPosWithRowCol(int rowIndex, int columnIndex, bool IsMoveXY, bool IsNeedAddUnitDeltaFlag)
{
    qDebug("MoveToPosWithRow");
    int unitRowIndex = rowIndex / pPickArmTrayConfig->rowsInsideUnit();
    int rowIndexInsideUnit = rowIndex % pPickArmTrayConfig->rowsInsideUnit();
    int unitColumnIndex = columnIndex / pPickArmTrayConfig->columnsInsideUnit();
    int columnIndexInsideUnit = columnIndex % pPickArmTrayConfig->columnsInsideUnit();
    MoveToPosWithRowCol(unitRowIndex, unitColumnIndex, rowIndexInsideUnit, columnIndexInsideUnit, IsMoveXY, IsNeedAddUnitDeltaFlag);
}

void PickArm::MoveToPosWithRowCol(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex, bool IsMoveXY, bool IsNeedAddUnitDeltaFlag)
{
    qDebug("pDetaperZ move");
    auto pPos = pPAXY->getPos<XYModulePos>(XYPosName::TakePictureForPickPos);
    double xDelta = unitRowIndex * pPickArmTrayConfig->unitRowDelta();
    qDebug() << "1-xDelta: " << xDelta;
    qDebug() << "1-unitRowIndex: " << unitRowIndex;
    qDebug() << "1-pPickArmTrayConfig->unitRowDelta(): " << pPickArmTrayConfig->unitRowDelta();

    // double yDelta = unitRowIndex * trayConfig->unitRowDelta() + rowIndex * trayConfig->rowDelta();
    double yDelta = unitColumnIndex * pPickArmTrayConfig->unitColumnDelta() + columnIndex * pPickArmTrayConfig->columnDelta();
    QPointF nQPointF;
    nQPointF.setX(pPos->xPos() + yDelta);
    nQPointF.setY(pPos->yPos());

    if (IsMoveXY)
    {
        pPAXY->moveTo(nQPointF);    // ForTakePicture
    }
    if (!IsMoveXY)
    {
        if (IsNeedAddUnitDeltaFlag)
        {
            pFeeding->TakeMaterialAwayWithOffset(xDelta, false);
        }
        else
        {
            pFeeding->TakeMaterialAwayWithOffset(pFeeding->pFeedingConfig->FeedingStep(), false);
        }
    }
}

void PickArm::MoveToTakeMaterialPicturePos()
{
    double nPos = pPACZ->axis()->getFeedbackPos(-1);
    if (nPos < pPACZ->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPACZ->moveTo(SAxisPosName::StandbyPos, false);
    }

    nPos = pPAZL->axis()->getFeedbackPos(-1);
    if (nPos < pPAZL->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZL->moveTo(SAxisPosName::StandbyPos, false);
    }

    nPos = pPAZR->axis()->getFeedbackPos(-1);
    if (nPos < pPAZR->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZR->moveTo(SAxisPosName::StandbyPos, false);
    }

    pPACZ->waitArrivedPos(SAxisPosName::StandbyPos);
    pPAZL->waitArrivedPos(SAxisPosName::StandbyPos);
    pPAZR->waitArrivedPos(SAxisPosName::StandbyPos);
    //  double X = pPAXY->getPos<XYModulePos>(XYPosName::TakePictureForMaterialA)->xPos();
    //  double Y = pPAXY->getPos<XYModulePos>(XYPosName::TakePictureForMaterialA)->yPos();

    PickArmMapInitIndex qPickArmMapInitIndex = GetPickArmMapInitIndex(false);
    if (qPickArmMapInitIndex.IsIndexEnd && qPickArmMapInitIndex.RowIndex)
    {
        pPickArmMap->reset(PickArmMapData::Init);
        //        AskFeedingOutCarrierBoard();
        //        AskFeedCarrierBoard();
        qDebug() << "盘子最后一个做完了";
    }
    else
    {
        MoveToPosWithRowCol(qPickArmMapInitIndex.RowIndex, qPickArmMapInitIndex.ColumnIndex, true, false);
        qDebug() << "行数: " << qPickArmMapInitIndex.RowIndex;
        qDebug() << "行总数: " << pPickArmTrayConfig->rows();
        qDebug() << "列数: " << qPickArmMapInitIndex.ColumnIndex;
        qDebug() << "列总数: " << pPickArmTrayConfig->columns();
    }
}

void PickArm::Test()
{
    checkIsInit();
    PickArmMapInitIndex qPickArmMapInitIndex = GetPickArmMapInitIndex(false);
    if (qPickArmMapInitIndex.IsIndexEnd)
    {
        pPickArmMap->reset(PickArmMapData::Init);
        qDebug() << "行总数时最后一个做完了";
    }
    else
    {
        qDebug() << "行数: " << qPickArmMapInitIndex.RowIndex;
        qDebug() << "行总数: " << pPickArmTrayConfig->rows();
        qDebug() << "列数: " << qPickArmMapInitIndex.ColumnIndex;
        qDebug() << "列总数: " << pPickArmTrayConfig->columns();
        qPickArmMapInitIndex.pPickArmMapData->setMaterialState(PickArmMapData::Ok);
    }

    // pushEvent(WorkerEvent("MoveToTakeOutTrayPicturePos"));
    // MoveToTakeOutTrayPicturePos();
    // MoveToTakeMaterialPicturePos();
}

void PickArm::StopTest()
{
    IsStop = true;
}

PickArmMapInitIndex PickArm::GetPickArmMapInitIndex(bool IsInit)
{
    PickArmMapInitIndex nPickArmMapInitIndex;
    for (int rowIndex = 0; rowIndex < pPickArmTrayConfig->rows(); rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < pPickArmTrayConfig->columns(); columnIndex++)
        {
            PickArmMapData *mPickArmMapData = pPickArmMap->getData<PickArmMapData>(rowIndex, columnIndex);
            nPickArmMapInitIndex.pPickArmMapData = mPickArmMapData;
            nPickArmMapInitIndex.RowIndex = rowIndex;
            nPickArmMapInitIndex.ColumnIndex = columnIndex;
            if (IsInit)
            {
                if (mPickArmMapData->materialState() == PickArmMapData::Init)
                {
                    return nPickArmMapInitIndex;
                }
            }
            else
            {
                if (mPickArmMapData->materialState() == PickArmMapData::InUse)
                {
                    return nPickArmMapInitIndex;
                }
            }
        }
    }
    nPickArmMapInitIndex.IsIndexEnd = true;
    return nPickArmMapInitIndex;
}

void PickArm::MoveToTakeOutTrayPicturePos()
{
    if (IsStop)
    {
        return;
    }
    double nPos = pPACZ->axis()->getFeedbackPos(-1);
    if (nPos < pPACZ->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPACZ->moveTo(SAxisPosName::StandbyPos, false);
    }
    nPos = pPAZL->axis()->getFeedbackPos(-1);
    if (nPos < pPAZL->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZL->moveTo(SAxisPosName::StandbyPos, false);
    }
    nPos = pPAZR->axis()->getFeedbackPos(-1);
    if (nPos < pPAZR->getPos<SingleAxisPos>(SAxisPosName::StandbyPos)->pos() - 1)
    {
        pPAZR->moveTo(SAxisPosName::StandbyPos, false);
    }
    pPACZ->axis()->waitArrivedPos();
    pPAZL->axis()->waitArrivedPos();
    pPAZR->axis()->waitArrivedPos();

    //放料拍照位
    m_DischargeMapOffset = pDischarge->TrayMoveToPlaceMaterialPos(0);    //移动trackX轴优先，判断到需要移动PaX时则需要移动X轴。

    XYModulePos *pPos = pPAXY->getPos<XYModulePos>(XYPosName::TakePictureForTrayPos);
    // DischargeMapOffset nDischargeMapOffset = pDischarge->GetDischargeOffset();
    QPointF nQPointF;
    if (m_DischargeMapOffset.IsMovePaX)
    {
        nQPointF.setX(pPos->xPos() + m_DischargeMapOffset.OffsetX);
        nQPointF.setY(pPos->yPos() + m_DischargeMapOffset.OffsetY);
    }
    else
    {
        nQPointF.setX(pPos->xPos());
        nQPointF.setY(pPos->yPos() + m_DischargeMapOffset.OffsetY);
    }
    pPAXY->moveTo(nQPointF);

    pDischarge->MoveToPlaceMaterialTrackZ();
}

#define PickMaterialRegionFoldIngEnd }

void PickArm::AskFeedCarrierBoardTest()
{
    pFeeding->FeedMag();
    pPickArmMap->reset(PickArmMapData::Init);
    OldUnitRowIndex = 0;
}

void PickArm::MoveToCutCarrierBoard_XY()
{
    // XY去到下切刀位，并置位状态告知Feeding模块。
    pPAXY->moveTo(XYPosName::CutCarrierBoardPosXY);
}

void PickArm::AskFeedingCutCarrierBoard()
{
    if (IsStop)
    {
        return;
    }
    pFeeding->CutCarrierBoard();
}

void PickArm::AskFeedingTakeMaterialAwayWithOffsetFirst()
{
    PickNum = pPickArmTrayConfig->columns();
    pFeeding->TakeMaterialAwayWithOffset(pFeeding->pFeedingConfig->FeedingStep(), true);
}

void PickArm::AskFeedingTakeMaterialAwayWithOffset()
{
    if (IsStop)
    {
        return;
    }
    PickArmMapInitIndex MapInitIndexTemp = GetPickArmMapInitIndex(true);

    if (MapInitIndexTemp.IsIndexEnd)
    {
        IsFeedingTakeMaterialAway = true;
        return;
    }
    else
    {
        if (OldUnitRowIndex != MapInitIndexTemp.RowIndex / pPickArmTrayConfig->rowsInsideUnit())
        {
            qDebug() << "新旧不等时，新UnitRowIndex" << MapInitIndexTemp.RowIndex / pPickArmTrayConfig->rowsInsideUnit();
            MoveToPosWithRowCol(MapInitIndexTemp.RowIndex, MapInitIndexTemp.ColumnIndex, false, true);

            OldUnitRowIndex = MapInitIndexTemp.RowIndex / pPickArmTrayConfig->rowsInsideUnit();
            UpdateMapAfterDetaper(MapInitIndexTemp.RowIndex);
        }
        else
        {
            qDebug() << "新旧相等时，UnitRowIndex" << MapInitIndexTemp.RowIndex / pPickArmTrayConfig->rowsInsideUnit();
            MoveToPosWithRowCol(MapInitIndexTemp.RowIndex, MapInitIndexTemp.ColumnIndex, false, false);
            UpdateMapAfterDetaper(MapInitIndexTemp.RowIndex);
        }
    }
}

void PickArm::UpdateMapAfterDetaper(int nRowIndex)
{
    for (int rowIndex = nRowIndex; rowIndex == nRowIndex; rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < pPickArmTrayConfig->columns(); columnIndex++)
        {
            PickArmMapData *mPickArmMapData = pPickArmMap->getData<PickArmMapData>(rowIndex, columnIndex);
            if (mPickArmMapData->materialState() == PickArmMapData::Init)
            {
                mPickArmMapData->setMaterialState(PickArmMapData::InUse);
            }
        }
    }
}

void PickArm::AskFeedingOutCarrierBoard()
{
    pFeeding->OutCarrierBoard();
}

void PickArm::WaitAxisStop() {}

void PickArm::GetVisionResult(POINT &nPoint, double &nAngle)
{
    PRResultStruct prOffset;
    pPACalibOffset_Location->performPR(prOffset);
}

void PickArm::TakePicture(bool IsLeft)
{
    if (IsLeft)
    {
    }
    else
    {
    }
}

void PickArm::AskFeedCarrierBoard()
{
    QVariantList nColumnParam;
    nColumnParam << 1;
    pushEvent(WorkerEvent("CalculatePosFromMap", nColumnParam));
}

void PickArm::VisionTest()
{
    PrResult glassPrOffset = GetPrResult(PrType::CalibrationPr);
    qDebug() << "H:" << glassPrOffset.mPrOffset.H;
    qDebug() << "W:" << glassPrOffset.mPrOffset.W;
    qDebug() << "X:" << glassPrOffset.mPrOffset.X;
    qDebug() << "Y:" << glassPrOffset.mPrOffset.Y;
}

void PickArm::CalibrateCameraToNozzleOffset(bool IsLeftNozzle)
{
    checkIsInit();
    QString paName = IsLeftNozzle ? tr("Left_Nozzle") : tr("Right_Nozzle");
    QString confirmString = tr("Make sure calibration glass is ready for %1 calibrate, please.").arg(paName);
    SingleAxis *thetaAxis = IsLeftNozzle ? pPAθL : pPAθR;

    if (!UIOperation::getIns()->okCancelConfirm(confirmString))
    {
        return;
    }
    if (pPAConfig->PACalibStepCount() < 3)
    {
        UIOperation::getIns()->okCancelConfirm(
            tr("PA calibrate step should More than 3! \r\ncurrent settint step: ").arg(pPAConfig->PACalibStepCount()));
        return;
    }

    MoveAllAxisUp();

    QString calibPosName = XYPosName::CalibrateCameraToNozzlePrPos;
    thetaAxis->moveTo(SAxisPosName::StandbyPos);
    pPAXY->moveTo(calibPosName);
    msleep(500);
    PrResult glassPrOffset = GetPrResult(PrType::CalibrationPr);
    qDebug() << "H:" << glassPrOffset.mPrOffset.H;
    qDebug() << "W:" << glassPrOffset.mPrOffset.W;
    qDebug() << "X:" << glassPrOffset.mPrOffset.X;
    qDebug() << "Y:" << glassPrOffset.mPrOffset.Y;

    double X = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->xPos();
    double Y = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->yPos();

    QPointF getGlassPos = QPointF(-glassPrOffset.mPrOffset.X + X, -glassPrOffset.mPrOffset.Y + Y);
    getGlassPos += IsLeftNozzle ? QPointF(pPAConfig->PA_L_Offset_X(), pPAConfig->PA_L_Offset_Y())
                                : QPointF(pPAConfig->PA_R_Offset_X(), pPAConfig->PA_R_Offset_Y());
    QVector<QPointF> offsets;
    for (int i = 0; i < pPAConfig->PACalibStepCount(); i++)
    {
        PrOffset offset;
        if (i == 0)
        {
            offset = glassPrOffset.mPrOffset;
        }
        else
        {
            offset = GetPrResult(PrType::CalibrationPr).mPrOffset;
        }
        offsets.append(QPointF(offset.X, offset.Y));

        if (i != pPAConfig->PACalibStepCount() - 1)
        {

            // pPAXY->relMove(QPointF(pPAConfig->PA_L_Offset_X(), pPAConfig->PA_L_Offset_Y()));
            pPAXY->moveTo(getGlassPos);
            msleep(500);
            PickCalibrationGlass(IsLeftNozzle);

            SingleAxis *thetaAxis = IsLeftNozzle ? pPAθL : pPAθR;
            thetaAxis->axis()->relMove(pPAConfig->CalibrateStep());

            QThread::msleep(200);
            PlaceCalibrationGlass(IsLeftNozzle);
            pPAXY->moveTo(calibPosName);
            QThread::msleep(1000);
        }
    }

    double radius;
    QPointF nozzelCenter;
    fitCircle(offsets, nozzelCenter, radius);
    qDebug() << "nozzelCenterX:" << nozzelCenter.x();
    qDebug() << "nozzelCenterY:" << nozzelCenter.y();

    QPointF offsetShift = nozzelCenter - QPointF(glassPrOffset.mPrOffset.X, glassPrOffset.mPrOffset.Y);

    double newPAToCameraOffsetX = offsetShift.x();
    newPAToCameraOffsetX += IsLeftNozzle ? pPAConfig->PA_L_Offset_X() : pPAConfig->PA_R_Offset_X();
    double newPAToCameraOffsetY = offsetShift.y();
    newPAToCameraOffsetY += IsLeftNozzle ? pPAConfig->PA_L_Offset_Y() : pPAConfig->PA_R_Offset_Y();

    QString saveConfirmString = tr("Calibrate %1 to PA Camere offset successful!\r\nNew core X_offset: %2, Y_offset: %3"
                                   "\r\nNew X_offset: %5, Y_offset: %6\r\n"
                                   "Would you like to save?")
                                    .arg(paName)
                                    .arg(offsetShift.x())
                                    .arg(offsetShift.y())
                                    .arg(newPAToCameraOffsetX)
                                    .arg(newPAToCameraOffsetY);

    if (UIOperation::getIns()->yesNoConfirm(saveConfirmString))
    {
        if (IsLeftNozzle)
        {
            pPAConfig->setPA_L_Offset_X(newPAToCameraOffsetX);
            pPAConfig->setPA_L_Offset_Y(newPAToCameraOffsetY);
        }
        else
        {
            pPAConfig->setPA_R_Offset_X(newPAToCameraOffsetX);
            pPAConfig->setPA_R_Offset_Y(newPAToCameraOffsetY);
        }
    }
}

void PickArm::XY_MoveToTakePictureForGlassPos()
{
    pPAXY->moveTo(XYPosName::CalibrateCameraToNozzlePrPos);
}

void PickArm::XY_MoveToGetGlassPos(bool IsLeftNozzle)
{
    double X = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->xPos();
    double Y = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->yPos();

    QPointF getGlassPos = QPointF(X, Y);
    getGlassPos += IsLeftNozzle ? QPointF(pPAConfig->PA_L_Offset_X(), pPAConfig->PA_L_Offset_Y())
                                : QPointF(pPAConfig->PA_R_Offset_X(), pPAConfig->PA_R_Offset_Y());
    pPAXY->moveTo(getGlassPos);
}

void PickArm::PickGlassWithOffset(bool IsLeftNozzle)
{
    pPAXY->moveTo(XYPosName::CalibrateCameraToNozzlePrPos);
    PrResult nPrResult = GetPrResult(PrType::CalibrationPr);
    PrOffset glassPrOffset;
    if (nPrResult.IsOk)
    {
        glassPrOffset = nPrResult.mPrOffset;
        qDebug() << "H:" << glassPrOffset.H;
        qDebug() << "W:" << glassPrOffset.W;
        qDebug() << "X:" << glassPrOffset.X;
        qDebug() << "Y:" << glassPrOffset.Y;

        double X = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->xPos();
        double Y = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->yPos();

        QPointF getGlassPos = QPointF(-glassPrOffset.X + X, -glassPrOffset.Y + Y);
        getGlassPos += IsLeftNozzle ? QPointF(pPAConfig->PA_L_Offset_X(), pPAConfig->PA_L_Offset_Y())
                                    : QPointF(pPAConfig->PA_R_Offset_X(), pPAConfig->PA_R_Offset_Y());
        pPAXY->moveTo(getGlassPos);

        PickCalibrationGlass(IsLeftNozzle);
        QThread::msleep(200);
    }
    else
    {
        throw SilicolAbort(tr("Pr return fail"));
    }
}

void PickArm::PlaceGlassWithOffset(bool IsLeftNozzle)
{
    double X = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->xPos();
    double Y = pPAXY->getPos<XYModulePos>(XYPosName::CalibrateCameraToNozzlePrPos)->yPos();
    double PaOffsetX = IsLeftNozzle ? pPAConfig->PA_L_Offset_X() : pPAConfig->PA_R_Offset_X();
    double PaOffsetY = IsLeftNozzle ? pPAConfig->PA_L_Offset_Y() : pPAConfig->PA_R_Offset_Y();
    pPAXY->moveTo(QPointF(PaOffsetX + X, PaOffsetY + Y));
    QThread::msleep(200);
    PlaceCalibrationGlass(IsLeftNozzle);
    pPAXY->moveTo(XYPosName::CalibrateCameraToNozzlePrPos);
    QThread::msleep(400);
}

PickArm::PrResult PickArm::GetPrResult(PrType nPrType)
{
    PrResult nPrResult;
    if (nPrType == PrType::CalibrationPr)
    {
        if (pPACalibOffset_Location != nullptr)
            if (pPACalibOffset_Location->performPR(nPrResult.mPrOffset))
            {
                nPrResult.IsOk = true;
            }
    }
    else if (nPrType == PrType::MaterialPr)
    {
        if (pGetMaterialLocation != nullptr)
            if (pGetMaterialLocation->performPR(nPrResult.mPrOffset))
            {
                nPrResult.IsOk = true;
            }
    }
    else if (nPrType == PrType::TrayPr)
    {
        if (pPlaceMaterialLocation != nullptr)
            if (pPlaceMaterialLocation->performPR(nPrResult.mPrOffset))
            {
                nPrResult.IsOk = true;
            }
    }
    return nPrResult;
}

void PickArm::PickCalibrationGlass(bool IsLeftNozzle)
{
    QString nNozzleName = IsLeftNozzle ? tr("NozzleL") : tr("NozzleR");
    QString getGlassPosDown = IsLeftNozzle ? SAxisPosName::LPAPickCalibGlassPosDown : SAxisPosName::RPAPickCalibGlassPosDown;
    QString getGlassPosUp = IsLeftNozzle ? SAxisPosName::LPAPickCalibGlassPosUp : SAxisPosName::RPAPickCalibGlassPosUp;
    SCVacuum *vacuum = IsLeftNozzle ? pPickVac_L : pPickVac_R;
    SingleAxis *zAxis = IsLeftNozzle ? pPAZL : pPAZR;
    SCDI *pPickVac = IsLeftNozzle ? pPaVacSNR_L : pPaVacSNR_R;
    if (pPickVac->get())
    {
        throw SilicolAbort(tr("%1_VCM has material, please remove by hand first!").arg(nNozzleName));
    }

    zAxis->moveTo(getGlassPosUp);
    vacuum->set(false, false);
    if (!vacuum->waitUntilTimeout(false))
    {
        MoveAllAxisUp();
        throw SilicolAbort(tr("%1_vacuum->set(false) TimeOut!").arg(nNozzleName));
    }

    zAxis->moveTo(getGlassPosDown);
    vacuum->set(true);
    msleep(500);
    if (!vacuum->waitUntilTimeout(true))
    {
        MoveAllAxisUp();
        throw SilicolAbort(tr("%1_vacuum->set(true) TimeOut!").arg(nNozzleName));
    }
    if (pPAConfig->VacummDelay() > 0)
    {
        QThread::msleep(pPAConfig->VacummDelay());
    }
    zAxis->moveTo(getGlassPosUp);

    if (!vacuum->get())
    {
        vacuum->set(false);
        throw SilicolAbort(tr("%1 Get Calibrate Glass fail!").arg(nNozzleName));
    }
}

void PickArm::PlaceCalibrationGlass(bool IsLeftNozzle)
{
    QString getGlassPosDown = IsLeftNozzle ? SAxisPosName::LPAPickCalibGlassPosDown : SAxisPosName::RPAPickCalibGlassPosDown;
    QString getGlassPosUp = IsLeftNozzle ? SAxisPosName::LPAPickCalibGlassPosUp : SAxisPosName::RPAPickCalibGlassPosUp;
    SCVacuum *vacuum = IsLeftNozzle ? pPickVac_L : pPickVac_R;
    SingleAxis *zAxis = IsLeftNozzle ? pPAZL : pPAZR;

    zAxis->moveTo(getGlassPosDown);
    vacuum->set(false);
    msleep(500);
    zAxis->moveTo(getGlassPosUp);
}

void PickArm::MoveAllAxisUp()
{
    pPAZL->moveTo(SAxisPosName::StandbyPos);
    pPAZR->moveTo(SAxisPosName::StandbyPos);
    pPACZ->moveTo(SAxisPosName::StandbyPos);
}

bool PickArm::WaitOKFlag(bool IsFeeding)
{
    bool nOkFlag = false;
    while (true)
    {
        if (IsFeeding)
        {
            if (pFeeding->GetFeedingState().IsReadyToPickMaterial)
            {
                nOkFlag = true;
                break;
            }
        }
        else
        {
            if (pDischarge->GetDischargeState())
            {
                nOkFlag = true;
                break;
            }
        }
    }
    return nOkFlag;
}

QPointF PickArm::PrPosA(bool nIsMaterial)
{
    //获取当前点加上PR结果，存起来。
    double X = pPAXY->xAxis()->getFeedbackPos();
    double Y = pPAXY->yAxis()->getFeedbackPos();
    if (nIsMaterial)
    {
        PrResult nResult = GetPrResult(PrType::MaterialPr);
        if (nResult.IsOk)
        {
            QPointF PosA(-nResult.mPrOffset.X + X, -nResult.mPrOffset.Y + Y);
            return PosA;
        }
        else
        {
            qCritical() << "PR识别失败";
            return QPointF(0.0, 0.0);
        }
    }
    else
    {
        PrResult nResult = GetPrResult(PrType::TrayPr);
        if (nResult.IsOk)
        {
            QPointF PosA(-nResult.mPrOffset.X + X, -nResult.mPrOffset.Y + Y);
            return PosA;
        }
        else
        {
            qCritical() << "PR识别失败";
            return QPointF(0.0, 0.0);
        }
    }
}

QPointF PickArm::PrPosB(bool nIsMaterial)
{
    //获取当前点加上PR结果，存起来。
    double X = pPAXY->xAxis()->getFeedbackPos();
    double Y = pPAXY->yAxis()->getFeedbackPos();
    if (nIsMaterial)
    {
        PrResult nResult = GetPrResult(PrType::MaterialPr);
        if (nResult.IsOk)
        {
            QPointF PosB(-nResult.mPrOffset.X + X, -nResult.mPrOffset.Y + Y);
            return PosB;
        }
        else
        {
            qCritical() << "PR识别失败";
            return QPointF(0.0, 0.0);
        }
    }
    else
    {
        PrResult nResult = GetPrResult(PrType::TrayPr);
        if (nResult.IsOk)
        {
            QPointF PosB(-nResult.mPrOffset.X + X, -nResult.mPrOffset.Y + Y);
            return PosB;
        }
        else
        {
            qCritical() << "PR识别失败";
            return QPointF(0.0, 0.0);
        }
    }
}

QPointF PickArm::CalculateSpacePosAToPosB(QPointF nPosA, QPointF nPosB)
{
    QPointF nRes;
    QPointF nA = nPosA;
    QPointF nB = nPosB;
    nRes = nB - nA;
    if (abs(nRes.x()) < 1 && abs(nRes.y()) < 1)
    {
        qCritical() << "计算得到的结果太小，不符合。";
        return QPointF(0.0, 0.0);
    }
    else
    {
        qDebug() << "abs(nRes.x():" << abs(nRes.x()) << "abs(nRes.y()):" << abs(nRes.y());
        return QPointF(abs(nRes.x()), abs(nRes.y()));
    }
}

void PickArm::testGetSpace1(bool nIsMaterial)
{
    nSpaceTemp = PrPosA(nIsMaterial);
}

void PickArm::testGetSpace2(bool nIsMaterial, QString nCalculateType)
{
    if (!PosAPrFirst)
    {
        PosAPrFirst = false;
    }
    if (nCalculateType == "ForPickArmTrayConfig")
    {
        pPickArmTrayConfig->setRowDelta(CalculateSpacePosAToPosB(nSpaceTemp, PrPosB(nIsMaterial)).y());
        pPickArmTrayConfig->setColumnDelta(CalculateSpacePosAToPosB(nSpaceTemp, PrPosB(nIsMaterial)).x());
    }
    if (nCalculateType == "ForDischargeTrayConfig")
    {
        pDischarge->pDischargeTrayConfig->setRowDelta(CalculateSpacePosAToPosB(nSpaceTemp, PrPosB(nIsMaterial)).x());
        pDischarge->pDischargeTrayConfig->setColumnDelta(CalculateSpacePosAToPosB(nSpaceTemp, PrPosB(nIsMaterial)).y());
    }
}

void PickArm::keyBoardMove(QString nAxisName, int nDirection)
{
    if (nAxisName == "PaX")
    {
        pPAXY->xAxis()->jogMove(nDirection);
    }
    else if (nAxisName == "PaY")
    {
        pPAXY->yAxis()->jogMove(nDirection);
    }
}

void PickArm::keyBoardStop(QString nAxisName)
{
    if (nAxisName == "PaX")
    {
        pPAXY->xAxis()->stop();
    }
    else if (nAxisName == "PaY")
    {
        pPAXY->yAxis()->stop();
    }
}
