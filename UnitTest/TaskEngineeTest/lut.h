#ifndef LUT_H
#define LUT_H

#include "loghelper.h"
#include "silicolerror.h"
#include <QObject>
#include <QThread>

class Lut : public QObject
{
    Q_OBJECT
public:
    explicit Lut(QObject *parent = nullptr);

    Q_INVOKABLE void moveToLoadLensPos(int timeout = 0)
    {
        qDebug(FUNC_ARG(timeout));
        QThread::sleep(1);
        qDebug("end moveToLoadLensPos");
    }

    Q_INVOKABLE void moveToAAPos()
    {
        qDebug(FUNC);
        QThread::sleep(1);
        qDebug("end moveToAAPos");
    }

    Q_INVOKABLE void openVacuum()
    {
        qDebug(FUNC);
        throw SilicolAbort("Open vacuum failed!");
    }
signals:

public slots:
};

#endif    // LUT_H
