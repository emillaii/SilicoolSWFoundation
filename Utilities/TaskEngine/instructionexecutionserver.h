#ifndef INSTRUCTIONEXECUTIONSERVER_H
#define INSTRUCTIONEXECUTIONSERVER_H

#include "./loging/Logger.h"
#include "instruction.h"
#include "rep_instructionExecutor_source.h"
#include "utilities_global.h"
#include <QMap>
#include <QObject>
#include <QUuid>
#include <QtConcurrent>

SILICOOL_DECLARE_LOGGING_CATEGORY(ieCate, )

class UTILITIESSHARED_EXPORT InstructionExecutionServer : public InstructionExecutorSimpleSource
{
    Q_OBJECT

private:
    explicit InstructionExecutionServer();

public:
    static InstructionExecutionServer &getIns();

    void registerObject(const QString &objName, QObject *obj);

    void init(const QString &serverAddress);

signals:
    void executionDone(QString uuid, QString errMsg, QVariant result);

    // InstructionExecutorSource interface
public slots:
    virtual void
    executeInstruction(QString uuid, QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async = false) override;

private:
    void executeInstructionImpl(const QString &uuid, bool returnResult, Instruction &ins);

private:
    QMap<QString, QObject *> objs;
    QRemoteObjectHost host;
    bool isInit = false;
};

#endif    // INSTRUCTIONEXECUTIONSERVER_H
