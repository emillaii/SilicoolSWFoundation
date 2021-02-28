#include "axismodule.h"

ModulePos::ModulePos()
{
    setReadOnlyProperty("posName");
    addEvent("MoveTo");
    addEvent("UpdatePos");
    init();
}

AxisModuleConfig::AxisModuleConfig(const QMetaObject *modulePosMetaObj, QObject *parent) : ConfigObject(parent), modulePosMetaObj(modulePosMetaObj)
{
    m_modulePositions = new ConfigObjectArray(modulePosMetaObj, this);
    m_modulePositions->setUIAddRemovable(false);
    setReadOnlyProperty("moduleName");
    init();
}

void ModulePos::updatePosWithCurrentPos(QVariantMap axesCurrentPos)
{
    foreach (auto key, axesCurrentPos.keys())
    {
        setConfig(key, axesCurrentPos[key]);
    }
}

void ModulePos::handleEvent(QString event)
{
    if (event == "MoveTo")
    {
        emit moveToReq(axisModuleConfig->moduleName(), getModuleType(), posName());
    }
    else if (event == "UpdatePos")
    {
        emit updatePosReq(axisModuleConfig->moduleName(), getModuleType(), posName());
    }
    else
    {
        ConfigObject::handleEvent(event);
    }
}

void ModulePos::setAxisModuleConfig(AxisModuleConfig *value)
{
    axisModuleConfig = value;
}

void AxisModule::init()
{
    axisModuleConfig->modulePos2Dic();
    qCDebug(motionCate()) << "Init" << axisModuleConfig->moduleName();
    initImpl();
}
