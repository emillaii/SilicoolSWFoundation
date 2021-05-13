#include "axismodule.h"

ModulePos::ModulePos()
{
    setReadOnlyProperty("posName");
    setIdentityProperty("posName");
    addEvent("MoveTo");
    addEvent("UpdatePos");
    init();
}

AxisModuleConfig::AxisModuleConfig(const QMetaObject *modulePosMetaObj, QObject *parent) : ConfigObject(parent), modulePosMetaObj(modulePosMetaObj)
{
    m_modulePositions = new ConfigObjectArray(modulePosMetaObj, this);
    m_modulePositions->setUIAddRemovable(false);
    setReadOnlyProperty("moduleName");
    setIdentityProperty("moduleName");
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
    m_axisModuleConfig->modulePos2Dic();
    qCDebug(motionCate()) << "Init" << m_axisModuleConfig->moduleName();
    initImpl();
}

bool AxisModule::useVelInPosCfg() const
{
    return m_useVelInPosCfg;
}

void AxisModule::setUseVelInPosCfg(bool useVelInPosCfg)
{
    m_useVelInPosCfg = useVelInPosCfg;
}

bool AxisModule::useAccInPosCfg() const
{
    return m_useAccInPosCfg;
}

void AxisModule::setUseAccInPosCfg(bool useAccInPosCfg)
{
    m_useAccInPosCfg = useAccInPosCfg;
}
