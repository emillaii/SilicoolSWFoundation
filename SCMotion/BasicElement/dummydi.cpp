#include "dummydi.h"

DummyDI::DummyDI(QString name, QObject *parent) : SCDI(name, parent) {}

bool DummyDI::getImpl() noexcept
{
    return dummyIOConfig->dummyValue();
}

void DummyDI::initImpl()
{
    dummyIOConfig = qobject_cast<DummyIOConfig *>(config());
    if (dummyIOConfig == nullptr)
    {
        throw SilicolAbort(
            tr("Can not cast config to DummyIOConfig! Source config type: %1").arg(config()->metaObject()->className()));
    }
}
