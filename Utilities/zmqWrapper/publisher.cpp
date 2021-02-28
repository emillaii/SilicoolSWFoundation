#include "publisher.h"

Publisher::Publisher(QString msgEndMark, QString msgPrefix, QObject *parent) : QObject(parent)
{
    this->msgPrefix = msgPrefix.toUtf8();
    this->msgEndMark = msgEndMark.toUtf8();
    ctx = new zmq::context_t;
    publisher = new zmq::socket_t(*ctx, zmq::socket_type::pub);
}

void Publisher::bind(QString address)
{
    try
    {
        publisher->bind(address.toStdString());
    }
    catch (std::exception &ex)
    {
        throw std::exception(QString("Zmq bind to address error! Error message: %1, address: %2")
                                 .arg(ex.what())
                                 .arg(address)
                                 .toStdString()
                                 .c_str());
    }
}

void Publisher::sendJson(const QJsonObject &jsonObj)
{
    auto buffer = QJsonDocument(jsonObj).toBinaryData() + msgEndMark;
    if (!msgPrefix.isEmpty())
    {
        buffer = msgPrefix + buffer;
    }
    size_t nPublishedBytes = 0;
    size_t bufferLen = buffer.length();
    char *data = buffer.data();
    QMutexLocker tmpLocker(&locker);
    while (nPublishedBytes < bufferLen)
    {
        nPublishedBytes += publisher->send(data + nPublishedBytes, bufferLen - nPublishedBytes);
    }
}

Publisher::~Publisher()
{
    SILICOOL_DELETE(publisher);
    SILICOOL_DELETE(ctx);
}
