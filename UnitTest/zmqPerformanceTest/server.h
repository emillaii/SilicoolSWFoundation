#ifndef SERVER_H
#define SERVER_H

#include "zmqWrapper/publisher.h"
#include "zmqWrapper/subscriber.h"
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr) : suber(parent, "!@#"), puber("!@#")
    {
        suber.subscribe("tcp://127.0.0.1:8000");
        puber.bind("tcp://127.0.0.1:9000");
        suber.installMessageHandle(std::bind(&Server::msgHandle, this, std::placeholders::_1));
        suber.startListenMessage();
    }

    void msgHandle(QJsonObject jsonObj)
    {
        if (jsonObj["name"].toString() == "Add")
        {
            QJsonObject res;
            res["result"] = jsonObj["x"].toInt() + jsonObj["y"].toInt();
            puber.sendJson(res);
        }
    }

public slots:

private:
    Subscriber suber;
    Publisher puber;
};

#endif    // SERVER_H
