#ifndef XYZRDEBUGGER_H
#define XYZRDEBUGGER_H

#include "MotionManager/motionmanager.h"
#include "xyzrdebuggerconfig.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT XYZRDebugger : public QObject
{
    Q_OBJECT
public:
    explicit XYZRDebugger(XYZRDebuggerConfig *config, QObject *parent = nullptr);

    ~XYZRDebugger();

public slots:
    void stepMove(QString axisType, double step);
    void jogMove(QString axisType, int dir, double velocity);
    void stopJogMove(QString axisType);
    void startUpdateAxesPos();
    void stopUpdateAxesPos();
    QPointF getCurrentXYPos();

signals:
    void axesPosPublished(double x, double y, double z, double r);

private slots:
    void updateAxesPos();

private:
    QString getAxisName(QString axisType);
    bool haveToReverseDir(QString axisType);
    double getAxisCurrentPos(QString axisType, bool isFeedback);

private:
    XYZRDebuggerConfig *m_config;
    QTimer updatePosTimer;
};

#endif    // XYZRDEBUGGER_H
