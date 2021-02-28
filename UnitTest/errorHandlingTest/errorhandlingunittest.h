//#ifndef ERRORHANDLINGUNITTEST_H
//#define ERRORHANDLINGUNITTEST_H

//#include "../loghelper.h"
//#include "actionerror.h"
//#include "pickdutfromjigerror.h"
//#include "picktrayduterror.h"
//#include "placedut2jigerror.h"
//#include "placedut2trayerror.h"
//#include <QObject>
//#include <QThread>
//#include <QtConcurrent>

// class CommonErrorHandler
//{
// public:
//    static ActionErrorHandler actionErrorHandler;
//};

// class Worker : public QObject
//{
//    Q_OBJECT

//    // QObject interface
// public:
//    virtual bool event(QEvent *event) override
//    {
//        try
//        {
//            return QObject::event(event);
//        }
//        catch (SilicolAbort &sa)
//        {
//            qWarning(sa.what());
//            return true;
//        }
//    }
//};

// class Axis : public Worker
//{
//    Q_OBJECT

// public slots:
//    void moveTo(double targetPos)
//    {
//        CommonErrorHandler::actionErrorHandler.handleError([=]() {
//            qDebug(FUNC_ARG(targetPos));

//            QThread::msleep(rand() % 200 + 200);
//            if (rand() % 10 == 2)
//            {
//                throw ActionError("Axis", QString("Move to pos %1 timeout").arg(targetPos));
//            }
//        });
//    }
//};

// class Cylinder : public Worker
//{
//    Q_OBJECT

// public slots:
//    void set(bool status)
//    {
//        CommonErrorHandler::actionErrorHandler.handleError([=]() {
//            qDebug(FUNC_ARG(status));

//            QThread::msleep(rand() % 200 + 200);
//            if (rand() % 10 == 2)
//            {
//                throw ActionError("Cylinder", QString("Set cylinder to %1 timeout").arg(status));
//            }
//        });
//    }
//};

// class PROffset
//{
// public:
//    void reset()
//    {
//        x = 0;
//        y = 0;
//    }

//    double x;
//    double y;
//};

// class PR
//{
// public:
//    bool executePR(PROffset &offset)
//    {
//        offset.reset();
//        if (rand() % 10 != 3)
//        {
//            offset.x = (rand() % 10000) / 3000.0;
//            offset.y = (rand() % 10000) / 3000.0;
//            return true;
//        }
//        return false;
//    }
//};

// class Position
//{
// public:
//    Position() {}

//    Position(double x, double y, double z) : x(x), y(y), z(z) {}

//    Position appendOffset(PROffset offset)
//    {
//        Position newPos;
//        newPos.x = this->x + offset.x;
//        newPos.y = this->y + offset.y;
//        newPos.z = this->z;
//        return newPos;
//    }

//    double x;
//    double y;
//    double z;
//};

// class Vacuum
//{
// public:
//    void set(bool status) {}

//    bool checkStatus(bool targetStatus)
//    {
//        return rand() % 10 != 5;
//    }
//};

// class TrayLoader : public QObject
//{
//    Q_OBJECT

// public slots:
//    void moveToNextDutPos()
//    {
//        qDebug(FUNC);
//    }

//    void changeTray()
//    {
//        qDebug(FUNC);
//    }

//    void setCurrentDutNg() {}
//};

// class PickArm : public Worker
//{
//    Q_OBJECT

// public slots:
//    void test()
//    {
//        QtConcurrent::run(this, &PickArm::pickDutFromTray);
//        QtConcurrent::run([this]() { this->pickDutFromTray(); });
//    }

//    void pickDutFromTray()
//    {
//        moveToPos(trayPrPos);
//        PROffset offset;
//        if (!pr.executePR(offset))
//        {
//            throw PickTrayDutError("PikArm", "tray dut pr fail");
//        }
//        moveToPos(trayPrPos.appendOffset(offset));
//        axisZ.moveTo(10);
//        vacuum.set(true);
//        axisZ.moveTo(0);
//        if (!vacuum.checkStatus(true))
//        {
//            throw PickTrayDutError("PikArm", "Pick dut fail");
//        }
//    }

