#include "gtdo.h"

QMutex GTDO::locker;

GTDO::GTDO(QString name, QObject *parent) : SCDO(name, parent) {}

GTIOConfig *GTDO::getGtioConfig() const
{
    return gtioConfig;
}

bool GTDO::getImpl() noexcept
{
    if (gtioConfig->ioType() == GTIOConfig::AxisIO)
    {
        char value = 0;
        short res = GTN_GetEcatAxisDOBit(gtioConfig->coreNo(), gtioConfig->axis(), gtioConfig->index() - 1, &value);
        printError(res, QString("%1 get axis do state failed!").arg(name()));
        return value != 0;
    }
    else
    {
        ushort value = 0;
        short res = GTN_EcatIOReadInput(gtioConfig->coreNo(), gtioConfig->slave(), gtioConfig->offset(), 2, reinterpret_cast<uchar *>(&value));
        printError(res, QString("%1 get ethercat do state failed!").arg(name()));
        return (value & (1 << (gtioConfig->index() - 1))) != 0;
    }
}

void GTDO::setImpl(bool state) noexcept
{
    if (gtioConfig->ioType() == GTIOConfig::AxisIO)
    {
        short res = GTN_SetEcatAxisDOBit(gtioConfig->coreNo(), gtioConfig->axis(), gtioConfig->index() - 1, state);
        printError(res, QString("%1 set axis do state failed!").arg(name()));
    }
    else
    {
        short res = GTN_EcatIOBitWriteOutput(gtioConfig->coreNo(), gtioConfig->slave(), gtioConfig->offset(), gtioConfig->index(), state);
        printError(res, QString("%1 set ethercat do state failed!").arg(name()));
    }
}

void GTDO::initImpl()
{
    gtioConfig = qobject_cast<GTIOConfig *>(config());
    if (gtioConfig == nullptr)
    {
        throw SilicolAbort(QString("Can not cast IOConfig to GTIOConfig! IOConfig type: %1").arg(config()->metaObject()->className()), EX_LOCATION);
    }
}
