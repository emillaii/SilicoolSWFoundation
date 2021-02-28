#include "uioperationimpl.h"

void UIOperationImpl::onUIResponse(QString uuid, QString clickedButton)
{
    m_msgBoxModel.removeMsgBox(uuid);
    emit InstructionExecutionServer::getIns().uiResponse(uuid, clickedButton);
}

void UIOperationImpl::showMessageBox(QString uuid, QString title, QString content, int icon, QList<QVariant> buttons)
{
    MsgBoxIcon::Icon ico = static_cast<MsgBoxIcon::Icon>(icon);
    QList<QString> buttonNames = unpackVariantList<QString>(buttons);
    m_msgBoxModel.addMsgBox(MsgBoxItem(uuid, title, ico, content, buttonNames));
}
