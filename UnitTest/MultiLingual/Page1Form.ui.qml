import QtQuick 2.4
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Page {
    property alias rdbChinese: rdbChinese
    property alias rdbEnglish: rdbEnglish
    ColumnLayout {
        RowLayout {
            RadioButton {
                id: rdbChinese
                text: qsTr("中文")
                checked: true
            }
            RadioButton {
                id: rdbEnglish
                text: qsTr("English")
                checked: false
            }
        }
        Label {
            text: qsTr("Lable")
        }
        TextEdit {
            text: qsTr("text")
        }
        Label {
            text: qsTr("修改")
        }
        Label {
            text: qsTr("添加")
        }
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
