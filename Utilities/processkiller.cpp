#include "processkiller.h"

void killProcess(qint64 pid)
{
    QProcess p;
    p.setProgram("cmd.exe");
    QStringList args;
    args << "/c" << QString("taskkill /F /PID %1 /T").arg(pid);
    p.setArguments(args);
    p.startDetached();
}
