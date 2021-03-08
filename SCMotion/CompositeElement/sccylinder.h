#ifndef SCCYLINDER_H
#define SCCYLINDER_H

#include "BasicElement/globalstate.h"
#include "BasicElement/scdi.h"
#include "BasicElement/scdo.h"
#include "MotionManager/motionelementcontainer.h"
#include "MoveProtection/moveprotection.h"
#include "QDataStream"
#include "cylinderconfig.h"
#include "errorHandling/silicolerror.h"
#include "loghelper.h"
#include "motionutility.h"
#include "scmotion_global.h"
#include <QThread>

class SCMOTIONSHARED_EXPORT SCCylinder : public QObject
{
    Q_OBJECT
public:
    enum CylinderState
    {
        False = 0,
        True = 1,
        Moving = 2
    };
    Q_ENUM(CylinderState)

    explicit SCCylinder(QString name, QObject *parent = nullptr);

    static MotionElement::Type elementType()
    {
        return MotionElement::Cylinder;
    }

    QString name() const
    {
        return objectName();
    }

    void setConfig(CylinderConfig *config);

    void init();

    void setMoveProtection(MoveProtection *moveProtection)
    {
        this->moveProtection = moveProtection;
    }

    Q_INVOKABLE CylinderState get();

    Q_INVOKABLE void set(bool state, bool waitDone = true);

    void wait(bool state);

    // just for CylinderDebugger UI element calling
    QVariantMap getDetailState();

    Q_INVOKABLE void setFalseOutput(bool state);

    Q_INVOKABLE void setTrueOutput(bool state);

    CylinderState getOutputState();
    CylinderState getInputState();

    Q_INVOKABLE void startLoopTest();
    Q_INVOKABLE void stopLoopTest();

private:
    bool checkState(bool targetState);
    void setImpl(bool state);

private:
    MoveProtection *moveProtection = nullptr;
    bool isSingleOutputControl;
    bool noFalseInputSNR;
    bool noTrueInputSNR;
    CylinderConfig *m_config = nullptr;
    SCDI *falseInput = nullptr;
    SCDI *trueInput = nullptr;
    SCDO *falseOutput = nullptr;
    SCDO *trueOutput = nullptr;
    bool isLoopTest = false;
};

#endif    // SCCYLINDER_H
