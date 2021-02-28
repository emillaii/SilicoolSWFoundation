#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "zmq.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

class UTILITIESSHARED_EXPORT Publisher : public QObject
{
    Q_OBJECT

public:
    Publisher(QString msgEndMark, QString msgPrefix = "", QObject *parent = nullptr);

    void bind(QString address);

    void sendJson(const QJsonObject &jsonObj);

    ~Publisher();

private:
    QByteArray msgPrefix;
    QByteArray msgEndMark;
    QMutex locker;
    zmq::context_t *ctx = nullptr;
    zmq::socket_t *publisher = nullptr;
};

#endif    // PUBLISHER_H
