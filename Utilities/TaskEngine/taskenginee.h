#ifndef TASKENGINEE_H
#define TASKENGINEE_H

#include "instruction.h"
#include "loging/Logger.h"
#include "uiHelper/uioperation.h"
#include <QThread>

class TaskEnginee : public QThread
{
    Q_OBJECT

public:
    explicit TaskEnginee(QObject *parent = nullptr) : QThread(parent) {}

    void clearInstruction()
    {
        asyncInstructions.clear();
    }

    void appendInstruction(const Instruction &instruction)
    {
        asyncInstructions.append(instruction);
    }

    // QThread interface
protected:
    virtual void run() override
    {
        try
        {
            while (asyncInstructions.count() > 0)
            {
                Instruction asyncInstruction = asyncInstructions.takeAt(0);
                asyncInstruction.execute();
            }
        }
        catch (SilicoolException &se)
        {
            UIOperation::getIns()->showMessage("Error", se.what(), MsgBoxIcon::Error, OkBtn);
        }
    }

private:
    QList<Instruction> asyncInstructions;
};

#endif    // TASKENGINEE_H
