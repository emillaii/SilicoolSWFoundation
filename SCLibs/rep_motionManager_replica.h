#ifndef REP_MOTIONMANAGER_REPLICA_H
#define REP_MOTIONMANAGER_REPLICA_H

// This is an autogenerated file.
// Do not edit this file, any changes made will be lost the next time it is generated.

#include <QtCore/qobject.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmetatype.h>

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectpendingcall.h>
#include <QtRemoteObjects/qremoteobjectreplica.h>


class MotionManagerReplica : public QRemoteObjectReplica
{
    Q_OBJECT
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_TYPE, "MotionManager")
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_SIGNATURE, "844aded0659851c4bfdd10bfa7f54b89eca2d0f4")

public:
    MotionManagerReplica() : QRemoteObjectReplica() { initialize(); }
    static void registerMetatypes()
    {
        static bool initialized = false;
        if (initialized)
            return;
        initialized = true;
    }

private:
    MotionManagerReplica(QRemoteObjectNode *node, const QString &name = QString())
        : QRemoteObjectReplica(ConstructWithNode)
    {
        initializeNode(node, name);
    }

    void initialize() override
    {
        MotionManagerReplica::registerMetatypes();
        QVariantList properties;
        properties.reserve(0);
        setProperties(properties);
    }

public:
    virtual ~MotionManagerReplica() {}


Q_SIGNALS:
    void reportMotionState(QVariant motionState);
    void instructionExecuteDone(QString uuid, QString errMsg, QVariant result);

public Q_SLOTS:
    void setMotionStateReporter(bool enable)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("setMotionStateReporter(bool)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(enable)
        ;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void setMoveProtectionEnable(bool enable)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("setMoveProtectionEnable(bool)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(enable)
        ;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void updateMoveProtection()
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("updateMoveProtection()");
        QVariantList __repc_args;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    QRemoteObjectPendingReply<bool> initMotionManager(QByteArray motionElementDefinition)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("initMotionManager(QByteArray)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(motionElementDefinition)
        ;
        return QRemoteObjectPendingReply<bool>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    void setIsPaused(bool isPaused)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("setIsPaused(bool)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(isPaused)
        ;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    QRemoteObjectPendingReply<bool> waitPaused()
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("waitPaused()");
        QVariantList __repc_args;
        return QRemoteObjectPendingReply<bool>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    void handleAbort()
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("handleAbort()");
        QVariantList __repc_args;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    void handleReset()
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("handleReset()");
        QVariantList __repc_args;
        send(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args);
    }
    QRemoteObjectPendingReply<double> getAxisFeedbackPos(QString axisName, int cacheInvalidationTime=2)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("getAxisFeedbackPos(QString,int)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(axisName)
            << QVariant::fromValue(cacheInvalidationTime=2)
        ;
        return QRemoteObjectPendingReply<double>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    QRemoteObjectPendingReply<double> getAxisCurrentPos(QString axisName)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("getAxisCurrentPos(QString)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(axisName)
        ;
        return QRemoteObjectPendingReply<double>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    QRemoteObjectPendingReply<double> getAxisTargetPos(QString axisName, int cacheInvalidationTime=2)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("getAxisTargetPos(QString,int)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(axisName)
            << QVariant::fromValue(cacheInvalidationTime=2)
        ;
        return QRemoteObjectPendingReply<double>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    QRemoteObjectPendingReply<int> getCylinderCurrentState(QString cylName, int cacheInvalidationTime=2)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("getCylinderCurrentState(QString,int)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(cylName)
            << QVariant::fromValue(cacheInvalidationTime=2)
        ;
        return QRemoteObjectPendingReply<int>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    QRemoteObjectPendingReply<bool> getDiCurrentState(QString diName, int cacheInvalidationTime=2)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("getDiCurrentState(QString,int)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(diName)
            << QVariant::fromValue(cacheInvalidationTime=2)
        ;
        return QRemoteObjectPendingReply<bool>(sendWithReply(QMetaObject::InvokeMetaMethod, __repc_index, __repc_args));
    }
    void onExecuteInstruction(QString uuid, int elementType, QString elementName, QString cmd, QVariantList args, bool sendbackResult=false)
    {
        static int __repc_index = MotionManagerReplica::staticMetaObject.indexOfSlot("onExecuteInstruction(QString,int,QString,QString,QVariantList,bool)");
        QVariantList __repc_args;
        __repc_args
            << QVariant::fromValue(uuid)
            << QVariant::fromValue(elementType)
            << QVariant::fromValue(elementName)
            << QVariant::fromValue(cmd)
            << QVariant::fromValue(args)
            << QVariant::fromValue(sendbackResult=false)
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


#endif // REP_MOTIONMANAGER_REPLICA_H
