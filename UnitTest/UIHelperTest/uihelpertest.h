#ifndef UIHELPERTEST_H
#define UIHELPERTEST_H

#include "uiHelper/uioperation.h"
#include <QDateTime>
#include <QThread>

class UIHelperTest : public QObject
{
    Q_OBJECT

public slots:
    void showRandanMsgBox()
    {
        qDebug("showRandanMsgBox thread id: %d", QThread::currentThreadId());
        UIOperation::getIns()->showMessage("testShow", QDateTime::currentDateTime().toString(), getRandonIcon(), getRandonButtons());
    }

    void getRandanUIRsp()
    {
        qDebug("getRandanUIRsp thread id: %d", QThread::currentThreadId());
        QString rsp = UIOperation::getIns()->getUIResponse("testShow", QDateTime::currentDateTime().toString(), getRandonIcon(), getRandonButtons());
        qDebug(rsp.toUtf8());
    }

private:
    MsgBoxIcon::Icon getRandonIcon()
    {
        return static_cast<MsgBoxIcon::Icon>(rand() % 4);
    }

    QList<QString> getRandonButtons()
    {
        QList<QString> buttons;
        buttons.append(QString::number(rand()));
        for (int i = 0; i < 5; i++)
        {
            if (rand() % 4 == 1)
            {
                buttons.append(QString::number(rand()));
            }
        }
        return buttons;
    }
};

class UIHelperThreadTest : public QObject
{
    Q_OBJECT

public:
    UIHelperThreadTest()
    {
        uiHelper.moveToThread(&thd);
        thd.start();
        connect(this, &UIHelperThreadTest::showRandanMsgBoxSig, &uiHelper, &UIHelperTest::showRandanMsgBox);
        connect(this, &UIHelperThreadTest::getRandanUIRspSig, &uiHelper, &UIHelperTest::getRandanUIRsp);
    }

    Q_INVOKABLE void showRandanMsgBox()
    {
        emit showRandanMsgBoxSig();
    }

    Q_INVOKABLE void getRandanUIRsp()
    {
        emit getRandanUIRspSig();
    }

signals:
    void showRandanMsgBoxSig();
    void getRandanUIRspSig();

private:
    QThread thd;
    UIHelperTest uiHelper;
};

#endif    // UIHELPERTEST_H
