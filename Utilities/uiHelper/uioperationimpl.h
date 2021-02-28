#ifndef UIOPERATIONIMPL_H
#define UIOPERATIONIMPL_H

#include "TaskEngine/instructionexecutionserver.h"
#include "btnnamedefinition.h"
#include "commonmethod.h"
#include "msgboxmodel.h"
#include "utilities_global.h"
#include <QObject>

class UTILITIESSHARED_EXPORT UIOperationImpl : public QObject
{
    Q_OBJECT
private:
    UIOperationImpl() {}

public:
    static UIOperationImpl &getIns()
    {
        static UIOperationImpl instance;
        return instance;
    }

    MsgBoxModel *msgBoxModel()
    {
        return &m_msgBoxModel;
    }

public slots:
    void onUIResponse(QString uuid, QString clickedButton);

    void showMessageBox(QString uuid, QString title, QString content, int icon, QList<QVariant> buttons);

private:
    MsgBoxModel m_msgBoxModel;
};

#endif    // UIOPERATIONIMPL_H
