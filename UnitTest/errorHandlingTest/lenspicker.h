#ifndef LENSPICKER_H
#define LENSPICKER_H

#include "axis.h"
#include "common.h"
#include "lenstray.h"
#include "loghelper.h"
#include "lut.h"
#include "pickdutfromtrayerror.h"
#include "placedut2jigerror.h"
#include "pr.h"
#include "vacuum.h"
#include <QObject>
#include <qthread.h>

class LensPicker : public QObject
{
    Q_OBJECT
public:
    explicit LensPicker(QObject *parent = nullptr)
        : lpx("lpx"),
          lpy("lpy"),
          lpz("lpz"),
          lpTheta("lpTheta"),
          pdfteh(std::bind(&LensPicker::pickNewLens, this)),
          pd2jeh([] { qDebug("//lut.setHasLens(false);lensPicker.setPickedLens(false);"); },
                 [this] { return lut.lutVacuum().checkState(false) && vacuum.checkState(false); },
                 [] { qDebug("//lut.setHasLens(true);lensPicker.setPickedLens(false);"); },
                 [this] { return lut.lutVacuum().checkState(true) && vacuum.checkState(false); })
    {
        pdfteh.setHandlingContinuousFailure(3, [this] {
            lensTray.changeTray();
            pickNewLens();
        });
    }

signals:

public slots:
    void moveToTrayLensPos()
    {
        qDebug(FUNC);
        lpz.moveTo(0);
        lpx.moveTo(1);
        lpy.moveTo(1);
    }

    void pickTrayLens()
    {
        qDebug(FUNC);
        pdfteh.tryToHandleError([this] {
            PROffset offset;
            if (!trayLensPr.execute(offset))
            {
                throw PickDutFromTrayError("PR", "tray lens pr failed", true);
            }
            lpx.stepMove(offset.x);
            lpy.stepMove(offset.y);
            lpz.moveTo(10);
            vacuum.set(true);
            lpz.moveTo(0);
            if (!vacuum.checkState(true))
            {
                throw PickDutFromTrayError("Vacuum", "Check dut failed");
            }
        });
    }

    void placeLensToTray() {}

    void moveToLutLensPos()
    {
        qDebug(FUNC);
        lpz.moveTo(0);
        lpx.moveTo(3);
        lpy.moveTo(3);
    }

    void placeLensToLut()
    {
        qDebug(FUNC);
        pd2jeh.tryToHandleError([this] {
            lut.moveToLoadPos();
            moveToLutLensPos();
            lpz.moveTo(10);
            lut.lutVacuum().set(true);
            lpz.moveTo(0);
            if (!lut.lutVacuum().checkState(true))
            {
                throw PlaceDut2JigError("Lut", "Check lut lens failed!");
            }
        });
    }

    void pickLutLens() {}

    void pickNewLens()
    {
        qDebug(FUNC);
        lensTray.moveToNextDutPos();
        moveToTrayLensPos();
        pickTrayLens();
    }

private:
    PR trayLensPr;
    PR lutLensPr;
    Axis lpx;
    Axis lpy;
    Axis lpz;
    Axis lpTheta;
    Vacuum vacuum;
    PickDurFromTrayErrorHandler pdfteh;
    PlaceDut2JigErrorHandler pd2jeh;
    LensTray lensTray;
    Lut lut;
};

#endif    // LENSPICKER_H
