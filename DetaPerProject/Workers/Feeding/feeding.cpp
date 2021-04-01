#include "feeding.h"

Feeding::Feeding(TrayConfig *trayConfig, FeedingMap *feedingMap, FeedingConfig *feedingConfig, QObject *parent)
    : Worker(WorkerName::Feeding, parent), pFeedTrayConfig(trayConfig), pFeedingMap(feedingMap), pFeedingConfig(feedingConfig)
{
    pFeedingConfigFile = new ConfigFile("FeedingConfig", pFeedingConfig, QString("%1/FeedingConfig.json").arg(Worker::getWorkerConfigDir()));
    pFeedingConfigFile->populate(true);
}

Feeding::~Feeding() {}

void Feeding::initEventHandle()
{
    pElevatorX = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::ElevatorX);
    pElevatorZ = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::ElevatorZ);
    pDetaperZ = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::DetaperZ);

    pContactRollerL = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::ContactRollerL);
    pContactRollerR = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::ContactRollerR);
    pPickArmC_Z = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PACZ);

    pMagGripper = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::MagGripper);

    pMagKicker = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::MagKicker);                      //推载片入剥单模块气缸
    pDtStopper = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::DtStopper);                      //挡载片气缸（入载片时）
    pDtGripper = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::DtGripper);                      //夹膜气缸
    pDtKicker = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::DtKicker);                        //空载片回弹夹
    pCollectSNR = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::CollectSNR);                    //接物料平台移动气缸
    pTrayClamp = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::TrayClamp);                      //防翻载板压载板气缸
    pPreventOverTurnSNR = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::PreventOverTurnSNR);    //防翻料气缸

    pMagKickerIn = MotionElementContainer::getIns()->getItem<SCDI>(DiName::MagKickerIn);          //
    pMagKickerOut = MotionElementContainer::getIns()->getItem<SCDI>(DiName::MagKickerOut);        //
    pMagFullSNR = MotionElementContainer::getIns()->getItem<SCDI>(DiName::MagFullSNR);            //
    pCobCarrierSNR = MotionElementContainer::getIns()->getItem<SCDI>(DiName::CobCarrierSNR);      //
    pDtStopperIn = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtStopperIn);          //
    pDtStopperOut = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtStopperOut);        //
    pDtGripperIn = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtGripperIn);          //
    pDtGripperOut = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtGripperOut);        //
    pDtKickerIn = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtKickerIn);            //
    pDtKickerOut = MotionElementContainer::getIns()->getItem<SCDI>(DiName::DtKickerOut);          //
    pVacPlatformIn = MotionElementContainer::getIns()->getItem<SCDI>(DiName::VacPlatformIn);      //
    pVacPlatformOut = MotionElementContainer::getIns()->getItem<SCDI>(DiName::VacPlatformOut);    //
    pIsExistMag = MotionElementContainer::getIns()->getItem<SCDI>(DiName::IsExistMag);

    pSensorVac = MotionElementContainer::getIns()->getItem<SCDO>(DoName::SensorVac);
}

void Feeding::startEventHandle(const WorkerEvent &event)
{
    auto runParam = event.eventArgs.toMap();
    bool isDryRun = (runParam["RunMode"] == "DryRun");
    if (isDryRun)
    {
        DisableAllDi();
        // Do something...
    }
    else
    {
    }
    //    m_nContinuallyPrFailed = 0;
    //    //verifyTrayMap();

    //    // ---For Test---
    //    qsrand(time(0));
    //    dutDownlookLocation->disable();
    //    // ---For Test---
}

void Feeding::stopEventHandle()
{
    // Do something...
    // stopEventHandled();
}

void Feeding::allWorkerStopped()
{
    // Do something...
}

void Feeding::purgeEventHandle()
{
    // Do something...
    // purgeEventHandled();
}

void Feeding::resetEventHandle()
{
    // Do something...
    m_nContinuallyPrFailed = 0;
}

void Feeding::homeWorker(bool moveToReadyPosAfterHome)
{
    pDtGripper->set(false);
    pDetaperZ->axis()->home(false);

    pMagGripper->set(false);
    pElevatorX->axis()->home(false);
    pElevatorZ->axis()->home();

    pElevatorX->axis()->waitHomeDone();
    pDetaperZ->axis()->waitHomeDone();
    if (pIsExistMag->get())
    {
        pElevatorZ->moveTo(SAxisPosName::OutMagPos, true);
        pElevatorX->moveTo(SAxisPosName::OutMagPos, true);
        pElevatorX->moveTo(SAxisPosName::StandbyPos, false);
    }

    SearchForMag();
    if (moveToReadyPosAfterHome)
    {
    }
}

