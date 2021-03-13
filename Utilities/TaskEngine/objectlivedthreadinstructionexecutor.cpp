#include "objectlivedthreadinstructionexecutor.h"

ObjectLivedThreadInstructionExecutor::ObjectLivedThreadInstructionExecutor(QObject *parent) : QObject(parent)
{
    connect(this, &ObjectLivedThreadInstructionExecutor::executeCmdReq, this, &ObjectLivedThreadInstructionExecutor::onExecuteCmdReq);
}

QVariant ObjectLivedThreadInstructionExecutor::runIns(const QString &cmd, const QVariantList &args, bool waitDone)
{
    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    if (waitDone)
    {
        insExeResults[uuid] = InstructionExecuteResult();
    }
    emit executeCmdReq(uuid, cmd, args, waitDone);
    if (waitDone)
    {
        return getInsResult(uuid);
    }
    else
    {
        return QVariant();
    }
}

QVariant ObjectLivedThreadInstructionExecutor::getInsResult(QString uuid)
{
    silicoolWait(
        -1, [&] { return insExeResults[uuid].isExecutionDone; }, 1);
    auto ier = insExeResults.take(uuid);
    if (!ier.errMsg.isEmpty())
    {
        throw SilicolAbort(ier.errMsg);
    }
    else
    {
        return ier.result;
    }
}

void ObjectLivedThreadInstructionExecutor::onExecuteCmdReq(QString uuid, QString cmd, QVariantList args, bool waitDone)
{
    Instruction ins(this, cmd, args);
    try
    {
        QVariant result = ins.execute();
        if (waitDone)
        {
            insExeResults[uuid].result = result;
        }
    }
    catch (SilicoolException &se)
    {
        if (waitDone)
        {
            insExeResults[uuid].errMsg = se.what();
        }
        else
        {
            qCritical() << se.what();
        }
    }
    if (waitDone)
    {
        insExeResults[uuid].isExecutionDone = true;
    }
}
