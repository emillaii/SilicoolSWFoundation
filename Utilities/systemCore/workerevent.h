#ifndef WORKEREVENT_H
#define WORKEREVENT_H

#include "enumhelper.h"
#include "statemashinedefinition.h"
#include "utilities_global.h"
#include <QDebug>
#include <QObject>
#include <QVariant>

struct UTILITIESSHARED_EXPORT WorkerEvent
{
public:
    WorkerEvent();

    WorkerEvent(const QString &eventName, const QVariant &eventArgs = QVariant(), int priority = 1000);
    WorkerEvent(SMD::Event event, const QVariant &eventArgs = QVariant());

    int priority;
    QString eventName;
    QVariant eventArgs;
    bool isFrameworkEvent;

private:
    static int getEventPriority(SMD::Event event);
};
Q_DECLARE_METATYPE(WorkerEvent)

bool UTILITIESSHARED_EXPORT operator<=(const WorkerEvent &event1, const WorkerEvent &event2);

QDebug operator<<(QDebug dbg, const WorkerEvent &event);

#endif    // WORKEREVENT_H
