#include "gtdo.h"

GTDO::GTDO(QString name, QObject *parent) : SCDO(name, parent) {}

GTIOConfig *GTDO::getGtioConfig() const
{
    return gtioConfig;
}

bool GTDO::getImpl() noexcept
{
    int ioIndex = gtioConfig->index() - 1;
    if (ioIndex < 32)
    {
        long value = 0;
        short res = GTN_GetDo(gtioConfig->coreNo(), MC_GPO, &value);
        printError(res, QString("%1 get do state failed!").arg(name()));
        return (value & (1 << ioIndex)) != 0;
    }
    else
    {
        int count = ioIndex / 32 + 1;
        long *values = new long[count];
        short res = GTN_GetDoEx(gtioConfig->coreNo(), MC_GPO, values, count);
        printError(res, QString("%1 get do state failed!").arg(name()));
        int subIndex = ioIndex % 32;
        bool result = ((values[count - 1] & (1 << subIndex)) != 0);
        delete[] values;
        return result;
    }
}

void GTDO::setImpl(bool state) noexcept
{
    short res = GTN_SetDoBit(gtioConfig->coreNo(), MC_GPO, gtioConfig->index(), state);
    printError(res, QString("%1 set do state to %2 failed!").arg(name()).arg(state));
}

void GTDO::initImpl()
{
    gtioConfig = qobject_cast<GTIOConfig *>(config());
    if (gtioConfig == nullptr)
    {
        throw SilicolAbort(
            QString("Can not cast IOConfig to GTIOConfig! IOConfig type: %1").arg(config()->metaObject()->className()),
            EX_LOCATION);
    }
}