bool Feeding::CheckCarrierBoardIsOnDetaperPlatform()
{
    bool IsUnsafe = false;
    SetSpeedAndAccForThreeAxis(pFeedingConfig->OutCarrierBoardSpeed(), pFeedingConfig->OutCarrierBoardSpeed() * 20);
    pContactRollerR->axis()->relMove(-1000, false);
    pContactRollerL->axis()->relMove(-1000, false);
    QElapsedTimer qTimer;
    qTimer.start();
    do
    {
        if (qTimer.elapsed() > 3000)
        {
            IsUnsafe = false;
            pContactRollerR->axis()->stop();
            pContactRollerL->axis()->stop();
            return IsUnsafe;
        }
    } while (!pCobCarrierSNR->checkState(true));
    pContactRollerR->axis()->stop();
    pContactRollerL->axis()->stop();

    IsUnsafe = true;
    return IsUnsafe;
}

void Feeding::DisableAllDi()
{
    pMagKickerIn->disableAll();       //
    pMagKickerOut->disableAll();      //
    pMagFullSNR->disableAll();        //
    pCobCarrierSNR->disableAll();     //判断载板是否有
    pDtStopperIn->disableAll();       //
    pDtStopperOut->disableAll();      //
    pDtGripperIn->disableAll();       //
    pDtGripperOut->disableAll();      //
    pDtKickerIn->disableAll();        //
    pDtKickerOut->disableAll();       //
    pVacPlatformIn->disableAll();     //
    pVacPlatformOut->disableAll();    //
    pIsExistMag->disableAll();        //判断是否存在弹夹
}

void Feeding::StartAutoRun()
{
    qDebug("StartAutoRun");
    bool nIsUnsafe = CheckCarrierBoardIsOnDetaperPlatform();
    if (nIsUnsafe)
    {
        throw SilicolAbort(tr("Having carrier board on detaper platform,Please handle it manually!"));
    }

    pushEvent(WorkerEvent("TakeMaterialAwayFuntion"));
}

bool Feeding::WaitForInputTrue(SCDI *nDI, long nTimeOut)
{
    bool nWaitInputRes = false;
    QElapsedTimer qTimer;
    qTimer.start();

    while (!nDI->get())
    {
        if (qTimer.elapsed() > nTimeOut)
        {
            nWaitInputRes = false;
            break;
        }
        if (nDI->get())
        {
            nWaitInputRes = true;
            break;
        }
    }
    return nWaitInputRes;
}

bool Feeding::WaitForInputFalse(SCDI *nDI, long nTimeOut)
{
    bool nWaitInputRes = false;
    QElapsedTimer qTimer;
    qTimer.start();

    while (nDI->get())
    {
        if (qTimer.elapsed() > nTimeOut)
        {
            nWaitInputRes = false;
            break;
        }
        if (!nDI->get())
        {
            nWaitInputRes = true;
            break;
        }
    }
    return nWaitInputRes;
}

#define FeedFoldIngStart {

void Feeding::FeedCarrierBoard(int nIndex)    //进载板
{
    qDebug("FeedCarrierBoard");
    // checkIsInit();
    pDtStopper->set(true);
    pDtKicker->set(false);
    pTrayClamp->set(false);
    pMagKicker->set(true);
    SetSpeedAndAccForThreeAxis(pFeedingConfig->FeedCarrierBoardSpeed(), pFeedingConfig->FeedCarrierBoardSpeed() * 20);
    pDetaperZ->moveTo(SAxisPosName::DetaperZ_S, false);

    msleep(1000);

    if (pCobCarrierSNR->get())
    {
        double dSpace = pFeedingConfig->FeedCarrierBoardStep();
        pContactRollerL->axis()->relMove(dSpace, true);
    }
    else if (!pCobCarrierSNR->get())
    {
        pDtStopper->set(false);
        pTrayClamp->set(false);
        pMagKicker->set(false);
        //如果没有感应到载板怎么处理？

        throw SilicolAbort(tr("Never search for carrier board, Feed carrier board failed!"));
        qDebug("Never search for Carrier Board");
        return;
    }

    pTrayClamp->set(true);
    pMagKicker->set(false);
    pDetaperZ->axis()->waitArrivedPos();
    //
}

