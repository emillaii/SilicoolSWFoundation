#include "xtdi.h"

int XtDI::diCount = -1;

int XtDI::generateNewDiID()
{
    if (diCount < 0)
    {
        diCount = XT_Controler_Extend::Profile_Get_IoIn_Count();
    }
    diCount += 2;
    return diCount;
}

XtDI::XtDI(QString name, QObject *parent) : SCDI(name, parent) {}

XtDI::XtDI(QString name, int id, QObject *parent) : SCDI(name, parent)
{
    this->id = id;
    dummyConfig = new IOConfig(this);
    dummyConfig->setReversal(false);
    setConfig(dummyConfig);
    init();
}

bool XtDI::getImpl() noexcept
{
    int inputState = 0;
    if (XT_Controler_Extend::Get_IoIn_State(id, inputState) != 1)
    {
        qCritical(motionCate()) << tr("Get digital input state failed!") << name();
        return false;
    }
    return inputState;
}

void XtDI::initImpl()
{
    if (id > 0)
    {
        return;
    }
    id = XT_Controler_Extend::Profile_Find_IoIn_Name(const_cast<LPWSTR>(name().toStdWString().c_str()));
    if (id < 0)
    {
        throw SilicolAbort(tr("Did not find digital input name: %1").arg(name()), EX_LOCATION);
    }
}
