#include "instruction.h"

QMutex Instruction::locker;
QMap<QString, ClassMetaMethods> Instruction::methodCache;

Instruction::Instruction(QObject *target, const QString &cmd, const QVariantList &args) : target(target), cmd(cmd), args(args)
{
    className = target->metaObject()->className();
    for (int i = 0; i < 10; i++)
    {
        genericArgs.append(QGenericArgument(nullptr));
    }
}

Instruction::Instruction(const Instruction &other)
    : target(other.target), cmd(other.cmd), args(other.args), className(other.className), genericArgs(other.genericArgs)
{
}

QVariant Instruction::execute()
{
    extractMethods();
    if (!methodCache[className].contains(cmd))
    {
        throw SilicolAbort(QObject::tr("Did not find method: %1 in class %2").arg(cmd).arg(className), EX_LOCATION);
    }
    for (QMetaMethod &method : methodCache[className][cmd])
    {
        if (convertArgType(method))
        {
            if (method.returnType() == QMetaType::Void)
            {
                if (method.invoke(target, Qt::DirectConnection, genericArgs[0], genericArgs[1], genericArgs[2], genericArgs[3], genericArgs[4],
                                  genericArgs[5], genericArgs[6], genericArgs[7], genericArgs[8], genericArgs[9]))
                {
                    return QVariant();
                }
            }
            else
            {
                QVariant result(method.returnType(), nullptr);
                if (method.invoke(target, Qt::DirectConnection, QGenericReturnArgument(QVariant::typeToName(method.returnType()), result.data()),
                                  genericArgs[0], genericArgs[1], genericArgs[2], genericArgs[3], genericArgs[4], genericArgs[5], genericArgs[6],
                                  genericArgs[7], genericArgs[8], genericArgs[9]))

                {
                    return result;
                }
            }
        }
    }
    reportError();
    return QVariant();
}

void Instruction::extractMethods()
{
    QMutexLocker tmpLocker(&locker);
    if (!methodCache.contains(className))
    {
        ClassMetaMethods cmm;
        for (int i = 0; i < target->metaObject()->methodCount(); i++)
        {
            auto method = target->metaObject()->method(i);
            if (!cmm.contains(method.name()))
            {
                cmm[method.name()] = MetaMethods();
            }
            cmm[method.name()].append(method);
        }
        methodCache[className] = cmm;
    }
}

bool Instruction::convertArgType(QMetaMethod &method)
{
    if (method.parameterCount() != args.count())
    {
        return false;
    }
    typeConvertedArgs = QVariantList(args);
    for (int i = 0; i < args.count(); i++)
    {
        int argType = method.parameterType(i);
        if (typeConvertedArgs[i].convert(argType))
        {
            genericArgs[i] = QGenericArgument(QVariant::typeToName(argType), typeConvertedArgs[i].data());
        }
        else
        {
            return false;
        }
    }
    return true;
}

void Instruction::reportError()
{
    QString argSignature;
    for (int i = 0; i < args.count(); i++)
    {
        argSignature.append(args[i].typeName());
        if (i != args.count() - 1)
        {
            argSignature.append(",");
        }
    }
    QString substitutesSignature;
    for (QMetaMethod &method : methodCache[className][cmd])
    {
        substitutesSignature.append(method.methodSignature() + ",");
    }
    if (substitutesSignature.length() > 0)
    {
        substitutesSignature = substitutesSignature.remove(substitutesSignature.length() - 1, 1);
    }
    throw SilicolAbort(QObject::tr("Did not find matched method. Calling signature: %1(%2), substitutes: %3")
                           .arg(className + "::" + cmd)
                           .arg(argSignature)
                           .arg(substitutesSignature),
                       EX_LOCATION);
}
