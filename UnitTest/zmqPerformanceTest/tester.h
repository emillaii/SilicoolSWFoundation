#ifndef TESTER_H
#define TESTER_H

#include "client.h"
#include "server.h"
#include <QObject>

class Tester : public QObject
{
    Q_OBJECT
public:
    explicit Tester(QObject *parent = nullptr);

signals:

public slots:

    void test()
    {
        QElapsedTimer timer;
        timer.start();
        int sum = 0;
        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                sum += client.add(i, j);
            }
        }
        qDebug() << "sum" << sum << "timecost" << timer.elapsed();
    }

private:
    Server server;
    client client;
};

#endif    // TESTER_H
