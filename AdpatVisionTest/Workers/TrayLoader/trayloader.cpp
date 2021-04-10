#include "trayloader.h"

TrayLoader::TrayLoader(TrayConfig *trayConfig, TrayMap *trayMap, TrayLoaderConfig *trayLoaderConfig, QObject *parent)
    : Worker(WorkerName::TrayLoader, parent), trayConfig(trayConfig), trayMap(trayMap), trayLoaderConfig(trayLoaderConfig)
{
}

void TrayLoader::resetGotDut()
{
    m_gotDut = false;
}

const PrOffset &TrayLoader::lastDutPrOffset()
{
    return m_lastDutOffset;
}

bool TrayLoader::gotDut() const
{
    return m_gotDut;
}

QPoint TrayLoader::lastDutIndex() const
{
    return m_lastDutIndex;
}

void TrayLoader::getDut()
{
    checkIsInit();

    for (int columnIndex = 0; columnIndex < trayConfig->columns(); columnIndex++)
    {
        for (int rowIndex = 0; rowIndex < trayConfig->rows(); rowIndex++)
        {
            auto dutData = trayMap->getData<DutData>(rowIndex, columnIndex);
            if (dutData->materialState() == DutData::PrOk)
            {
                m_lastDutIndex = QPoint(rowIndex, columnIndex);
                m_gotDut = true;
                return;
            }
            if (dutData->materialState() == DutData::Init)
            {
                moveToPrPosImpl(rowIndex, columnIndex);
                PrOffset prOffset;

                bool prResult = dutDownlookLocation->performPR(prOffset);

                // ---For Test---
                prResult = (qrand() % 4 != 2);
                QThread::msleep(qrand() % 50 + 50);
                // ---For Test---

                if (prResult)
                {
                    m_nContinuallyPrFailed = 0;
                    m_lastDutOffset = prOffset;
                    m_lastDutIndex = QPoint(rowIndex, columnIndex);
                    dutData->setMaterialState(DutData::PrOk);
                    m_gotDut = true;
                    return;
                }
                else
                {
                    m_nContinuallyPrFailed++;
                    dutData->setMaterialState(DutData::PrNg);
                    if (trayLoaderConfig->nChangeTrayAfterContinuallyPrFailed() > 0
                        && m_nContinuallyPrFailed >= trayLoaderConfig->nChangeTrayAfterContinuallyPrFailed())
                    {
                        pushEvent(WorkerEvent("changeTray"));
                        pushEvent(WorkerEvent("getDut"));
                        return;
                    }
                }
            }
        }
    }

    // have to change tray first
    trayMap->grabMapToImage(QString("./trayMapResult/%1.jpg").arg(QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_zzz")));
    pushEvent(WorkerEvent("changeTray"));
    pushEvent(WorkerEvent("getDut"));
}

void TrayLoader::changeTray()
{
    checkIsInit();

    tlXy->moveTo(XYPosName::ChangeTrayPos);

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    // ---For Test---

    tlTrayClamp->set(false);
    while (true)
    {
        if (hasAbortOrAutoStopEvent())
        {
            return;
        }
        if (!UIOperation::getIns()->okCancelConfirm(tr("Please change tray!")))
        {
            throw SilicolAbort(tr("User cancel changing tray!"));
        }
        if (!tlTraySensor->checkState(true))
        {
            UIOperation::getIns()->showError(tr("Did not detect tray!"));
            continue;
        }
        break;
    }
    tlTrayClamp->set(true);
    trayMap->reset(DutData::Init);
}

void TrayLoader::moveToPrPos(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex)
{
    checkIsInit();

    trayConfig->getValidUiIndex(unitRowIndex, unitColumnIndex, rowIndex, columnIndex);
    moveToPrPosImpl(unitRowIndex, unitColumnIndex, rowIndex, columnIndex);
}

void TrayLoader::prTest()
{
    QImage img;
    img.load("C:\\Users\\Silicool\\Desktop\\Test\\test.jpg");
    auto image = img.convertToFormat(QImage::Format_Indexed8);
    PrOffset prOffset;
    dutDownlookLocation->performPR(image, prOffset);
}

void TrayLoader::initEventHandle()
{
    tlXy = MotionElementContainer::getIns()->getItem<XYModule>(XYModuleName::TLXY);
    tlTrayClamp = MotionElementContainer::getIns()->getItem<SCCylinder>(CylName::TLTrayClamp);
    tlTraySensor = MotionElementContainer::getIns()->getItem<SCDI>(DiName::TLTraySNR);
    tlDemoDO = MotionElementContainer::getIns()->getItem<SCDO>(DoName::DemoDO);

    dutDownlookLocation = VisionManager::getIns().getVisionLocation(VLName::DutDownlookLocation);
}

void TrayLoader::startEventHandle(const WorkerEvent &event)
{
    auto runParam = event.eventArgs.toMap();
    bool isDryRun = (runParam["RunMode"] == "DryRun");
    if (isDryRun)
    {
        // Do something...
    }

    m_nContinuallyPrFailed = 0;
    verifyTrayMap();

    // ---For Test---
    qsrand(time(0));
    dutDownlookLocation->disable();
    // ---For Test---
}

void TrayLoader::resetEventHandle()
{
    // Do something...
    m_nContinuallyPrFailed = 0;
}

void TrayLoader::homeWorker(bool moveToReadyPosAfterHome)
{
    tlXy->xAxis()->home(false);
    tlXy->yAxis()->home();
    tlXy->xAxis()->waitHomeDone();
    if (moveToReadyPosAfterHome)
    {
        tlXy->moveTo(XYPosName::StandbyPos);
    }
}

void TrayLoader::moveToPrPosImpl(int rowIndex, int columnIndex)
{
    int unitRowIndex = rowIndex / trayConfig->rowsInsideUnit();
    int rowIndexInsideUnit = rowIndex % trayConfig->rowsInsideUnit();
    int unitColumnIndex = columnIndex / trayConfig->columnsInsideUnit();
    int columnIndexInsideUnit = columnIndex % trayConfig->columnsInsideUnit();
    moveToPrPosImpl(unitRowIndex, unitColumnIndex, rowIndexInsideUnit, columnIndexInsideUnit);
}

void TrayLoader::moveToPrPosImpl(int unitRowIndex, int unitColumnIndex, int rowIndex, int columnIndex)
{
    auto prLTPos = tlXy->getPos<XYModulePos>(XYPosName::PrLeftTop);
    double xDelta = unitColumnIndex * trayConfig->unitColumnDelta() + columnIndex * trayConfig->columnDelta();
    double yDelta = unitRowIndex * trayConfig->unitRowDelta() + rowIndex * trayConfig->rowDelta();
    tlXy->setNextPosVelAcc(XYPosName::PrLeftTop);
    tlXy->moveTo(QPointF(prLTPos->xPos() + xDelta, prLTPos->yPos() + yDelta), true, 0.001);
}

void TrayLoader::verifyTrayMap()
{
    for (int columnIndex = 0; columnIndex < trayConfig->columns(); columnIndex++)
    {
        for (int rowIndex = 0; rowIndex < trayConfig->rows(); rowIndex++)
        {
            auto dutData = trayMap->getData<DutData>(rowIndex, columnIndex);
            if (dutData->materialState() == DutData::PrOk)
            {
                // PrOK的物料可能丢失PrOffset数据
                dutData->setMaterialState(DutData::Init);
            }
        }
    }
}
