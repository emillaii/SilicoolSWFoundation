#include "configmanager.h"

ConfigManager::ConfigManager()
{
    configChangedLog.init("ConfigChangeLogs", "configChangeLog", 1024 * 1024 * 3, 10, true);
}

ConfigManager *ConfigManager::getIns()
{
    static ConfigManager cfgManager;
    return &cfgManager;
}

bool ConfigManager::executeCommand(CommandInterface *command)
{
    if (undoCommandStack.count() > 0)
    {
        while (undoCommandStack.count() > 0)
        {
            delete undoCommandStack.pop();
        }
        setUndoCommandCount(undoCommandStack.count());
    }

    if (command->execute())
    {
        configChangedLog.appendLine(command->getExecutionLog());
        commandStack.push(command);
        setCommandCount(commandStack.count());
        return true;
    }
    else
    {
        return false;
    }
}

void ConfigManager::undo()
{
    if (commandStack.count() > 0)
    {
        CommandInterface *lastCommand = commandStack.pop();
        if (lastCommand->unExecute())
        {
            configChangedLog.appendLine(lastCommand->getUnExecutionLog());
            undoCommandStack.push(lastCommand);
            setCommandCount(commandStack.count());
            setUndoCommandCount(undoCommandStack.count());
        }
        else
        {
            commandStack.push(lastCommand);
        }
    }
}

void ConfigManager::redo()
{
    if (undoCommandStack.count() > 0)
    {
        CommandInterface *lastUndoCommand = undoCommandStack.pop();
        if (lastUndoCommand->execute())
        {
            configChangedLog.appendLine(lastUndoCommand->getExecutionLog());
            commandStack.push(lastUndoCommand);
            setCommandCount(commandStack.count());
            setUndoCommandCount(undoCommandStack.count());
        }
        else
        {
            undoCommandStack.push(lastUndoCommand);
        }
    }
}

int ConfigManager::commandCount() const
{
    return m_commandCount;
}

int ConfigManager::undoCommandCount() const
{
    return m_undoCommandCount;
}

ConfigManager::~ConfigManager()
{
    while (commandStack.count() > 0)
    {
        delete commandStack.pop();
    }
    while (undoCommandStack.count() > 0)
    {
        delete undoCommandStack.pop();
    }
}

void ConfigManager::setCommandCount(int commandCount)
{
    if (m_commandCount == commandCount)
        return;

    m_commandCount = commandCount;
    emit commandCountChanged(m_commandCount);
}

void ConfigManager::setUndoCommandCount(int undoCommandCount)
{
    if (m_undoCommandCount == undoCommandCount)
        return;

    m_undoCommandCount = undoCommandCount;
    emit undoCommandCountChanged(m_undoCommandCount);
}
