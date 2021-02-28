#ifndef CLIENT_H
#define CLIENT_H

#include "zmqWrapper/publisher.h"
#include "zmqWrapper/subscriber.h"
#include <QObject>

class client : public QObject
{
    Q_OBJECT
public:
    explicit client(QObject *parent = nullptr) : suber(parent, "!@#"), puber("!@#")
    {
        suber.subscribe("tcp://127.0.0.1:9000");
        puber.bind("tcp://127.0.0.1:8000");
    }

    int add(int x, int y)
    {
        QJsonObject jsonObj;
        jsonObj["name"] = "Add";
        jsonObj["x"] = x;
        jsonObj["y"] = y;
        puber.sendJson(jsonObj);
        QJsonObject res;
        suber.receiveJson(res);
        return res["result"].toInt();
    }

private:
    Subscriber suber;
    Publisher puber;
};

#endif    // CLIENT_H
