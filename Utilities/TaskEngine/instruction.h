#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "./errorHandling/silicolerror.h"
#include "instructionexecutionresult.h"
#include "utilities_global.h"
#include <QMetaMethod>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

typedef QList<QMetaMethod> MetaMethods;
typedef QMap<QString, MetaMethods> ClassMetaMethods;

///
/// \brief The Instruction class
/// default parameter supported
/// overload function supoorted
/// parameter type conversion supported
///
class UTILITIESSHARED_EXPORT Instruction
{
public:
    Instruction(QObject *target, const QString &cmd, const QVariantList &args);

    QVariant execute();

    template <typename T>
    T execute()
    {
        QVariant result = execute();
        if (result.canConvert<T>())
        {
            return result.value<T>();
        }
        else
        {
            throw SilicolAbort(QObject::tr("Error return type!"), EX_LOCATION);
        }
    }

private:
    void extractMethods();

    bool convertArgType(QMetaMethod &method);

    void reportError();

private:
    QObject *target;
    QString className;
    QString cmd;
    QVariantList args;

    QVariantList typeConvertedArgs;
    QList<QGenericArgument> genericArgs;

    static QMutex locker;
    static QMap<QString, ClassMetaMethods> methodCache;
};

#endif    // INSTRUCTION_H
