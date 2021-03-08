#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include "loging/rollbackfile.h"
#include "utilities_global.h"
#include <QObject>
#include <QStack>

class CommandInterface
{
public:
    virtual bool execute() = 0;
    virtual bool unExecute() = 0;
    virtual QString getExecutionLog() = 0;
    virtual QString getUnExecutionLog() = 0;

    virtual ~CommandInterface() = default;
};

class UTILITIESSHARED_EXPORT ConfigManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int commandCount READ commandCount WRITE setCommandCount NOTIFY commandCountChanged)
    Q_PROPERTY(int undoCommandCount READ undoCommandCount WRITE setUndoCommandCount NOTIFY undoCommandCountChanged)

private:
    ConfigManager();

public:
    static ConfigManager *getIns();

    bool executeCommand(CommandInterface *command);

    Q_INVOKABLE void undo();

    Q_INVOKABLE void redo();

    int commandCount() const;

    int undoCommandCount() const;

    ~ConfigManager() override;

public slots:
    void setCommandCount(int commandCount);

    void setUndoCommandCount(int undoCommandCount);

signals:
    void commandCountChanged(int commandCount);

    void undoCommandCountChanged(int undoCommandCount);

private:
    QStack<CommandInterface *> commandStack;
    QStack<CommandInterface *> undoCommandStack;
    int m_commandCount = 0;
    int m_undoCommandCount = 0;
    RollbackFile configChangedLog;
};

#endif    // CONFIGMANAGER_H
