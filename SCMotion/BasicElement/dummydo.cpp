#include "dummydo.h"

DummyDO::DummyDO(QString name, QObject *parent) : SCDO(name, parent) {}

bool DummyDO::getImpl() noexcept
{
    return currentValue;
}

void DummyDO::setImpl(bool state) noexcept
{
    currentValue = state;
}
