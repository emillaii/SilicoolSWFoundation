import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Popup {
    id: popupChangePassword
    x: 0
    y: 0
    width: 350
    height: 250
    closePolicy: Popup.NoAutoClose

    function clearText(){
        txtOldPassword.clear()
        txtNewPassword.clear()
        txtRepeatPassword.clear()
    }

    contentItem: ColumnLayout{
        anchors.centerIn: parent

        GridLayout{
            rows: 4
            columns: 2
            Label{
                text: qsTr("UserName:")
                horizontalAlignment: Text.AlignRight
            }
            TextField{
                id: txtUserName
                Layout.fillWidth: true
                text: basicConfig.lastInputUser
                selectByMouse: true
                onEditingFinished: {
                    basicConfig.setLastInputUser(text)
                }
            }
            Label{
                text: qsTr("OldPassword:")
                horizontalAlignment: Text.AlignRight
            }
            TextField{
                id: txtOldPassword
                echoMode: TextInput.Password
                selectByMouse: true
                Layout.fillWidth: true
            }
            Label{
                text: qsTr("NewPassword:")
                horizontalAlignment: Text.AlignRight
            }
            TextField{
                id: txtNewPassword
                echoMode: TextInput.Password
                Layout.fillWidth: true
                selectByMouse: true
            }
            Label{
                text: qsTr("RepeatPassword:")
                horizontalAlignment: Text.AlignRight
            }
            TextField{
                id: txtRepeatPassword
                echoMode: TextInput.Password
                selectByMouse: true
                Layout.fillWidth: true
            }
        }

        RowLayout{
            Button{
                id: btnOK
                text: qsTr("Ok")
                onClicked: {
                    if(userManagement.changePassword(txtUserName.text, txtOldPassword.text,
                                                     txtNewPassword.text, txtRepeatPassword.text))
                    {
                        popupChangePassword.close()
                    }
                }
            }
            Button{
                id: btnCancel
                text: qsTr("Cancel")
                onClicked: {
                    popupChangePassword.close()
                }
            }
        }

        Keys.onEscapePressed: {
            btnCancel.clicked()
        }
        Keys.onReturnPressed: {
            btnOK.clicked()
        }
    }
}


