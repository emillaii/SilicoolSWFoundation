#ifndef INSTRUCTIONEXECUTIONCLIENT_H
#define INSTRUCTIONEXECUTIONCLIENT_H

#include "commonmethod.h"
#include "errorHandling/silicolerror.h"
#include "instructionexecutionresult.h"
#include "loging/Logger.h"
#include "rep_instructionExecutor_replica.h"
#include "utilities_global.h"
#include <QMap>
#include <QObject>

SILICOOL_DECLARE_LOGGING_CATEGORY(ieCate, )

class UTILITIESSHARED_EXPORT InstructionExecutionClient : public QObject
{
    Q_OBJECT

private:
    explicit InstructionExecutionClient() = default;

public:
    static InstructionExecutionClient &getIns()
    {
        static InstructionExecutionClient instance;
        return instance;
    }

    void init(const QString &serverAddress);

    QVariant runInstruction(const QString &targetObjName, const QString &cmd, const QVariantList &args, bool async);

    template <typename T>
    T runInstruction(const QString &targetObjName, const QString &cmd, const QVariantList &args, bool async)
    {
        return runInstruction(targetObjName, cmd, args, async).value<T>();
    }

    QString
    executeInstruction(const QString &targetObjName, const QString &cmd, const QVariantList &args, bool returnResult = false, bool async = false);

    QVariant getInstructionExecutionResult(const QString &uuid);

    template <typename T>
    T getInstructionExecutionResult(const QString &uuid)
    {
        return getInstructionExecutionResult(uuid).value<T>();
    }

    InstructionExecutorReplica *getIeReplica() const;

signals:
    QString executeInstructionReq(QString uuid, QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async);

private slots:
    void onInstructionExecuteDone(QString uuid, QString errMsg, QVariant result);

private:
    InstructionExecutorReplica *ieReplica = nullptr;
    QRemoteObjectNode *node = nullptr;
    bool isInit = false;
    QString serverAddress;
    QMap<QString, InstructionExecuteResult> instructionExecuteResults;
};

#endif    // INSTRUCTIONEXECUTIONCLIENT_H
