#ifndef INSTRUCTIONEXECUTIONRESULT_H
#define INSTRUCTIONEXECUTIONRESULT_H

#include "utilities_global.h"
#include <QObject>
#include <QVariant>

struct UTILITIESSHARED_EXPORT InstructionExecuteResult
{
public:
    bool isExecutionDone = false;
    QString errMsg;
    QVariant result;
};

#endif    // INSTRUCTIONEXECUTIONRESULT_H
