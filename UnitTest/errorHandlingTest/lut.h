#ifndef LUT_H
#define LUT_H

#include "axis.h"
#include "common.h"
#include "lenstray.h"
#include "loghelper.h"
#include "pickdutfromtrayerror.h"
#include "pr.h"
#include "vacuum.h"
#include <QObject>
#include <qthread.h>

class Lut : public QObject
{
    Q_OBJECT
public:
    explicit Lut(QObject *parent = nullptr) {}

signals:

public slots:
    void moveToLoadPos()
    {
        qDebug(FUNC);
        QThread::msleep(myRand(500, 700));
    }

    Vacuum &lutVacuum()
    {
        return vacuum;
    }

private:
    Vacuum vacuum;
};

#endif    // LUT_H