void Feeding::FeedCarrierBoardTest()
{
    pPreventOverTurnSNR->set(true);
    sleep(5);
    pPreventOverTurnSNR->set(false);
}

FeedingState Feeding::GetFeedingState()    //获取PickArm是否在可下切刀位
{
    return m_FeedingState;
}

void Feeding::AskPickArmMoveToCutPos()
{
    //测试防翻料气缸
    // pPreventOverTurnSNR->set(false);
}

void Feeding::CutCarrierBoard()
{
    pDtGripper->set(false);
    pCollectSNR->set(false);
    pDetaperZ->moveTo(SAxisPosName::DetaperZ_S);
    pPickArmC_Z->moveTo(SAxisPosName::CutCarrierBoardPosZ);
    pDetaperZ->moveTo(SAxisPosName::DetaperZ_ClampBlueFilmPos);
    pDtGripper->set(true);
    pPickArmC_Z->moveTo(SAxisPosName::StandbyPos);
    pCollectSNR->set(true);
}

void Feeding::TakeMaterialAwayWithOffset(double nOffset, bool IsFirstDetaper)    //剥离物料
{
    pPreventOverTurnSNR->set(true);
    pSensorVac->set(true);
    if (IsFirstDetaper)
    {
        pDtStopper->set(false);
        pTrayClamp->set(false);
        SetSpeedAndAccForThreeAxis(pFeedingConfig->DetaperSpeed(), pFeedingConfig->DetaperSpeed() * 20);
        pContactRollerR->axis()->relMove(nOffset, false);
        pContactRollerL->axis()->relMove(nOffset, false);
        pDetaperZ->axis()->relMove(-nOffset);
        pContactRollerR->axis()->stop();
        pContactRollerL->axis()->stop();
    }
    else
    {
        pDtStopper->set(false);
        pTrayClamp->set(false);
        SetSpeedAndAccForThreeAxis(pFeedingConfig->DetaperSpeed(), pFeedingConfig->DetaperSpeed() * 20);
        pContactRollerR->axis()->relMove(nOffset, false);
        pContactRollerL->axis()->relMove(nOffset, false);
        pDetaperZ->axis()->relMove(-nOffset);
        pContactRollerR->axis()->stop();
        pContactRollerL->axis()->stop();
    }
    pPreventOverTurnSNR->set(false);
    pSensorVac->set(false);
}

#define FeedFoldIngEnd }

#define OutCarrierFoldIngStart {

void Feeding::WaitAxisDone(SingleAxis *nAxis)
{
    pContactRollerL->axis()->relMove(-500, false);
    pContactRollerL->axis()->waitArrivedPos();

    pDtKicker->set(true);
    sleep(1);

    pDtKicker->set(false);
}

void Feeding::OutCarrierBoard()
{
    //    double preSpeed = pContactRollerR->axis()->config()->maxVel();
    //    pContactRollerR->axis()->config()->setMaxVel(4);

    pDtStopper->set(false);
    pCollectSNR->set(false);
    pTrayClamp->set(false);
    //添加了设置加速度和速度
    // maxVelocity---maxAcc
    SetSpeedAndAccForThreeAxis(pFeedingConfig->OutCarrierBoardSpeed(), pFeedingConfig->OutCarrierBoardSpeed() * 20);
    pDetaperZ->axis()->absMove(pDetaperZ->getPos<SingleAxisPos>(SAxisPosName::DetaperZ_ClampBlueFilmPos)->pos(), false);

    pContactRollerR->axis()->relMove(-1000, false);
    pContactRollerL->axis()->relMove(-1000, false);

    QElapsedTimer qTimer;
    qTimer.start();
    while (pDetaperZ->axis()->getFeedbackPos(-1) < (pDetaperZ->getPos<SingleAxisPos>(SAxisPosName::DetaperZ_ClampBlueFilmPos)->pos() - 2))
    {
        if (qTimer.elapsed() > 30000)
        {
            throw SilicolAbort("pDetaperZ->axis()->getFeedbackPos(-1) TimeOut!");
        }
    }
    pDtGripper->set(false);
    if (pCobCarrierSNR->get())
    {
        WaitForDiChange(pCobCarrierSNR);
    }
    else
    {
        WaitForDiChange(pCobCarrierSNR);
        WaitForDiChange(pCobCarrierSNR);
    }
    pContactRollerL->axis()->stop();
    pContactRollerR->axis()->stop();
    pDtKicker->set(true);
    pDtKicker->set(false);

    if (pCobCarrierSNR->get())
    {
        qDebug("Out carrier error.");
        //        NuzzleHasMaterial err;
        //        auto option = ErrorHandler::getErrorHandlingOption(err);

        //        if(option == ErrorHandlingOption::UseCurrent_)
        //        {
        //            return;
        //        }
        //        else if(option == ErrorHandlingOption::FinishHandle_)
        //        {
        //            return;
        //        }
        //        else
        //        {
        //            return;
        //        }
    }
}

