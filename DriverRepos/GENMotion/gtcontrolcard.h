#ifndef GTCONTROLCARD_H
#define GTCONTROLCARD_H

#include "BasicElement/controlcard.h"
#include "configManager/configfile.h"
#include "gtcontrolcardconfig.h"
#include "gtresulthandler.h"
#include <QFile>

class GTControlCard : public ControlCard
{
    Q_OBJECT

public:
    GTControlCard(QObject *parent = nullptr);

    // ControlCard interface
public:
    virtual void preInit() override {}
    virtual void postInit() override;
    virtual void dispose() override;

private:
    const short EthercatCore = 1;
    bool isOpen = false;
    bool isInitEcat = false;
};

#endif    // GTCONTROLCARD_H