//    void placeDutToJig()
//    {
//        moveToPos(jigPrPos);
//        PROffset offset;
//        if (!pr.executePR(offset))
//        {
//            throw PlaceDut2JigError("PickArm", "jig pr failed");
//        }
//        moveToPos(jigPrPos.appendOffset(offset));
//        axisZ.moveTo(10);
//        vacuum.set(false);
//    }

//    void moveZUp()
//    {
//        axisZ.moveTo(0);
//    }

// private:
//    void moveToPos(Position pos)
//    {
//        axisX.moveTo(pos.x);
//        axisY.moveTo(pos.y);
//        axisZ.moveTo(pos.z);
//    }

// private:
//    Axis axisX;
//    Axis axisY;
//    Axis axisZ;
//    Cylinder pogpin;
//    PR pr;
//    Vacuum vacuum;

//    Position trayPrPos;
//    Position jigPrPos;
//    Position placeDut2ToTrayPos;
//    Position pickDutFromJigPos;
//};

// class PickArmState
//{
// public:
//    bool hasPickedDut;
//    bool hasPickedNgDut;
//};

// class SUTState
//{
// public:
//    bool hasDut;
//    bool hasNgDut;
//};

// class SUT : public Worker
//{
// public:
//    void moveToLoadPos()
//    {
//        moveToPos(loadDutPos);
//    }

//    bool checkDut(bool targetStatus)
//    {
//        return rand() % 10 == 3;
//    }

//    void setVacuum(bool targetStatus)
//    {
//        vacuum.set(targetStatus);
//    }

// private:
//    void moveToPos(Position pos)
//    {
//        axisX.moveTo(pos.x);
//        axisY.moveTo(pos.y);
//        axisZ.moveTo(pos.z);
//    }

// private:
//    Position loadDutPos;

//    Cylinder pogpin;
//    Vacuum vacuum;
//    Axis axisX;
//    Axis axisY;
//    Axis axisZ;
//};

// class DUTLoader : public Worker
//{
//    Q_OBJECT
// public:
//    DUTLoader()
//        : pickTrayDutErrorHandler([this]() {
//              trayLoader.setCurrentDutNg();
//              qDebug("skip");
//          }),
//          placeDut2JigErrorHandler(
//              [this]() {
//                  pickArmStates.hasPickedDut = false;
//                  sutStates.hasDut = false;
//              },
//              [this]() {
//                  pickArmStates.hasPickedDut = false;
//                  sutStates.hasDut = true;
//              })
//    {
//        pickTrayDutErrorHandler.setHandlingContinuousFailure(5, [this]() {
//            if (YesBtn
//                == UIOperation::getIns()->getUIResponse(
//                    "Pick dut fail", QString("Pick dut continuous failed 5 times, do you want to change tray?"),
//                    MsgBoxIcon::Question, YesNoBtns))
//            {
//                trayLoader.changeTray();
//            }
//            else
//            {
//            }
//        });
//    }

// public slots:
//    void pickDutFromTray()
//    {
//        pickTrayDutErrorHandler.handleError([this]() {
//            trayLoader.moveToNextDutPos();
//            pickArm.pickDutFromTray();
//            pickArmStates.hasPickedDut = true;
//        });
//    }

//    void placeDut2Jig()
//    {
//        placeDut2JigErrorHandler.handleError([this]() {
//            sut.moveToLoadPos();
//            pickArm.placeDutToJig();
//            sut.setVacuum(true);
//            pickArm.moveZUp();
//            if (!sut.checkDut(true))
//            {
//                throw PlaceDut2JigError("sut", "check dut failed");
//            }
//            pickArmStates.hasPickedDut = false;
//            sutStates.hasDut = true;
//        });
//    }

// public:
//    SUT sut;
//    TrayLoader trayLoader;
//    PickArm pickArm;

// private:
//    SUTState sutStates;
//    PickArmState pickArmStates;

//    PickTrayDutErrorHandler pickTrayDutErrorHandler;
//    PlaceDut2JigErrorHandler placeDut2JigErrorHandler;
//};

//#endif    // ERRORHANDLINGUNITTEST_H
