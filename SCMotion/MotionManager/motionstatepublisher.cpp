#include "motionstatepublisher.h"

MotionStatePublisher::MotionStatePublisher(const QList<QString> &definedCylinders,
                                           const QList<QString> &definedVacuums,
                                           const QList<QString> &definedDis,
                                           const QList<QString> &definedDos,
                                           QObject *parent)
    : QThread(parent), definedCylinders(definedCylinders), definedVacuums(definedVacuums), definedDis(definedDis), definedDos(definedDos)
{
    connect(&MotionManager::getIns(), &MotionManager::inited, this, [this] {
        if (subscribedVacuums.count() > 0 || subscribedCylinders.count() > 0 || subscribedDis.count() > 0 || subscribedDos.count() > 0)
        {
            isRun = true;
            start();
        }
    });
}

MotionStatePublisher::~MotionStatePublisher()
{
    isRun = false;
    wait();
}

void MotionStatePublisher::subscribeCylinderState(QString cylName)
{
    SUB_ELEMENT(cylName, "Cylinder", definedCylinders, subscribedCylinders)
}

void MotionStatePublisher::subscribeVacuumState(QString vacuumName)
{
    SUB_ELEMENT(vacuumName, "Vacuum", definedVacuums, subscribedVacuums)
}

void MotionStatePublisher::subscribeDIState(QString diName)
{
    SUB_ELEMENT(diName, "DI", definedDis, subscribedDis)
}

void MotionStatePublisher::subscribeDOState(QString doName)
{
    SUB_ELEMENT(doName, "DO", definedDos, subscribedDos)
}

void MotionStatePublisher::run()
{
#define PUB_ELEMENT_STATE(elementNamesContainer, elementType, sig)                                                                                   \
    if (elementNamesContainer.count() > 0)                                                                                                           \
    {                                                                                                                                                \
        QVariantMap states;                                                                                                                          \
        foreach (auto name, elementNamesContainer)                                                                                                   \
        {                                                                                                                                            \
            states[name] = MotionManager::getIns().runInstruction(elementType, name, "get");                                                         \
        }                                                                                                                                            \
        emit sig(states);                                                                                                                            \
    }

    while (isRun)
    {
        if (subscribedCylinders.count() > 0)
        {
            QVariantMap cylStates;
            foreach (auto cylName, subscribedCylinders)
            {
                cylStates[cylName] = MotionManager::getIns().getCylinderCurrentState(cylName);
            }
            emit cylStatesPublished(cylStates);
        }
        PUB_ELEMENT_STATE(subscribedVacuums, MotionElement::Vacuum, vacuumStatePublished);
        PUB_ELEMENT_STATE(subscribedDis, MotionElement::DI, diStatePublished);
        PUB_ELEMENT_STATE(subscribedDos, MotionElement::DO, doStatePublished);

        for (int i = 0; i < 10; i++)
        {
            QThread::msleep(20);
            if (!isRun)
            {
                return;
            }
        }
    }
}
