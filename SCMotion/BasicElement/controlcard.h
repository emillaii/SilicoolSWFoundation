#ifndef CONTROLCARD_H
#define CONTROLCARD_H

#include "scmotion_global.h"
#include <QObject>

class SCMOTIONSHARED_EXPORT ControlCard : public QObject
{
    Q_OBJECT
public:
    ControlCard(QObject *parent = nullptr) : QObject(parent) {}
    virtual void preInit() = 0;
    virtual void postInit() = 0;
    virtual void dispose() = 0;
};

#endif    // CONTROLCARD_H
