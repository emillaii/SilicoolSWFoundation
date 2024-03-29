#ifndef REP_INSTRUCTIONEXECUTOR_REPLICA_H
#define REP_INSTRUCTIONEXECUTOR_REPLICA_H

// This is an autogenerated file.
// Do not edit this file, any changes made will be lost the next time it is generated.

#include <QtCore/qobject.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmetatype.h>

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectpendingcall.h>
#include <QtRemoteObjects/qremoteobjectreplica.h>


class InstructionExecutorReplica : public QRemoteObjectReplica
{
    Q_OBJECT
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_TYPE, "InstructionExecutor")
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_SIGNATURE, "ea8e4b6fbd1da2b0cea574a6aae48d5565dd1b2f")

public:
    InstructionExecutorReplica() : QRemoteObjectReplica() { initialize(); }
    static void registerMetatypes()
    {
        static bool initialized = false;
        if (initialized)
            return;
        initialized = true;
    }

private:
    InstructionExecutorReplica(QRemoteObjectNode *node, const QString &name = QString())
        : QRemoteObjectReplica(ConstructWithNode)
    {
        initializeNode(node, name);
    }

    void initialize() override
    {
        InstructionExecutorReplica::registerMetatypes();
        QVariantList properties;
        properties.reserve(0);
        setProperties(properties);
    }

public:
    virtual ~InstructionExecutorReplica() {}


Q_SIGNALS:
    void uiResponse(QString uuid, QString option);
    void instructionExecuteDone(QString uuid, QString errMsg, QVariant result);

public Q_SLOTS:
    void executeInstruction(QString uuid, QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async=false)
    {
        static int __repc_index = InstructionExecutorReplica::staticMetaObject.indexOfSlot("executeInstruction(QString,QString,QString,QVariantList,bool,bool)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(uuid)
            << QVariant::fromValue(targetObjName)
            << QVariant::fromValue(cmd)
            << QVariant::fromValue(args)
            << QVariant::fromValue(returnResult)
            << QVariant::fromValue(async=false)
        ;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }

private:
    friend class QT_PREPEND_NAMESPACE(QRemoteObjectNode);
};

#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
#endif


QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


#endif // REP_INSTRUCTIONEXECUTOR_REPLICA_H
