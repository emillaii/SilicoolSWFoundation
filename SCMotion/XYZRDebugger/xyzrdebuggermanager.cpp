#include "xyzrdebuggermanager.h"

XYZRDebuggerManager::XYZRDebuggerManager(QObject *parent) : QObject(parent)
{
    m_debuggerDefs = new ConfigObjectArray(&XYZRDebuggerDefinition::staticMetaObject, this);
    debuggerDefFile = new ConfigFile("XYZRDebuggerDefinition", m_debuggerDefs, debuggerDefFileName);
    debuggerDefFile->populate();

    m_debuggerConfigs = new ConfigObjectArray(&XYZRDebuggerConfig::staticMetaObject, this);
    debuggerConfigFile = new ConfigFile("XYZRDebuggerConfig", m_debuggerConfigs, debuggerConfigFileName);
    debuggerConfigFile->populate();

    m_debuggerConfigs->setUIAddRemovable(false);

    QStringList definedDbg;
    for (int i = 0; i < m_debuggerDefs->count(); i++)
    {
        definedDbg.append(m_debuggerDefs->getConfig<XYZRDebuggerDefinition>(i)->debuggerName());
    }
    definedDbg.removeDuplicates();
    for (int i = m_debuggerConfigs->count() - 1; i >= 0; i--)
    {
        auto dbgConfig = m_debuggerConfigs->getConfig<XYZRDebuggerConfig>(i);
        if (!definedDbg.contains(dbgConfig->debuggerName()))
        {
            m_debuggerConfigs->remove(i);
        }
        else
        {
            debuggerConfigMap[dbgConfig->debuggerName()] = dbgConfig;
        }
    }
    for (int i = 0; i < m_debuggerDefs->count(); i++)
    {
        auto dbgDef = m_debuggerDefs->getConfig<XYZRDebuggerDefinition>(i);
        if (!debuggerConfigMap.contains(dbgDef->debuggerName()))
        {
            XYZRDebuggerConfig *dbgConfig = new XYZRDebuggerConfig(m_debuggerConfigs);
            dbgConfig->setDebuggerName(dbgDef->debuggerName());
            m_debuggerConfigs->executeAddConfigObject(m_debuggerConfigs->count(), dbgConfig);
            debuggerConfigMap[dbgConfig->debuggerName()] = dbgConfig;
        }
        debuggerConfigMap[dbgDef->debuggerName()]->setShortcut(dbgDef->shortcut());
        debuggerMap[dbgDef->debuggerName()] = new XYZRDebugger(debuggerConfigMap[dbgDef->debuggerName()], this);
    }
}

XYZRDebuggerManager &XYZRDebuggerManager::getIns()
{
    static XYZRDebuggerManager instance;
    return instance;
}

ConfigObjectArray *XYZRDebuggerManager::debuggerDefs() const
{
    return m_debuggerDefs;
}

ConfigObjectArray *XYZRDebuggerManager::debuggerConfigs() const
{
    return m_debuggerConfigs;
}

QStringList XYZRDebuggerManager::debuggerNames() const
{
    return debuggerConfigMap.keys();
}

QStringList XYZRDebuggerManager::debuggerShortcuts() const
{
    QStringList shortcuts;
    for (int i = 0; i < m_debuggerDefs->count(); i++)
    {
        auto sc = m_debuggerDefs->getConfig<XYZRDebuggerDefinition>(i)->shortcut();
        if (!sc.isEmpty())
        {
            shortcuts.append(sc);
        }
    }
    return shortcuts;
}

void XYZRDebuggerManager::setOptionalAxisNames(QVariantList axisNames)
{
    foreach (auto config, debuggerConfigMap.values())
    {
        config->setOptionalAxisName(axisNames);
    }
}

QObject *XYZRDebuggerManager::debuggerConfig(QString debuggerName) const
{
    if (debuggerConfigMap.contains(debuggerName))
    {
        return debuggerConfigMap[debuggerName];
    }
    return nullptr;
}

QObject *XYZRDebuggerManager::debugger(QString debuggerName) const
{
    if (debuggerMap.contains(debuggerName))
    {
        return debuggerMap[debuggerName];
    }
    return nullptr;
}
