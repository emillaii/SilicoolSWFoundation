#include "gtdi.h"

GTDI::GTDI(QString name, QObject *parent) : SCDI(name, parent) {}

bool GTDI::getImpl() noexcept
{
    short value = 0;
    short res = GTN_GetDiBit(gtioConfig->coreNo(), MC_GPI, gtioConfig->index(), &value);
    printError(res, QString("%1 get di state failed!").arg(name()));
    return value;
}

void GTDI::initImpl()
{
    gtioConfig = qobject_cast<GTIOConfig *>(config());
    if (gtioConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast IOConfig to GTIOConfig! IOConfig type: %1").arg(config()->metaObject()->className()), EX_LOCATION);
    }
}
