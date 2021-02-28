#ifndef LENSTRAY_H
#define LENSTRAY_H

#include "axis.h"
#include "common.h"
#include "loghelper.h"
#include "vacuum.h"
#include <QObject>
#include <qthread.h>

class LensTray : public QObject
{
    Q_OBJECT
public:
    explicit LensTray(QObject *parent = nullptr) : lensTrayX("LensTrayX"), lensTrayY("LensTrayY") {}

signals:

public slots:
    void changeTray()
    {
        qDebug(FUNC);
        QThread::msleep(myRand(200, 500));
        rowIndex = 0;
        ColumnIndex = 0;
    }

    void moveToNextDutPos()
    {
        qDebug(FUNC);
        rowIndex++;
        if (rowIndex >= nColumn)
        {
            ColumnIndex++;
            rowIndex = 0;
        }
        lensTrayX.moveTo(rowIndex);
        lensTrayY.moveTo(ColumnIndex);
    }

    bool hasGottenLens()
    {
        return lutVacuum.checkState(true);
    }

private:
    Axis lensTrayX;
    Axis lensTrayY;

    Vacuum lutVacuum;

    int rowIndex = 0;
    int ColumnIndex = 0;
    int nColumn;
};

#endif    // LENSTRAY_H