bool Feeding::WaitForDiChange(SCDI *pDi)
{
    bool OldDi = pDi->get();
    QElapsedTimer qTimer;
    qTimer.start();
    bool IsTimeOut = false;
    while (true)
    {
        if (OldDi == !pDi->get())
        {
            break;
        }
        if (qTimer.elapsed() > 10000)
        {
            IsTimeOut = true;
            break;
        }
    }
    return IsTimeOut;
}

void Feeding::SetSpeedAndAccForThreeAxis(double nSpeed, double nAcc)
{
    pDetaperZ->axis()->setNextMoveVelAcc(nSpeed, nAcc);
    pContactRollerL->axis()->setNextMoveVelAcc(nSpeed, nAcc);
    pContactRollerR->axis()->setNextMoveVelAcc(nSpeed, nAcc);
}

#define OutCarrierFoldIngEnd }

void Feeding::FeedMag()
{
    for (int rowIndex = 0; rowIndex < pFeedTrayConfig->rows(); rowIndex++)
    {
        auto mFeedingMapData = pFeedingMap->getData<FeedingMapData>(rowIndex, 0);

        if (mFeedingMapData->materialState() == FeedingMapData::Init)
        {
            qDebug("MoveToPosWithCol before");
            MoveToPosWithRow(rowIndex, 0);
            if (!pMagGripper->get())
            {
                pMagGripper->set(true);
            }
            FeedCarrierBoard(0);
            mFeedingMapData->setMaterialState(FeedingMapData::Ok);
            return;
        }
    }

    pFeedingMap->reset(FeedingMapData::Init);
    pushEvent(WorkerEvent("ChangeMag"));
}

void Feeding::ChangeMag()
{
    pElevatorZ->moveTo(SAxisPosName::OutMagPos, true);
    pElevatorX->moveTo(SAxisPosName::OutMagPos, true);
    pElevatorX->moveTo(SAxisPosName::StandbyPos, false);
    pElevatorZ->moveTo(SAxisPosName::SecondMagStandbyPos, true);
    if (pIsExistMag->get())
    {
        if (!pMagGripper->get())
        {
            pMagGripper->set(true, true);
        }
        qDebug("Change magazine Ok.");
    }
    else
    {
        qDebug("Load magazine fail.");
    }
}

void Feeding::MoveToPosWithRow(int rowIndex, int columnIndex)
{
    qDebug("MoveToPosWithRow");
    int unitRowIndex = rowIndex / pFeedTrayConfig->rowsInsideUnit();
    int rowIndexInsideUnit = rowIndex % pFeedTrayConfig->rowsInsideUnit();
    int unitColumnIndex = columnIndex / pFeedTrayConfig->columnsInsideUnit();
    int columnIndexInsideUnit = columnIndex % pFeedTrayConfig->columnsInsideUnit();
    MoveToPosWithRow(unitRowIndex, unitColumnIndex, rowIndexInsideUnit, columnIndexInsideUnit);
}

void Feeding::MoveToPosWithRow(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex)
{
    qDebug("ElevatorZ move");
    auto pPos = pElevatorZ->getPos<SingleAxisPos>(SAxisPosName::FirstCarierBoardPos);
    double zDelta = unitRowIndex * pFeedTrayConfig->unitRowDelta() + rowIndex * pFeedTrayConfig->rowDelta();
    pElevatorZ->axis()->absMove(pPos->pos() - zDelta, true);
}

void Feeding::FeedingFuntion()
{
    qDebug("FeedingFuntion");
}

