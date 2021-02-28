#include "xtdo.h"

XtDO::XtDO(QString name, QObject *parent) : SCDO(name, parent) {}

XtDO::XtDO(QString name, int id, QObject *parent) : SCDO(name, parent)
{
    this->id = id;
    dummyConfig = new IOConfig(this);
    dummyConfig->setReversal(false);
    setConfig(dummyConfig);
    init();
}

void XtDO::initImpl()
{
    if (id > 0)
    {
        return;
    }
    id = XT_Controler_Extend::Profile_Find_IoOut_Name(const_cast<LPWSTR>(name().toStdWString().c_str()));
    if (id < 0)
    {
        throw SilicolAbort(tr("Did not find digital output name: %1").arg(name()), EX_LOCATION);
    }
}

bool XtDO::getImpl() noexcept
{
    int outputState = 0;
    if (XT_Controler_Extend::Get_IoOut_State(id, outputState) != 1)
    {
        qCritical(motionCate()) << tr("Get digital output state failed!") << name();
        return false;
    }
    return outputState;
}

void XtDO::setImpl(bool state) noexcept
{
    if (XT_Controler_Extend::DigitOut_PreciousTrig_SetCurIoOutput(id, state) != 1)
    {
        qCritical(motionCate()) << tr("Set digital output state failed!") << name();
    }
}

int XtDO::getId() const
{
    return id;
}
