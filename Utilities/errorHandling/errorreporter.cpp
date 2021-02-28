#include "errorreporter.h"

void ErrorReporter::init(const QString &processName, const QString &errorReceiverObjName)
{
    this->processName = processName;
    this->errorReceiverObjName = errorReceiverObjName;
}

QString ErrorReporter::reportError(const QString &module, const QString &msg)
{
    QVariantList args;
    QString errorId = QUuid::createUuid().toString(QUuid::Id128);
    args << errorId << processName << module << msg;
    InstructionExecutionClient::getIns().executeInstruction(errorReceiverObjName, "errorOccured", args);
    return errorId;
}

void ErrorReporter::errorHandled(const QString &errorId)
{
    QVariantList args;
    args << errorId;
    InstructionExecutionClient::getIns().executeInstruction(errorReceiverObjName, "errorHandled", args);
}

void ErrorReporter::requestAbort()
{
    InstructionExecutionClient::getIns().executeInstruction(errorReceiverObjName, "requestAbort", QVariantList());
}
