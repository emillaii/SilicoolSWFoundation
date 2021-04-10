#include "pickarm.h"

SILICOOL_CREATE_LOGGING_CATEGORY(paTimeCost, "paTimeCost")

PickArm::PickArm(DutTrayMap *trayMap, TrayLoader *tl, UPHHelper *uphHelper, QObject *parent)
    : Worker(WorkerName::PickArm, parent), trayMap(trayMap), tl(tl), uphHelper(uphHelper)
{
}

void PickArm::waitDut()
{
    SCTimer t(__func__, paTimeCost());

    tl->resetGotDut();
    tl->pushEvent(WorkerEvent("getDut"));

    QElapsedTimer timer;
    timer.start();
    while (true)
    {
        if (tl->gotDut())
        {
            break;
        }
        if (hasAbortOrAutoStopEvent())
        {
            return;
        }
        QThread::msleep(5);
        if (hasAbortOrAutoStopEvent())
        {
            return;
        }
        if (timer.elapsed() > 3000)
        {
            timer.restart();
            qCWarning(logCate()) << tr("Waiting dut...");
        }
    }

    pushEvent(WorkerEvent("moveFromStandbyToPickPos"));

    qDebug() << "push pick dut";
    pushEvent(WorkerEvent("pickDut"));
}

void PickArm::moveFromStandbyToPickPos()
{
    SCTimer t(__func__, paTimeCost());

    QPointF paToCameraOffset;
    paXyz->moveXY(paToCameraOffset.x() + tl->lastDutPrOffset().X, paToCameraOffset.y() + tl->lastDutPrOffset().Y);

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    // ---For Test---
}

void PickArm::pickDut()
{
    SCTimer t(__func__, paTimeCost());

    paZ->moveTo(SAxisPosName::PickPos);
    paVacuum->set(true, false);
    paVacuum->waitUntilTimeout(true);
    paZ->moveTo(SAxisPosName::StandbyPos);
    currentDutIndex = tl->lastDutIndex();

    bool vacuumFeedback = paVacuum->get();

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    vacuumFeedback = (qrand() % 10 != 2);
    // ---For Test---

    if (!vacuumFeedback)
    {
        trayMap->getData<DutData>(currentDutIndex.x(), currentDutIndex.y())->setMaterialState(DutData::PickFailed);

        moveToStandbyPos();
        pushEvent(WorkerEvent("waitDut"));
        return;
    }
    trayMap->getData<DutData>(currentDutIndex.x(), currentDutIndex.y())->setMaterialState(DutData::InUse);
    pushEvent(WorkerEvent("placeDut"));
}

void PickArm::placeDut()
{
    SCTimer t(__func__, paTimeCost());

    paXyz->moveToPos(XYZPosName::PlacePos, MoveSequence::XY_Z, false);
    auto standbyPos = paXyz->getPos<XYZModulePos>(XYZPosName::StandbyPos);
    paXyz->yAxis()->waitGreaterThanPos(standbyPos->yPos(), 1);

    bool vacuumFeedback = paVacuum->get();

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    vacuumFeedback = (qrand() % 10 != 2);
    // ---For Test---

    if (!vacuumFeedback)
    {
        paXyz->yAxis()->stop();
        paXyz->xAxis()->stop();
        trayMap->getData<DutData>(currentDutIndex.x(), currentDutIndex.y())->setMaterialState(DutData::Lost);
        moveToStandbyPos();
        pushEvent(WorkerEvent("waitDut"));
        return;
    }
    else
    {
        tl->resetGotDut();
        tl->pushEvent(WorkerEvent("getDut"));
    }
    paXyz->waitArrivedPos(XYZPosName::PlacePos);

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    // ---For Test---

    paVacuum->set(false);
    uphHelper->appendData();

    if (hasAbortOrAutoStopEvent())
    {
        return;
    }

    paXyz->moveToPos(XYZPosName::StandbyPos, MoveSequence::Z_XY, false);

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    // ---For Test---

    while (true)
    {
        if (tl->gotDut())
        {
            QPointF paToCameraOffset;
            double targetX = paToCameraOffset.x() + tl->lastDutPrOffset().X;
            double targetY = paToCameraOffset.y() + tl->lastDutPrOffset().Y;
            paXyz->xAxis()->absForceMove(targetX);
            paXyz->yAxis()->absForceMove(targetY);
            paXyz->xAxis()->waitArrivedPos();
            paXyz->yAxis()->waitArrivedPos();

            qDebug() << "push pick dut";
            pushEvent(WorkerEvent("pickDut"));
            return;
        }
        if (hasAbortOrAutoStopEvent())
        {
            return;
        }
        QThread::msleep(5);
    }
}

void PickArm::moveToStandbyPos()
{
    SCTimer t(__func__, paTimeCost());

    paXyz->moveToPos(XYZPosName::StandbyPos, MoveSequence::Z_XY);

    // ---For Test---
    QThread::msleep(qrand() % 50 + 50);
    // ---For Test---
}

void PickArm::initEventHandle()
{
    paXyz = MotionElementContainer::getIns()->getItem<XYZModule>(XYZModuleName::PAXYZ);
    paZ = MotionElementContainer::getIns()->getItem<SingleAxis>(SAxisModuleName::PAZ);
    paVacuum = MotionElementContainer::getIns()->getItem<SCVacuum>(VacuumName::PAVacuum);
}

void PickArm::startEventHandle(const WorkerEvent &event)
{
    Q_UNUSED(event)

    pushEvent(WorkerEvent("moveToStandbyPos"));
    pushEvent(WorkerEvent("waitDut"));
}

void PickArm::resetEventHandle()
{
    // Do something...
}

void PickArm::homeWorker(bool moveToReadyPosAfterHome)
{
    paXyz->xAxis()->home(false);
    paXyz->yAxis()->home();
    paXyz->xAxis()->waitHomeDone();
    if (moveToReadyPosAfterHome)
    {
        paXyz->moveToPos(XYZPosName::StandbyPos, MoveSequence::Z_XY);
    }
}
