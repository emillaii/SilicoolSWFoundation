#ifndef STATEMASHINEDEFINITION_H
#define STATEMASHINEDEFINITION_H

#include "commonmethod.h"
#include "enumhelper.h"
#include "utilities_global.h"
#include <QMetaEnum>
#include <QObject>

/// State Machine Definition
class UTILITIESSHARED_EXPORT SMD : public QObject
{
    Q_OBJECT
public:
    enum Event
    {
        Init,
        Start,
        Stop,
        Pause,
        Continue,
        Purge,
        Reset,
        Abort
    };
    Q_ENUM(Event)

    enum State
    {
        Idle,
        Initializing,
        Initialized,
        Purging,
        Ready,
        Running,
        Pausing,
        Paused,
        Stopping,
        Error,
        Aborting,
        Aborted,
        Reseting
    };
    Q_ENUM(State)

    static const EnumHelper<Event> &EventEnumInfo()
    {
        static const EnumHelper<Event> enumInfo(staticMetaObject, "Event");
        return enumInfo;
    }

    static const EnumHelper<State> &StateEnumInfo()
    {
        static const EnumHelper<State> enumInfo(staticMetaObject, "State");
        return enumInfo;
    }
};

#endif    // STATEMASHINEDEFINITION_H
