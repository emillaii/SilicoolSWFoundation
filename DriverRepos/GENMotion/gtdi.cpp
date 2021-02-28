#include "gtdi.h"

GTDI::GTDI(QString name, QObject *parent) : SCDI(name, parent) {}

bool GTDI::getImpl() noexcept
{
    if (gtioConfig->ioType() == GTIOConfig::AxisIO)
    {
        ulong value = 0;
        short res = GTN_GetEcatAxisDI(gtioConfig->coreNo(), gtioConfig->axis(), &value);
        printError(res, QString("%1 get axis di state failed!").arg(name()));
        return (value & (1 << (gtioConfig->index() - 1))) != 0;
    }
    else
    {
        ushort value = 0;
        short res = GTN_EcatIOReadInput(gtioConfig->coreNo(), gtioConfig->slave(), gtioConfig->offset(), 2,
                                        reinterpret_cast<uchar *>(&value));
        printError(res, QString("%1 get ethercat di state failed!").arg(name()));
        return (value & (1 << (gtioConfig->index() - 1))) != 0;
    }
}

void GTDI::initImpl()
{
    gtioConfig = qobject_cast<GTIOConfig *>(config());
    if (gtioConfig == nullptr)
    {
        throw SilicolAbort(
            QString("Can not cast IOConfig to GTIOConfig! IOConfig type: %1").arg(config()->metaObject()->className()),
            EX_LOCATION);
    }
}
