#include "instructionexecutionclient.h"

void InstructionExecutionClient::init(const QString &serverAddress)
{
    this->serverAddress = serverAddress;
    if (!isInMainThread())
    {
        qFatal("InstructionExecutionClient have to be inited in main thread!");
    }
    if (!isInit)
    {
        node = new QRemoteObjectNode();
        node->connectToNode(serverAddress);
        ieReplica = node->acquire<InstructionExecutorReplica>();
        connect(this, &InstructionExecutionClient::executeInstructionReq, ieReplica, &InstructionExecutorReplica::executeInstruction);
        connect(ieReplica, &InstructionExecutorReplica::instructionExecuteDone, this, &InstructionExecutionClient::onInstructionExecuteDone);
        isInit = true;
    }
}

QVariant InstructionExecutionClient::runInstruction(const QString &targetObjName, const QString &cmd, const QVariantList &args, bool async)
{
    QString uuid = executeInstruction(targetObjName, cmd, args, true, async);
    return getInstructionExecutionResult(uuid);
}

QString InstructionExecutionClient::executeInstruction(
    const QString &targetObjName, const QString &cmd, const QVariantList &args, bool returnResult, bool async)
{
    if (!isInit)
    {
        qFatal("InstructionExecutionClient is not inited!");
    }
    while (true)
    {
        if (!silicoolWait(
                3000, [this] { return ieReplica->isReplicaValid(); }, 100))
        {
            qCritical() << "InstructionExecutionClient replica is invalid! Please check the connection to source:" << serverAddress;
        }
        else
        {
            break;
        }
    }

    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    if (returnResult)
    {
        instructionExecuteResults[uuid] = InstructionExecuteResult();
    }
    emit executeInstructionReq(uuid, targetObjName, cmd, args, returnResult, async);
    return uuid;
}

QVariant InstructionExecutionClient::getInstructionExecutionResult(const QString &uuid)
{
    if (!instructionExecuteResults.contains(uuid))
    {
        throw SilicolAbort(tr("Instruction with uuid %1 was not watched!").arg(uuid), EX_LOCATION);
    }
    silicoolWait(
        -1, [&] { return instructionExecuteResults[uuid].isExecutionDone; }, 1);
    InstructionExecuteResult ier = instructionExecuteResults[uuid];
    instructionExecuteResults.remove(uuid);
    if (!ier.errMsg.isEmpty())
    {
        throw SilicolAbort(ier.errMsg);
    }
    return ier.result;
}

InstructionExecutorReplica *InstructionExecutionClient::getIeReplica() const
{
    if (!isInit)
    {
        qFatal("InstructionExecutionClient is not inited!");
    }
    return ieReplica;
}

void InstructionExecutionClient::onInstructionExecuteDone(QString uuid, QString errMsg, QVariant result)
{
    instructionExecuteResults[uuid].errMsg = std::move(errMsg);
    instructionExecuteResults[uuid].result = std::move(result);
    instructionExecuteResults[uuid].isExecutionDone = true;
}
