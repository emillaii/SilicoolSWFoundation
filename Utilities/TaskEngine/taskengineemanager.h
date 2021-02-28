#ifndef TASKENGINEEMANAGER_H
#define TASKENGINEEMANAGER_H

#include "systemCore/worker.h"
#include "taskenginee.h"
#include "uiHelper/uioperation.h"
#include "utilities_global.h"
#include <QObject>

///
/// \brief The TaskEngineeManager class
/// abbreviation explaination
///
class UTILITIESSHARED_EXPORT TaskEngineeManager : public QObject
{
    Q_OBJECT

public:
    enum EngineesState
    {
        AllIdle,
        AllBuzy,
        SomeIdleSomeBuzy
    };
    Q_ENUM(EngineesState)
    Q_PROPERTY(EngineesState engineesState READ engineesState WRITE setEngineesState NOTIFY engineesStateChanged)

    explicit TaskEngineeManager(QObject *parent = nullptr, int nTaskEnginee = 3);

    ///
    /// \brief runSingleCmd  run single command on single object
    /// \param target object to run the command
    /// \param cmd command name
    /// \param args command argument
    ///
    Q_INVOKABLE void runSingleCmd(QObject *target, QString cmd, QVariantList args = QVariantList());

    ///
    /// \brief runMultiCmdsNoArg run multi commands without argument on single object
    /// \param cmds the list of command names
    ///
    Q_INVOKABLE void runMultiCmdsNoArg(QObject *target, QVariantList cmds);

    ///
    /// \brief runMultiCmdsHasArg run multi commands with argument on single object
    /// \param cmdArgPairs example: [[cmd1, arg1, arg2...], [cmd2, arg1, arg2...], [cmd3, arg1, arg2...]]
    ///
    Q_INVOKABLE void runMultiCmdsHasArg(QObject *target, QVariantList cmdArgPairs);

    ///
    /// \brief runOnMultiObjects run multi commands with argument on multi objects
    /// \param objCmdArgParis example: [[obj1, cmd1, arg1, arg2...], [obj1, cmd2, arg1, arg2...], [obj2, cmd3, arg1,
    /// arg2...]]
    ///
    Q_INVOKABLE void runOnMultiObjects(QVariantList objCmdArgParis);

    EngineesState engineesState() const
    {
        return m_engineesState;
    }

public slots:
    void setEngineesState(TaskEngineeManager::EngineesState engineesState)
    {
        if (m_engineesState == engineesState)
            return;

        m_engineesState = engineesState;
        emit engineesStateChanged(m_engineesState);
    }

signals:
    void engineesStateChanged(TaskEngineeManager::EngineesState engineesState);

private slots:
    void onEngineeRunningStatesChanged();

private:
    TaskEnginee *getIdleEnginee();
    bool checkWorkerInited(QObject *target);

private:
    int nTaskEnginee;
    QList<TaskEnginee *> taskEnginees;
    EngineesState m_engineesState{ AllIdle };
};

#endif    // TASKENGINEEMANAGER_H
