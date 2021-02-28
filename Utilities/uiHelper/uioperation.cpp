#include "uioperation.h"

UIOperation::UIOperation() {}

void UIOperation::setGrabMouseItem(QQuickItem *item)
{
    grabMouseItem = item;
}

void UIOperation::init(QString uiReqReceiverObjName)
{
    this->uiReqReceiverObjName = uiReqReceiverObjName;
    if (!isInit)
    {
        connect(InstructionExecutionClient::getIns().getIeReplica(), &InstructionExecutorReplica::uiResponse, this, &UIOperation::onUIResponse);
        isInit = true;
    }
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    getLogger(icon) << QString("showMessage, title: %1, content: %2").arg(title).arg(content);

    QVariantList args;
    args << QUuid::createUuid().toString(QUuid::Id128) << title << content << icon << QVariant::fromValue(toVariantList(buttons));
    InstructionExecutionClient::getIns().executeInstruction(uiReqReceiverObjName, "showMessageBox", args);
}

void UIOperation::showMessage(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    showMessage(title, content, icon, button.split(BtnSeperator));
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QList<QString> buttons)
{
    auto logger = getLogger(icon);
    logger << QString("getUIResponse, title: %1, content: %2, buttons: %3").arg(title).arg(content).arg(combineString("|", buttons));

    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    UIResponse *response = new UIResponse;
    uiRsp[uuid] = response;
    QVariantList args;
    args << uuid << title << content << icon << QVariant::fromValue(toVariantList(buttons));
    QString instructionId = InstructionExecutionClient::getIns().executeInstruction(uiReqReceiverObjName, "showMessageBox", args);

    resetMouse();

    silicoolWait(
        -1, [&response] { return response->gotResponse; }, 20);

    QString clickedButton = response->clickedButton;
    uiRsp.remove(uuid);
    delete response;

    logger << QString("getUIResponse, user clicked button: %1").arg(clickedButton);
    return clickedButton;
}

QString UIOperation::getUIResponse(QString title, QString content, MsgBoxIcon::Icon icon, QString button)
{
    return getUIResponse(title, content, icon, button.split(BtnSeperator));
}

void UIOperation::showTip(QString tipMsg)
{
    showMessage("Tip", tipMsg, MsgBoxIcon::Information, OkBtn);
}

void UIOperation::showError(QString errMsg)
{
    showMessage("Error", errMsg, MsgBoxIcon::Error, OkBtn);
}

bool UIOperation::yesNoConfirm(QString content)
{
    auto rsp = getUIResponse("Confirm", content, MsgBoxIcon::Question, YesNoBtns);
    return rsp == YesBtn;
}

bool UIOperation::okCancelConfirm(QString content)
{
    auto rsp = getUIResponse("Confirm", content, MsgBoxIcon::Question, OkCancelBtns);
    return rsp == OkBtn;
}

void UIOperation::onUIResponse(QString uuid, QString clickedButton)
{
    if (uiRsp.contains(uuid))
    {
        uiRsp[uuid]->clickedButton = clickedButton;
        uiRsp[uuid]->gotResponse = true;
    }
}

QDebug UIOperation::getLogger(MsgBoxIcon::Icon icon)
{
    switch (icon)
    {
        case MsgBoxIcon::Error:
            return qCritical();
        case MsgBoxIcon::Warning:
            return qWarning();
        default:
            return qInfo();
    }
}

void UIOperation::resetMouse()
{
    if (isInMainThread())
    {
        if (grabMouseItem)
        {
            grabMouseItem->grabMouse();
            grabMouseItem->ungrabMouse();
        }
    }
}
