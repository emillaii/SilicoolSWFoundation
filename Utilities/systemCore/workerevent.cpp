#include "workerevent.h"

WorkerEvent::WorkerEvent()
{
    priority = 0;
    isFrameworkEvent = false;
}

WorkerEvent::WorkerEvent(SMD::Event event, const QVariant &eventArgs) : eventArgs(eventArgs), isFrameworkEvent(true)
{
    priority = getEventPriority(event);
    eventName = SMD::EventEnumInfo().enumToName(event);
}

WorkerEvent::WorkerEvent(const QString &eventName, const QVariant &eventArgs, int priority)
    : priority(priority), eventName(eventName), eventArgs(eventArgs), isFrameworkEvent(false)
{
}

int WorkerEvent::getEventPriority(SMD::Event event)
{
    static QMap<SMD::Event, int> eventPriority;
    if (eventPriority.isEmpty())
    {
        eventPriority[SMD::Init] = INT_MAX - 1;
        eventPriority[SMD::Start] = INT_MAX - 1;
        eventPriority[SMD::Stop] = INT_MAX - 1;
        eventPriority[SMD::Pause] = INT_MAX - 1;
        eventPriority[SMD::Continue] = INT_MAX - 1;
        eventPriority[SMD::Purge] = INT_MAX - 1;
        eventPriority[SMD::Reset] = INT_MAX - 1;
        eventPriority[SMD::Abort] = INT_MAX;
    }
    return eventPriority[event];
}

bool operator<=(const WorkerEvent &event1, const WorkerEvent &event2)
{
    return event1.priority <= event2.priority;
}

QDebug operator<<(QDebug dbg, const WorkerEvent &event)
{
    dbg << "EventName:" << event.eventName << "Priority:" << event.priority;
    if (event.eventArgs.isValid())
    {
        dbg << "Arg:" << event.eventArgs;
    }
    return dbg;
}
