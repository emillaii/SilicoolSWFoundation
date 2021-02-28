#include "taskengineemanager.h"

TaskEngineeManager::TaskEngineeManager(QObject *parent, int nTaskEnginee) : QObject(parent), nTaskEnginee(nTaskEnginee)
{
    for (int i = 0; i < nTaskEnginee; i++)
    {
        TaskEnginee *taskEnginee = new TaskEnginee(this);
        connect(taskEnginee, &TaskEnginee::started, this, &TaskEngineeManager::onEngineeRunningStatesChanged);
        connect(taskEnginee, &TaskEnginee::finished, this, &TaskEngineeManager::onEngineeRunningStatesChanged);
        taskEnginees.append(taskEnginee);
    }
}

void TaskEngineeManager::runSingleCmd(QObject *target, QString cmd, QVariantList args)
{
    if (!checkWorkerInited(target))
    {
        return;
    }
    if (auto te = getIdleEnginee())
    {
        qCDebug(ieCate()) << "runSingleCmd, cmd:" << QString("%1::%2").arg(target->metaObject()->className()).arg(cmd) << "args:" << args;
        te->clearInstruction();
        te->appendInstruction(Instruction(target, cmd, args));
        te->start();
    }
    else
    {
        qCCritical(ieCate()) << QString("Ignored command: %1::%2").arg(target->metaObject()->className()).arg(cmd);
    }
}

void TaskEngineeManager::runMultiCmdsNoArg(QObject *target, QVariantList cmds)
{
    if (!checkWorkerInited(target))
    {
        return;
    }
    if (auto te = getIdleEnginee())
    {
        te->clearInstruction();
        QVariantList emptyList;
        foreach (auto cmd, cmds)
        {
            te->appendInstruction(Instruction(target, cmd.toString(), emptyList));
        }
        te->start();
    }
    else
    {
        qCCritical(ieCate()) << "Ignored commands, target:" << target->metaObject()->className() << "commands:" << cmds;
    }
}

void TaskEngineeManager::runMultiCmdsHasArg(QObject *target, QVariantList cmdArgPairs)
{
    if (!checkWorkerInited(target))
    {
        return;
    }
    if (auto te = getIdleEnginee())
    {
        te->clearInstruction();
        for (int i = 0; i < cmdArgPairs.count(); i++)
        {
            QVariantList cmdArgPair = cmdArgPairs[i].toList();
            Q_ASSERT(cmdArgPair.count() >= 1);
            QString cmd = cmdArgPair.takeAt(0).toString();
            te->appendInstruction(Instruction(target, cmd, cmdArgPair));
        }
        te->start();
    }
    else
    {
        QStringList cmds;
        for (int i = 0; i < cmdArgPairs.count(); i++)
        {
            QVariantList cmdArgPair = cmdArgPairs[i].toList();
            Q_ASSERT(cmdArgPair.count() >= 1);
            cmds << cmdArgPair[0].toString();
        }
        qCCritical(ieCate()) << "Ignored commands, target:" << target->metaObject()->className() << "commands:" << cmds;
    }
}

void TaskEngineeManager::runOnMultiObjects(QVariantList objCmdArgParis)
{
    if (auto te = getIdleEnginee())
    {
        te->clearInstruction();
        for (int i = 0; i < objCmdArgParis.count(); i++)
        {
            QVariantList objCmdArgPari = objCmdArgParis[i].toList();
            Q_ASSERT(objCmdArgPari.count() >= 2);
            QObject *target = objCmdArgPari.takeAt(0).value<QObject *>();
            Q_ASSERT(target);
            QString cmd = objCmdArgPari.takeAt(0).toString();
            te->appendInstruction(Instruction(target, cmd, objCmdArgPari));
        }
        te->start();
    }
    else
    {
        for (int i = 0; i < objCmdArgParis.count(); i++)
        {
            QVariantList objCmdArgPari = objCmdArgParis[i].toList();
            Q_ASSERT(objCmdArgPari.count() >= 2);
            QObject *target = objCmdArgPari.takeAt(0).value<QObject *>();
            Q_ASSERT(target);
            QString cmd = objCmdArgPari.takeAt(0).toString();
            qCCritical(ieCate()) << QString("Ignored command: %1::%2").arg(target->metaObject()->className()).arg(cmd);
        }
    }
}

void TaskEngineeManager::onEngineeRunningStatesChanged()
{
    bool allIdle = true;
    bool allBuzy = true;
    for (TaskEnginee *taskEnginee : taskEnginees)
    {
        if (taskEnginee->isRunning())
        {
            allIdle = false;
        }
        else
        {
            allBuzy = false;
        }
    }
    if (allIdle)
    {
        setEngineesState(EngineesState::AllIdle);
    }
    else if (allBuzy)
    {
        setEngineesState(EngineesState::AllBuzy);
    }
    else
    {
        setEngineesState(EngineesState::SomeIdleSomeBuzy);
    }
}

TaskEnginee *TaskEngineeManager::getIdleEnginee()
{
    foreach (auto te, taskEnginees)
    {
        if (!te->isRunning())
        {
            return te;
        }
    }

    UIOperation::getIns()->showMessage("Error", tr("All task enginees were buzy!"), MsgBoxIcon::Error, OkBtn);
    return nullptr;
}

bool TaskEngineeManager::checkWorkerInited(QObject *target)
{
    Worker *worker = qobject_cast<Worker *>(target);
    if (worker != nullptr && !worker->isInit())
    {
        UIOperation::getIns()->showError(tr("%1未初始化!").arg(worker->workerName()));
        return false;
    }
    return true;
}