void Feeding::SearchForMag()
{
    if (!pIsExistMag->get())
    {
        // pElevatorZ->axis()->jogMove(1, 10);
        pElevatorZ->moveTo(SAxisPosName::SecondMagStandbyPos, false);
    }
    QElapsedTimer qTimer;
    qTimer.start();
    bool IsTimeOut = false;
    do
    {
        if (qTimer.elapsed() > 10000)
        {
            IsTimeOut = true;
            qDebug("pIsExistMag->get() TimeOut");
            break;
        }

        if (pIsExistMag->get())
        {
            break;
        }
        msleep(2);
    } while (!pIsExistMag->get());

    if (IsTimeOut)
    {
        pElevatorZ->axis()->stop();
        pElevatorZ->moveTo(SAxisPosName::FirstCarierBoardPos);
        pMagGripper->set(true);
        qDebug("Never find Mag and Go to StandbyPos");
        return;
    }
    else
    {
        pElevatorZ->axis()->stop();
        sleep(1);
        pElevatorZ->axis()->relMove(pFeedingConfig->SearchForMagOffset);
        pMagGripper->set(true);
    }
}

void Feeding::MoveToFeedCarrierBoardPos()
{
    checkIsInit();
    if (hasAbortEvent())
    {
        return;
    }

    pElevatorZ->moveTo(SAxisPosName::ChangeBoardPos);
    // QThread::msleep(50);
}

void Feeding::MoveToTakeMaterialPos()
{
    pDetaperZ->moveTo(SAxisPosName::FirstCarierBoardPos);
}

void Feeding::BeginStrippingMaterial()
{
    if (pDtGripper->get())
    {
        pDtGripper->set(false);
    }
    pDetaperZ->moveTo(SAxisPosName::FirstCarierBoardPos);
    pDtGripper->set(true);
    for (int columnIndex = 0; columnIndex < pFeedTrayConfig->columns(); columnIndex++)
    {
        for (int rowIndex = 0; rowIndex < pFeedTrayConfig->rows(); rowIndex++)
        {
            auto mFeedingMapData = pFeedingMap->getData<FeedingMapData>(0, columnIndex);
            if (mFeedingMapData->materialState() == FeedingMapData::Init)
            {
                MoveToChoseClunm(columnIndex, true);
            }
        }
    }
}

void Feeding::MoveToChoseClunm(int Column, bool IsFirstMag)
{

    if (IsFirstMag)
    {
        SingleAxisPos *pPos = pDetaperZ->getPos<SingleAxisPos>(SAxisPosName::FirstCarierBoardPos);
        double PosZ = pPos->pos() + Column * pFeedTrayConfig->columnsInsideUnit();
        pDetaperZ->axis()->absMove(PosZ);

        auto mFeedingMapData = pFeedingMap->getData<FeedingMapData>(0, Column);
        mFeedingMapData->setMaterialState(FeedingMapData::Ok);
    }
    else
    {
        if (pFeedingConfig->mMagCloumn == pFeedTrayConfig->columns())
        {
            //表示已经做完，需要出料
        }
        SingleAxisPos *pPos = pDetaperZ->getPos<SingleAxisPos>(SAxisPosName::SecondMagStandbyPos);
        double PosZ = pPos->pos() + Column * pFeedTrayConfig->columnsInsideUnit();
        pDetaperZ->axis()->absMove(PosZ);

        auto mFeedingMapData = pFeedingMap->getData<FeedingMapData>(0, Column);
        mFeedingMapData->setMaterialState(FeedingMapData::Ok);
    }
}

QPointF Feeding::CalculatePosFromColumn(int nColumn)
{
    QPointF PosTemp;
    double PosX;
    SingleAxisPos *pPos = pDetaperZ->getPos<SingleAxisPos>(SAxisPosName::FirstCarierBoardPos);
    PosX = pPos->pos() + nColumn * pFeedTrayConfig->columnsInsideUnit();

    PosTemp.setX(PosX);
    PosTemp.setY(0);

    return PosTemp;
}

//设置主从轴绑定，注意运行完之后要单独运动需要解绑
//    pContactRollerL->axis()->setMasterAxis(pDetaperZ->axis());
//    pContactRollerR->axis()->setMasterAxis(pDetaperZ->axis());
//    pContactRollerL->axis()->bindToMasterAxis();
//    pContactRollerR->axis()->bindToMasterAxis();
//    pContactRollerL->axis()->unBindToMasterAxis();
//    pContactRollerR->axis()->unBindToMasterAxis();
