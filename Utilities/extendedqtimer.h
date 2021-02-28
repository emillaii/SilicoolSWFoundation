#ifndef EXTENDEDQTIMER_H
#define EXTENDEDQTIMER_H

#include "utilities_global.h"
#include <QObject>
#include <QThread>
#include <QTimer>

class UTILITIESSHARED_EXPORT ExtendedQTimer : public QTimer
{
    Q_OBJECT

public:
    explicit ExtendedQTimer(QObject *parent = nullptr);

public slots:
    void reqStart();
    void reqStart(int msec);
    void reqStop();

signals:
    void startSig(int msec, QPrivateSignal);
    void stopSig(QPrivateSignal);

private slots:
    void onStartReq(int msec);
};

#endif    // EXTENDEDQTIMER_H
