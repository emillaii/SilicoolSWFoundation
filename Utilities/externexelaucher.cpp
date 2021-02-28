#include "externexelaucher.h"

ExternExeLaucher::ExternExeLaucher(QObject *parent) : QObject(parent)
{
    externExeConfigs = new ConfigObjectArray(&ExternExeConfig::staticMetaObject, this);
    externExeConfigsFile = new ConfigFile("ExternExeConfigs", externExeConfigs, "./config/externExeConfig.json", false);
    externExeConfigsFile->populate();
    for (int i = 0; i < externExeConfigs->count(); ++i)
    {
        auto externExeConfig = externExeConfigs->getConfig<ExternExeConfig>(i);
        externExes[externExeConfig->exeName()] = externExeConfig;
    }
}

QStringList ExternExeLaucher::externExeNames() const
{
    return externExes.keys();
}

void ExternExeLaucher::open(QString exeName)
{
    if(!externExes.contains(exeName))
    {
        UIOperation::getIns()->showError( tr("Undefined exe name: %1").arg(exeName));
        return;
    }
    QString exeFileFullName = externExes[exeName]->exeFileFullName();
    QFileInfo f(exeFileFullName);
    if(!f.exists())
    {
        UIOperation::getIns()->showError( tr("Exe did not exist: %1").arg(exeFileFullName));
        return;
    }

    QProcess p;
    p.setProgram("cmd.exe");
    p.setWorkingDirectory(f.absolutePath());
    QStringList args;
    args << "/c" << f.fileName();
    p.setArguments(args);
    p.startDetached();
    p.waitForStarted();
}

ConfigObjectArray *ExternExeLaucher::getExternExeConfigs() const
{
    return externExeConfigs;
}
