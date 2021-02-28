#ifndef CALCULATORTEST_H
#define CALCULATORTEST_H

#include "TaskEngine/instruction.h"
#include "calculator.h"
#include <QElapsedTimer>
#include <QObject>
#include <QThread>

class CalculatorTest : public QObject
{
    Q_OBJECT
public:
    explicit CalculatorTest(QObject *parent = nullptr) : QObject(parent)
    {
        calc.moveToThread(&thd);
        thd.start();
    }

signals:
    int add(int x, int y = 0, int z = 0);

public slots:
    void signalSlotQueueTest()
    {
        connect(this, &CalculatorTest::add, &calc, &Calculator::add, Qt::BlockingQueuedConnection);
        QElapsedTimer timer;
        timer.start();
        int sum = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                sum += emit add(i, j);
            }
        }
        qDebug("signalSlotQueueTest, sum: %d, elapsed time: %d", sum, timer.elapsed());
        disconnect(this, &CalculatorTest::add, &calc, &Calculator::add);
    }

    void signalSlotDirectTest()
    {
        connect(this, &CalculatorTest::add, &calc, &Calculator::add, Qt::DirectConnection);
        QElapsedTimer timer;
        timer.start();
        int sum = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                sum += emit add(i, j);
            }
        }
        qDebug("signalSlotDirectTest, sum: %d, elapsed time: %d", sum, timer.elapsed());
        disconnect(this, &CalculatorTest::add, &calc, &Calculator::add);
    }

    void invokeTest()
    {
        QElapsedTimer timer;
        timer.start();
        int sum = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                QVariantList args;
                args << i << j;
                Instruction ins(&calc, "add", args);
                sum += ins.execute<int>();
            }
        }
        qDebug("invoke test, sum: %d, elapsed time: %d", sum, timer.elapsed());
    }

private:
    Calculator calc;
    QThread thd;
};

#endif    // CALCULATORTEST_H
