#include "discharge.h"

Discharge::Discharge(TrayConfig *nDischargeTrayConfig, DischargeMap *nDischargeMap, DischargeConfig *nDischargeConfig, QObject *parent)
           :Worker(WorkerName::Discharge, parent),
            pDischargeTrayConfig(nDischargeTrayConfig),
            pDischargeMap(nDischargeMap),
            pDischargeConfig(nDischargeConfig)
{
    pDischargeConfigFile = new ConfigFile("DischargeConfig", pDischargeConfig,
                                        QString("%1/DischargeConfig.json").arg(Worker::getWorkerConfigDir()));
    pDischargeConfigFile->populate(true);
}

void Discharge::initEventHandle()
{
    //出料
    pTrackX = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::TrackX);//盤子在軌道上移動
    pTrackZ = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::TrackZ);//轨道和夹子整体升降

    //DI
    pTraySNR = MotionElementContainer::getIns()->getItem<SCDI>(DiName::TraySNR);//检测客户料盒有无Tray盘

    //DO
    pTrackStop_Z = MotionElementContainer::getIns()->getItem<SCDO>(DoName::TrackStop_Z);//电机刹车（出料轨道上下电机）
    pTrayGripper = MotionElementContainer::getIns()->getItem<SCDO>(DoName::TrayGripper);
}

void Discharge::startEventHandle(const WorkerEvent &event)
{

}

void Discharge::stopEventHandle()
{

}

void Discharge::allWorkerStopped()
{

}

void Discharge::purgeEventHandle()
{
      //purgeEventHandled();
}

void Discharge::resetEventHandle()
{

}

void Discharge::homeWorker(bool moveToReadyPosAfterHome)
{
    pTrackX->axis()->home(false);

    if(pTrackStop_Z->get())
    {
        pTrackStop_Z->set(false);
    }
    qDebug("TrackZ go home.");
    pTrackZ->axis()->home(true);//注意此轴有刹车的。
    pTrackX->axis()->waitHomeDone();
    qDebug("TrackZ go home finish.");
}

void Discharge::StartAutoRun()
{

}

void Discharge::DischargeFuntion()
{

}

void Discharge::GetTray(int nIndex)
{
    pTrayGripper->set(false);
    pTrackZ->moveTo(SAxisPosName::GetTrayPos);
    pTrackX->moveTo(SAxisPosName::GetTrayPos);
    msleep(2000);
    pTrayGripper->set(true);
    msleep(1000);
//    auto *Pos = pTrackX->getPos<SingleAxisPos>(SAxisPosName::p);
//    double TempPos = Pos->pos() + 10;
//    pTrackX->axis()->absMove(TempPos);
    pTrackX->moveTo(SAxisPosName::TakePicturePosForPlace);
    //pTrackX->axis()->slowMoveTo(TempPos);//减速回移，防止回弹
    //pTrayGripper->set(false);
    //pTrackX->moveTo(SAxisPosName::PlaceTraySafePos);//减速回移，防止回弹
}

void Discharge::OutTray(int nIndex)
{
    pTrackZ->moveTo(SAxisPosName::OutTrayPos);
    pTrackX->moveTo(SAxisPosName::OutTrayPos);

    pTrackX->moveTo(SAxisPosName::GetTraySafePos);
}

void Discharge::ElevatorMove(bool IsMoveUp)
{

}

void Discharge::GetTrayFomElevatorFloorNum(int nFloorNum)
{

}

void Discharge::ClampTray()
{
    pTrayGripper->set(true);
}

void Discharge::ReleaseTay()
{
    pTrayGripper->set(false);
}

void Discharge::MoveToGetTrayPos()
{
    pTrackZ->moveTo(SAxisPosName::GetTrayPos);
    pTrackX->moveTo(SAxisPosName::GetTrayPos);
}

void Discharge::MoveToOutTrayPos()
{
    pTrackZ->moveTo(SAxisPosName::OutTrayPos);
    pTrackX->moveTo(SAxisPosName::OutTrayPos);
}

void Discharge::ResetGotDut()
{
    m_gotDut = false;
}

bool Discharge::GotDut()
{
    return  true;
}

QPoint Discharge::LastDutIndex() const
{
    return m_lastDutIndex;
}

const PrOffset &Discharge::LastDutPrOffset()
{
    return m_lastDutOffset;
}

bool Discharge::IsNeedPAX_Move()
{
    return true;
}

