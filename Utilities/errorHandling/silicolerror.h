#ifndef SILICOLERROR_H
#define SILICOLERROR_H

#include "../commonmethod.h"
#include "errorhandlingoption.h"
#include "utilities_global.h"
#include <QUuid>
#include <exception>

#define EX_LOCATION QString("File_%1, Line_%2, Function_%3").arg(__FILE__).arg(__LINE__).arg(__FUNCTION__)

void UTILITIESSHARED_EXPORT scTerminate();

typedef std::function<void()> SilicolFunc;

struct ExceptionInfo
{
public:
    ExceptionInfo() = default;
    ExceptionInfo(const QString &exceptionId, const QString &exceptionMsg);

    QString exceptionId;
    QString exceptionMsg;
    bool hasPrintMsg;
    int instanceCount;
};

class ExceptionInfoManager
{
private:
    ExceptionInfoManager() = default;

public:
    static ExceptionInfoManager &getIns()
    {
        static ExceptionInfoManager instance;
        return instance;
    }

    void exceptionConstructed(const QString &exceptionId, const QString &exceptionMsg)
    {
        exceptionInfos[exceptionId] = ExceptionInfo(exceptionId, exceptionMsg);
    }

    void exceptionCopyConstructed(const QString &exceptionId)
    {
        exceptionInfos[exceptionId].instanceCount++;
    }

    void exceptionPrinted(const QString &exceptionId)
    {
        exceptionInfos[exceptionId].hasPrintMsg = true;
    }

    void exceptionDestructed(const QString &exceptionId);

    void outputUnprintedException();

private:
    QMap<QString, ExceptionInfo> exceptionInfos;
};

class UTILITIESSHARED_EXPORT SilicoolException
{
public:
    SilicoolException(const QString &errMsg) : description(errMsg.toUtf8())
    {
        errId = QUuid::createUuid().toString(QUuid::Id128);
        ExceptionInfoManager::getIns().exceptionConstructed(errId, errMsg);
    }

    SilicoolException(const SilicoolException &other)
    {
        this->errId = other.errId;
        this->description = other.description;
        ExceptionInfoManager::getIns().exceptionCopyConstructed(this->errId);
    }

    const char *what()
    {
        ExceptionInfoManager::getIns().exceptionPrinted(errId);
        return description.data();
    }

    virtual ~SilicoolException()
    {
        ExceptionInfoManager::getIns().exceptionDestructed(errId);
    }

private:
    QString errId;
    QByteArray description;
};

class UTILITIESSHARED_EXPORT SilicolError : public SilicoolException
{
public:
    ///
    /// \brief SilicolError
    /// \param moduleName
    /// \param errorMsg
    /// \param suggestion Your suggestion for how to handle this error.
    ///
    SilicolError(const QString &moduleName, const QString &errorMsg, const QString &suggestion = "");

    ///
    /// \brief setAutoHandlingOption
    /// If you set this option, framework would use this error handling option rather than get error handling option
    /// from UI.
    /// \param autoHandlingOption
    ///
    void setAutoHandlingOption(const QString &autoHandlingOption)
    {
        this->autoHandlingOption = autoHandlingOption;
    }

protected:
    void appendErrorHandlingOption(const QString &option);

    ///
    /// \brief implemented
    /// To prevent instantiate this class, for the reason that error handling options is empty.
    /// Derived class just need to overwrite this function with an empty function body.
    ///
    virtual void implemented() = 0;

private:
    friend class ErrorHandler;

    QString moduleName;
    QString errorMsg;
    QList<QString> errorHandlingOptions;
    QString suggestion;

    QString autoHandlingOption;
};

class UTILITIESSHARED_EXPORT UnhandledError : public SilicoolException
{
public:
    UnhandledError(SilicolError &se, const QString &errHandlingOption)
        : SilicoolException(
            QString("UnHandled error: %1, Error handling option: %2").arg(se.what()).arg(errHandlingOption)),
          errHandlingOption(errHandlingOption)
    {
    }

public:
    QString errHandlingOption;
};

class GeneralError : public SilicolError
{
public:
    GeneralError(const QString &moduleName,
                 const QString &errorMsg,
                 bool canIgnore = false,
                 const QString &suggestion = "")
        : SilicolError(moduleName, errorMsg, suggestion)
    {
        appendErrorHandlingOption(Retry_);
        if (canIgnore)
        {
            appendErrorHandlingOption(Ignore_);
        }
        appendErrorHandlingOption(Abort_);
    }

    // SilicolError interface
protected:
    virtual void implemented() override {}
};

class UTILITIESSHARED_EXPORT SilicolAbort : public SilicoolException
{
public:
    SilicolAbort(const QString &abortMsg, const QString &location = "");
};

#endif    // SILICOLERROR_H
