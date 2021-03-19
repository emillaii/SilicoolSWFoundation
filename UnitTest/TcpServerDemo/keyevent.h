#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <QObject>
#include <QThread>
#include <stdio.h>

class KeyEvent : public QThread
{
    Q_OBJECT
public:
    explicit KeyEvent(QObject *parent = nullptr) : QThread(parent) {}

signals:
    void initServer();
    void disposeServer();
    void closeProgram();

protected:
    void run();
};

#endif    // KEYEVENT_H
