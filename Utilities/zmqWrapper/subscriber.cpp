#include "subscriber.h"

Subscriber::Subscriber(QObject *parent, QString endMark, QString messagePrefix, int receiveTimeout, int bufferLen)
    : QThread(parent), bufferLen(bufferLen)
{
    this->endMark = endMark.toUtf8();
    endMarkData = this->endMark.data();
    endMarkDataLen = this->endMark.length();
    buffer = new char[bufferLen];
    memset(buffer, 0, bufferLen);
    ctx = new zmq::context_t;
    subscriber = new zmq::socket_t(*ctx, zmq::socket_type::sub);
    auto msgPrefix = messagePrefix.toUtf8();
    subscriber->setsockopt(ZMQ_SUBSCRIBE, msgPrefix, static_cast<size_t>(msgPrefix.length()));
    subscriber->setsockopt(ZMQ_RCVTIMEO, &receiveTimeout, sizeof(int));
}

void Subscriber::subscribe(QString address)
{
    try
    {
        subscriber->connect(address.toStdString());
    }
    catch (std::exception &ex)
    {
        throw std::exception(QString("Zmq connect to address error! Error message: %1, address: %2")
                                 .arg(ex.what())
                                 .arg(address)
                                 .toStdString()
                                 .c_str());
    }
}

void Subscriber::installMessageHandle(JsonMessageHandler msgHandle)
{
    this->messageHandler = msgHandle;
}

void Subscriber::clearBuffer()
{
    validDataLen = 0;
    lastCompareIndex = -1;
}

bool Subscriber::receiveJson(QJsonObject &jsonObj, int maxRetry)
{
    QMutexLocker tmpLocker(&locker);
    for (int t = 0; t < maxRetry; t++)
    {
        if (validDataLen >= bufferLen)
        {
            qFatal("Did not find end mark: %s, buffer may be too small: %d", endMarkData, bufferLen);
        }
        int receivedSize = subscriber->recv(buffer + validDataLen, bufferLen - validDataLen);
        if (receivedSize > 0)
        {
            validDataLen += receivedSize;
        }
        if (validDataLen > 0)
        {
            for (int i = lastCompareIndex + 1; i <= validDataLen - endMarkDataLen; i++)
            {
                bool findEndMark = true;
                for (int j = 0; j < endMarkDataLen; j++)
                {
                    if (buffer[i + j] != endMarkData[j])
                    {
                        findEndMark = false;
                        break;
                    }
                }
                if (findEndMark)
                {
                    jsonObj = QJsonDocument::fromBinaryData(QByteArray(buffer, i)).object();
                    validDataLen -= (i + endMarkDataLen);
                    if (validDataLen > 0)
                    {
                        memcpy(buffer, buffer + i + endMarkDataLen, validDataLen);
                    }
                    lastCompareIndex = -1;
                    return true;
                }
            }
            lastCompareIndex = validDataLen - endMarkDataLen;
        }
    }
    return false;
}

void Subscriber::startListenMessage()
{
    if (isRun)
    {
        return;
    }
    if (messageHandler == nullptr)
    {
        qCritical("Please installMessageHandle first!");
        return;
    }
    isRun = true;
    start();
}

void Subscriber::stopListenMessage()
{
    if (isRun)
    {
        isRun = false;
        wait();
    }
}

Subscriber::~Subscriber()
{
    stopListenMessage();
    delete[] buffer;
    SILICOOL_DELETE(subscriber);
    SILICOOL_DELETE(ctx);
}

void Subscriber::run()
{
    QJsonObject jsonObj;
    while (isRun)
    {
        if (receiveJson(jsonObj, 8))
        {
            messageHandler(jsonObj);
        }
    }
}
