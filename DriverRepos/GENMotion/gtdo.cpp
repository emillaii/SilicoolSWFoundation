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
        ulong value = 0;
        short res = GTN_GetEcatAxisDO(gtioConfig->coreNo(), gtioConfig->axis(), &value);
        printError(res, QString("%1 get axis do state failed!").arg(name()));
        return (value & (1 << (gtioConfig->index() - 1))) != 0;
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
        QMutexLocker tmpLocker(&locker);
        ulong value = 0;
        short res = GTN_GetEcatAxisDO(gtioConfig->coreNo(), gtioConfig->axis(), &value);
        printError(res, QString("%1 get axis do state failed!").arg(name()));
        if (res != CMD_SUCCESS)
        {
            return;
        }
        if (state)
        {
            value |= (1 << (gtioConfig->index() - 1));
        }
        else
        {
            value &= (~(1 << (gtioConfig->index() - 1)));
        }
        res = GTN_SetEcatAxisDO(gtioConfig->coreNo(), gtioConfig->axis(), value);
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
