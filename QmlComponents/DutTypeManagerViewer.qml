import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    RowLayout{
        Label{
            text: qsTr("DutTypes:")
        }
        ComboBox{
            id: cmbDutType
            model: dutTypeManager.dutTypes
            implicitWidth: 200
        }
        RoundButton{
            icon.source: "qrc:/commonicons/remove.png"
            icon.color: "transparent"
            onClicked: {
                dutTypeManager.removeDutType(cmbDutType.currentText)
            }
        }
    }
    RowLayout{
        CheckBox{
            id: ckbCopyFromSelectedDutType
            text: qsTr("CopyFromSelectedDutType")
            checked: false
        }
        Label{
            text: qsTr("DutType:")
        }
        TextField{
            id: txtDutType
            selectByMouse: true
            implicitWidth: 150
        }
        RoundButton{
            icon.source: "qrc:/commonicons/add.png"
            icon.color: "transparent"
            onClicked: {
                var copyFormDutType = ""
                if(ckbCopyFromSelectedDutType.checked){
                    copyFormDutType = cmbDutType.currentText
                }
                dutTypeManager.addDutType(txtDutType.text, copyFormDutType)
            }
        }

    }
}
