#ifndef ERRORREPORTER_H
#define ERRORREPORTER_H

#include "TaskEngine/instructionexecutionclient.h"
#include "commonmethod.h"
#include "utilities_global.h"
#include <QObject>
#include <QUuid>

class UTILITIESSHARED_EXPORT ErrorReporter
{
private:
    explicit ErrorReporter() = default;

public:
    static ErrorReporter &getIns()
    {
        static ErrorReporter instance;
        return instance;
    }

    void init(const QString &processName, const QString &errorReceiverObjName);

    QString reportError(const QString &module, const QString &msg);

    void errorHandled(const QString &errorId);

    void requestAbort();

private:
    QString errorReceiverObjName = "UnDefinedName";
    QString processName = "UnDefinedProcessName";
};

#endif    // ERRORREPORTER_H
