#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDebug>
#include <QObject>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = nullptr);

signals:

public slots:
    int add(double x, double y = 0, double z = 0)
    {
        return x + y + z;
    }
};

#endif    // CALCULATOR_H
