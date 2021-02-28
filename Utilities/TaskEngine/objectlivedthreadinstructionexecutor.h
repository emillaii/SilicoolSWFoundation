#ifndef OBJECTLIVEDTHREADINSTRUCTIONEXECUTOR_H
#define OBJECTLIVEDTHREADINSTRUCTIONEXECUTOR_H

#include "errorHandling/silicolerror.h"
#include "instruction.h"
#include "instructionexecutionresult.h"
#include "utilities_global.h"
#include <QMap>
#include <QObject>
#include <QUuid>

class UTILITIESSHARED_EXPORT ObjectLivedThreadInstructionExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ObjectLivedThreadInstructionExecutor(QObject *parent = nullptr);

    template <typename T>
    T runIns(const QString &cmd, const QVariantList &args = QVariantList())
    {
        return runIns(cmd, args, true).value<T>();
    }

    QVariant runIns(const QString &cmd, const QVariantList &args = QVariantList(), bool waitDone = true);

private:
    QVariant getInsResult(QString uuid);

signals:
    void executeCmdReq(QString uuid, QString cmd, QVariantList args, bool waitDone);

private slots:
    void onExecuteCmdReq(QString uuid, QString cmd, QVariantList args, bool waitDone);

private:
    QMap<QString, InstructionExecuteResult> insExeResults;
};

#endif    // OBJECTLIVEDTHREADINSTRUCTIONEXECUTOR_H
