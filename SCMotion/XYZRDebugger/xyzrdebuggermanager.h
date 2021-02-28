#ifndef XYZRDEBUGGERMANAGER_H
#define XYZRDEBUGGERMANAGER_H

#include "configManager/configfile.h"
#include "configManager/configobjectarray.h"
#include "xyzrdebugger.h"
#include <QMap>
#include <QObject>

class SCMOTIONSHARED_EXPORT XYZRDebuggerManager : public QObject
{
    Q_OBJECT

private:
    XYZRDebuggerManager(QObject *parent = nullptr);

public:
    static XYZRDebuggerManager &getIns();

    ConfigObjectArray *debuggerDefs() const;

    ConfigObjectArray *debuggerConfigs() const;

    QStringList debuggerNames() const;

    QStringList debuggerShortcuts() const;

    void setOptionalAxisNames(QVariantList axisNames);

    Q_INVOKABLE QObject *debuggerConfig(QString debuggerName) const;

    Q_INVOKABLE QObject *debugger(QString debuggerName) const;

private:
    const QString debuggerDefFileName = "./config/platformConfig/xyzrDebuggerDefinition.json";
    const QString debuggerConfigFileName = "./config/platformConfig/xyzrDebuggerConfig.json";
    ConfigObjectArray *m_debuggerDefs;
    ConfigObjectArray *m_debuggerConfigs;
    ConfigFile *debuggerDefFile;
    ConfigFile *debuggerConfigFile;

    QMap<QString, XYZRDebuggerConfig *> debuggerConfigMap;
    QMap<QString, XYZRDebugger *> debuggerMap;
};

#endif    // XYZRDEBUGGERMANAGER_H
