#ifndef MOTIONSTATE_H
#define MOTIONSTATE_H

#include <QDataStream>
#include <QMap>

class AxisState
{
    Q_GADGET

public:
    bool isEnabled;
    bool hasHome;
    double position;
    bool hasAlarm;
    bool isRunning;
    bool jogMovable;
};

inline QDataStream &operator<<(QDataStream &ds, const AxisState &obj)
{
    ds << obj.isEnabled << obj.hasHome << obj.position << obj.hasAlarm << obj.isRunning << obj.jogMovable;
    return ds;
}

inline QDataStream &operator>>(QDataStream &ds, AxisState &obj)
{
    ds >> obj.isEnabled >> obj.hasHome >> obj.position >> obj.hasAlarm >> obj.isRunning >> obj.jogMovable;
    return ds;
}

class MotionState
{
    Q_GADGET

public:
    QMap<QString, bool> diStates;
    QMap<QString, bool> doStates;
    QMap<QString, bool> vacuumStates;
    QMap<QString, int> cylStates;
    QMap<QString, AxisState> axisStates;
};

inline QDataStream &operator<<(QDataStream &ds, const MotionState &obj)
{
    ds << obj.diStates << obj.doStates << obj.vacuumStates << obj.cylStates << obj.axisStates;
    return ds;
}

inline QDataStream &operator>>(QDataStream &ds, MotionState &obj)
{
    ds >> obj.diStates >> obj.doStates >> obj.vacuumStates >> obj.cylStates >> obj.axisStates;
    return ds;
}

#endif    // MOTIONSTATE_H
