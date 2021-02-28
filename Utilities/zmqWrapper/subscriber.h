#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "../commonmethod.h"
#include "../utilities_global.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "zmq.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QThread>

typedef std::function<void(QJsonObject)> JsonMessageHandler;

class UTILITIESSHARED_EXPORT Subscriber : public QThread
{
    Q_OBJECT

public:
    explicit Subscriber(QObject *parent,
                        QString endMark,
                        QString messagePrefix = "",
                        int receiveTimeout = 100,
                        int bufferLen = 1024 * 100);

    void subscribe(QString address);

    void installMessageHandle(JsonMessageHandler msgHandle);

    void clearBuffer();

    bool receiveJson(QJsonObject &jsonObj, int maxRetry = 10);

    void startListenMessage();

    void stopListenMessage();

    virtual ~Subscriber() override;

    // QThread interface
protected:
    virtual void run() override;

private:
    QByteArray endMark;
    char *endMarkData;
    int endMarkDataLen;

    int bufferLen;
    char *buffer;
    int validDataLen = 0;
    int lastCompareIndex = -1;

    bool isRun = false;
    QMutex locker;
    zmq::context_t *ctx = nullptr;
    zmq::socket_t *subscriber = nullptr;
    JsonMessageHandler messageHandler = nullptr;
};
#endif    // SUBSCRIBER_H
