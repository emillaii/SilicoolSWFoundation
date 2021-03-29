import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

ColumnLayout{
    function clearText()
    {
        txtPassword.clear()
    }

    ChangePassword{
        id: popupChangePsw
    }

    signal closePopup()

    property bool cancelButtonVisible: true
    property bool changePswButtonVisible: true

    GridLayout{
        rows: 2
        columns: 2
        Label{
            text: qsTr("UserName:")
            horizontalAlignment: Text.AlignRight
        }
        ComboBox{
            id: txtUserName

            property var isInit: false
            editable: true
            Layout.fillWidth: true
            model: userManagement.userNameList
            Component.onCompleted: {
                var lastInputUser = basicConfig.lastInputUser
                for(var i in model){
                    if(lastInputUser === model[i]){
                        currentIndex = i
                        break
                    }
                }
                txtUserName.isInit = true
            }
            onCurrentTextChanged: {
                if(!txtUserName.isInit){
                    return
                }
                basicConfig.setLastInputUser(currentText)
            }
        }
        Label{
            text: qsTr("Password:")
            horizontalAlignment: Text.AlignRight
        }
        TextField{
            id: txtPassword
            echoMode: TextInput.Password
            selectByMouse: true
            Layout.fillWidth: true
        }
    }

    RowLayout{
        Button{
            id: btnLogin
            text: qsTr("Login")
            onClicked: {
                if(userManagement.login(txtUserName.currentText, txtPassword.text))
                {
                    closePopup()
                }
            }
        }
        Button{
            id: btnCancel
            visible: cancelButtonVisible
            text: qsTr("Cancel")
            onClicked: {
                closePopup()
            }
        }
        Button{
            visible: changePswButtonVisible
            text: qsTr("ChangePassword")
            onClicked: {
                popupChangePsw.clearText()
                popupChangePsw.open()
            }
        }
    }

    Keys.onEscapePressed: {
        btnCancel.clicked()
    }
    Keys.onReturnPressed: {
        btnLogin.clicked()
    }
}
