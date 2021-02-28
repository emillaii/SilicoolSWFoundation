#ifndef LENSLOADER_H
#define LENSLOADER_H

#include "loghelper.h"
#include <QObject>
#include <QThread>

class LensLoader : public QObject
{
    Q_OBJECT
public:
    explicit LensLoader(QObject *parent = nullptr);

    Q_INVOKABLE void moveToLensPos(int x = 0, int y = 0)
    {
        qDebug(FUNC_ARG(x, y));
        QThread::sleep(1);
        qDebug("end moveToLensPos");
    }

    Q_INVOKABLE void pickLens(bool doPr = false)
    {
        qDebug(FUNC_ARG(doPr));
        QThread::sleep(1);
        qDebug("end pickLens");
    }

    Q_INVOKABLE void moveToPlaceLensPos()
    {
        qDebug(FUNC);
        QThread::sleep(1);
        qDebug("end moveToPlaceLensPos");
    }

    Q_INVOKABLE void placeLens()
    {
        qDebug(FUNC);
        QThread::sleep(1);
        qDebug("end placeLens");
    }

signals:

public slots:
};

#endif    // LENSLOADER_H
