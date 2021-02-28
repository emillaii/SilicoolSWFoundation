import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import UserMng 1.0

Popup {
    id: popupUserManagement
    x: 100
    y: 60
    width: 350
    height: 650

    function clearText()
    {
        loginComponent.clearText()
        txtUserName.clear()
        txtPassword.clear()
    }

    contentItem: ColumnLayout{
        anchors.fill: parent

        GroupBox{
            title: qsTr("Login Admin account")
            Layout.fillWidth: true
            LoginComponent{
                id: loginComponent
                anchors.fill: parent
                cancelButtonVisible: false
                changePswButtonVisible: false
            }
        }
        GroupBox{
            title: qsTr("用户管理")
            Layout.fillWidth: true
            ColumnLayout{
                anchors.fill: parent
                GridLayout{
                    columns: 2
                    rows: 3

                    Label{
                        text: qsTr("UserName:")
                    }
                    TextField{
                        id: txtUserName
                        selectByMouse: true
                        Layout.fillWidth: true
                    }
                    Label{
                        text: qsTr("Password:")
                    }
                    TextField{
                        id: txtPassword
                        selectByMouse: true
                        echoMode: TextInput.Password
                        Layout.fillWidth: true
                    }
                    Label{
                        text: qsTr("Authority:")
                    }
                    ComboBox{
                        id: cbxAuthority

                        model: ListModel{
                            ListElement{ text: "Engineer"; authority: UserMng.Engineer}
                            ListElement{ text: "Operator"; authority: UserMng.Operator}
                        }
                        textRole: "text"
                    }
                }

                RowLayout{
                    Button{
                        text: qsTr("Add")
                        onClicked: {
                            userManagement.addUser(txtUserName.text, txtPassword.text,
                                                   cbxAuthority.model.get(cbxAuthority.currentIndex).authority)
                        }
                    }
                    Button{
                        text: qsTr("Remove")
                        onClicked: {
                            userManagement.removeUser(tableUsers.model.get(tableUsers.currentRow, "name"))
                        }
                    }
                }
            }
        }

        UsersView{
            id: tableUsers

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
