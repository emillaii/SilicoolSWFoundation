#include "instructionexecutionserver.h"

SILICOOL_CREATE_LOGGING_CATEGORY(ieCate, "InstructionExecutor")

InstructionExecutionServer::InstructionExecutionServer()
{
    connect(this, &InstructionExecutionServer::executionDone, this, &InstructionExecutionServer::instructionExecuteDone, Qt::QueuedConnection);
}

InstructionExecutionServer &InstructionExecutionServer::getIns()
{
    static InstructionExecutionServer instance;
    return instance;
}

void InstructionExecutionServer::registerObject(const QString &objName, QObject *obj)
{
    objs[objName] = obj;
}

void InstructionExecutionServer::init(const QString &serverAddress)
{
    if (!isInit)
    {
        host.setHostUrl(serverAddress);
        host.enableRemoting(this);
        isInit = true;
    }
}

void InstructionExecutionServer::executeInstruction(QString uuid, QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async)
{
    if (objs.contains(targetObjName))
    {
        Instruction ins(objs[targetObjName], cmd, args);
        if (async)
        {
            QtConcurrent::run([&] { executeInstructionImpl(uuid, returnResult, ins); });
        }
        else
        {
            executeInstructionImpl(uuid, returnResult, ins);
        }
    }
    else
    {
        QString errMsg = tr("UnDefined object, Name: %1").arg(targetObjName);
        if (returnResult)
        {
            emit executionDone(uuid, errMsg, QVariant());
        }
        else
        {
            qCCritical(ieCate()) << errMsg;
        }
    }
}

void InstructionExecutionServer::executeInstructionImpl(const QString &uuid, bool returnResult, Instruction &ins)
{
    try
    {
        QVariant result = ins.execute();
        if (returnResult)
        {
            emit executionDone(uuid, "", result);
        }
    }
    catch (SilicoolException &se)
    {
        if (returnResult)
        {
            emit executionDone(uuid, se.what(), QVariant());
        }
        else
        {
            qCCritical(ieCate()) << se.what();
        }
    }
}