DischargeMapOffset Discharge::TrayMoveToPlaceMaterialPos(double CenterOffset)
{
    //如果移动前判断到轴位置不足以移动，则返回false给PickArm模块，然后使用轴X移动补充。
    DischargeMapOffset nDischargeMapOffset = GetDischargeOffset();
    if(!nDischargeMapOffset.IsMovePaX)
    {
        SingleAxisPos *pPos = pTrackX->getPos<SingleAxisPos>(SAxisPosName::TakePicturePosForPlace);
        double xDelta = pPos->pos() - nDischargeMapOffset.OffsetX - CenterOffset;
        pTrackX->axis()->absMove(xDelta);

    }
    else
    {
        return nDischargeMapOffset;
    }

    return nDischargeMapOffset;
}

bool Discharge::GetDischargeState()
{
    return  m_IsReadyToPlaceMaterial;
}

QPointF Discharge::CalculateOutTrayOffset(QPointF nPrOffset, double nPrTheta, QPointF TrayRowColumnOffset)
{
    QPointF nResultOffset;
    nResultOffset.setX(-nPrOffset.x() + TrayRowColumnOffset.x()*cos(nPrTheta*M_PI/180));
    nResultOffset.setY(-nPrOffset.y()+TrayRowColumnOffset.x()*sin(nPrTheta*M_PI/180));
    return  nResultOffset;
}

QPointF Discharge::CalculateOutTrayOffset(double nPrTheta, QPointF TrayRowColumnOffset)
{
    QPointF nResultOffset;
    nResultOffset.setX(TrayRowColumnOffset.x()*cos(nPrTheta*M_PI/180));
    nResultOffset.setY(TrayRowColumnOffset.x()*sin(nPrTheta*M_PI/180));
    return  nResultOffset;
}

DischargeMapInitIndex Discharge::GetPickArmMapInitIndex()
{
    DischargeMapInitIndex nDischargeMapInitIndex;
    for (int rowIndex = 0; rowIndex < pDischargeTrayConfig->rows(); rowIndex++)
    {
        for (int columnIndex = 0; columnIndex < pDischargeTrayConfig->columns(); columnIndex++)
        {
            DischargeMapData *mDischargeMapData = pDischargeMap->getData<DischargeMapData>(rowIndex, columnIndex);
            nDischargeMapInitIndex.pDischargeMapData = mDischargeMapData;
            //m_DischargeMapInitIndex.pDischargeMapData = mDischargeMapData;
            if (mDischargeMapData->materialState() == DischargeMapData::Init)
            {
                nDischargeMapInitIndex.RowIndex = rowIndex;
                nDischargeMapInitIndex.ColumnIndex = columnIndex;
                //m_PickArmMapInitIndex = nPickArmMapInitIndex;
                return nDischargeMapInitIndex;
            }
        }
    }
    return nDischargeMapInitIndex;
}

DischargeMapOffset Discharge::GetDischargeOffset()
{
    DischargeMapOffset nDischargeMapOffset;
    DischargeMapInitIndex nDischargeMapInitIndex;
    nDischargeMapInitIndex = GetPickArmMapInitIndex();
    nDischargeMapOffset.pDischargeMapData = nDischargeMapInitIndex.pDischargeMapData;

    int unitRowIndex = nDischargeMapInitIndex.RowIndex / pDischargeTrayConfig->rowsInsideUnit();
    int rowIndexInsideUnit = nDischargeMapInitIndex.RowIndex % pDischargeTrayConfig->rowsInsideUnit();
    int unitColumnIndex = nDischargeMapInitIndex.ColumnIndex / pDischargeTrayConfig->columnsInsideUnit();
    int columnIndexInsideUnit = nDischargeMapInitIndex.ColumnIndex % pDischargeTrayConfig->columnsInsideUnit();

    qDebug()<<"rowIndexInsideUnit:"<<rowIndexInsideUnit<<"JudgeIndex:"<<pDischargeConfig->JudgeIndex();
    if(rowIndexInsideUnit>pDischargeConfig->JudgeIndex())
    {
        nDischargeMapOffset.IsMovePaX = true;
        rowIndexInsideUnit = rowIndexInsideUnit - pDischargeConfig->JudgeIndex();
    }
    else
    {
        nDischargeMapOffset.IsMovePaX = false;
    }
    double xDelta = unitRowIndex * pDischargeTrayConfig->unitRowDelta() + rowIndexInsideUnit * pDischargeTrayConfig->rowDelta();
    double yDelta = unitColumnIndex * pDischargeTrayConfig->unitColumnDelta() + columnIndexInsideUnit * pDischargeTrayConfig->columnDelta();

    nDischargeMapOffset.OffsetX = xDelta;
    nDischargeMapOffset.OffsetY = yDelta;

    return nDischargeMapOffset;
}

void Discharge::MoveToPlaceMaterialTrackZ()
{
    pTrackZ->moveTo(SAxisPosName::PlaceMaterialPos);
}
