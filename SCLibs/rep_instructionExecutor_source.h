#ifndef REP_INSTRUCTIONEXECUTOR_SOURCE_H
#define REP_INSTRUCTIONEXECUTOR_SOURCE_H

// This is an autogenerated file.
// Do not edit this file, any changes made will be lost the next time it is generated.

#include <QtCore/qobject.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qvariant.h>
#include <QtCore/qmetatype.h>

#include <QtRemoteObjects/qremoteobjectnode.h>
#include <QtRemoteObjects/qremoteobjectsource.h>


class InstructionExecutorSource : public QObject
{
    Q_OBJECT
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_TYPE, "InstructionExecutor")
    Q_CLASSINFO(QCLASSINFO_REMOTEOBJECT_SIGNATURE, "ea8e4b6fbd1da2b0cea574a6aae48d5565dd1b2f")

public:
    explicit InstructionExecutorSource(QObject *parent = nullptr) : QObject(parent)
    {
    }

public:
    virtual ~InstructionExecutorSource() {}


Q_SIGNALS:
    void uiResponse(QString uuid, QString option);
    void instructionExecuteDone(QString uuid, QString errMsg, QVariant result);

public Q_SLOTS:
    virtual void executeInstruction(QString uuid, QString targetObjName, QString cmd, QVariantList args, bool returnResult, bool async=false) = 0;

private:
    friend class QT_PREPEND_NAMESPACE(QRemoteObjectNode);
};

#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
#endif


class InstructionExecutorSimpleSource : public InstructionExecutorSource
{
    Q_OBJECT

public:
    explicit InstructionExecutorSimpleSource(QObject *parent = nullptr) : InstructionExecutorSource(parent)
    {
    }

public:
    virtual ~InstructionExecutorSimpleSource() {}


private:
};


template <class ObjectType>
struct InstructionExecutorSourceAPI : public SourceApiMap
{
    InstructionExecutorSourceAPI(ObjectType *object, const QString &name = QStringLiteral("InstructionExecutor"))
        : SourceApiMap(), m_name(name)
    {
        Q_UNUSED(object);
        m_enums[0] = 0;
        m_properties[0] = 0;
        m_signals[0] = 2;
        m_signals[1] = QtPrivate::qtro_signal_index<ObjectType>(&ObjectType::uiResponse, static_cast<void (QObject::*)(QString,QString)>(0),m_signalArgCount+0,&m_signalArgTypes[0]);
        m_signals[2] = QtPrivate::qtro_signal_index<ObjectType>(&ObjectType::instructionExecuteDone, static_cast<void (QObject::*)(QString,QString,QVariant)>(0),m_signalArgCount+1,&m_signalArgTypes[1]);
        m_methods[0] = 1;
        m_methods[1] = QtPrivate::qtro_method_index<ObjectType>(&ObjectType::executeInstruction, static_cast<void (QObject::*)(QString,QString,QString,QVariantList,bool,bool)>(0),"executeInstruction(QString,QString,QString,QVariantList,bool,bool)",m_methodArgCount+0,&m_methodArgTypes[0]);
    }

    QString name() const override { return m_name; }
    QString typeName() const override { return QStringLiteral("InstructionExecutor"); }
    int enumCount() const override { return m_enums[0]; }
    int propertyCount() const override { return m_properties[0]; }
    int signalCount() const override { return m_signals[0]; }
    int methodCount() const override { return m_methods[0]; }
    int sourceEnumIndex(int index) const override
    {
        if (index < 0 || index >= m_enums[0])
            return -1;
        return m_enums[index+1];
    }
    int sourcePropertyIndex(int index) const override
    {
        if (index < 0 || index >= m_properties[0])
            return -1;
        return m_properties[index+1];
    }
    int sourceSignalIndex(int index) const override
    {
        if (index < 0 || index >= m_signals[0])
            return -1;
        return m_signals[index+1];
    }
    int sourceMethodIndex(int index) const override
    {
        if (index < 0 || index >= m_methods[0])
            return -1;
        return m_methods[index+1];
    }
    int signalParameterCount(int index) const override
    {
        if (index < 0 || index >= m_signals[0])
            return -1;
        return m_signalArgCount[index];
    }
    int signalParameterType(int sigIndex, int paramIndex) const override
    {
        if (sigIndex < 0 || sigIndex >= m_signals[0] || paramIndex < 0 || paramIndex >= m_signalArgCount[sigIndex])
            return -1;
        return m_signalArgTypes[sigIndex][paramIndex];
    }
    int methodParameterCount(int index) const override
    {
        if (index < 0 || index >= m_methods[0])
            return -1;
        return m_methodArgCount[index];
    }
    int methodParameterType(int methodIndex, int paramIndex) const override
    {
        if (methodIndex < 0 || methodIndex >= m_methods[0] || paramIndex < 0 || paramIndex >= m_methodArgCount[methodIndex])
            return -1;
        return m_methodArgTypes[methodIndex][paramIndex];
    }
    int propertyIndexFromSignal(int index) const override
    {
        Q_UNUSED(index);
        return -1;
    }
    int propertyRawIndexFromSignal(int index) const override
    {
        Q_UNUSED(index);
        return -1;
    }
    const QByteArray signalSignature(int index) const override
    {
        switch (index) {
        case 0: return QByteArrayLiteral("uiResponse(QString,QString)");
        case 1: return QByteArrayLiteral("instructionExecuteDone(QString,QString,QVariant)");
        }
        return QByteArrayLiteral("");
    }
    QList<QByteArray> signalParameterNames(int index) const override
    {
        if (index < 0 || index >= m_signals[0])
            return QList<QByteArray>();
        return ObjectType::staticMetaObject.method(m_signals[index + 1]).parameterNames();
    }
    const QByteArray methodSignature(int index) const override
    {
        switch (index) {
        case 0: return QByteArrayLiteral("executeInstruction(QString,QString,QString,QVariantList,bool,bool)");
        }
        return QByteArrayLiteral("");
    }
    QMetaMethod::MethodType methodType(int) const override
    {
        return QMetaMethod::Slot;
    }
    QList<QByteArray> methodParameterNames(int index) const override
    {
        if (index < 0 || index >= m_methods[0])
            return QList<QByteArray>();
        return ObjectType::staticMetaObject.method(m_methods[index + 1]).parameterNames();
    }
    const QByteArray typeName(int index) const override
    {
        switch (index) {
        case 0: return QByteArrayLiteral("void");
        }
        return QByteArrayLiteral("");
    }
    QByteArray objectSignature() const override { return QByteArray{"ea8e4b6fbd1da2b0cea574a6aae48d5565dd1b2f"}; }

    int m_enums[1];
    int m_properties[1];
    int m_signals[3];
    int m_methods[2];
    const QString m_name;
    int m_signalArgCount[2];
    const int* m_signalArgTypes[2];
    int m_methodArgCount[1];
    const int* m_methodArgTypes[1];
};

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


#endif // REP_INSTRUCTIONEXECUTOR_SOURCE_H