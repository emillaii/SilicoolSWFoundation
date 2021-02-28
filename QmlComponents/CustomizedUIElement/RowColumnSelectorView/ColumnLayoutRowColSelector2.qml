import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    property var rowColInfo: null
    property bool unitIndexVisible: true
    property int customizedImplicitWidth: 60

    RowLayout{
        spacing: 0
        Label{
            visible: unitIndexVisible
            text: qsTr("UnitIndex:")
        }
        TextField{
            visible: unitIndexVisible
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: customizedImplicitWidth
            text: rowColInfo.unitIndex
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                rowColInfo.unitIndex = text
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/add1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.unitIndex += 1
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/reduce1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.unitIndex -= 1
            }
        }
    }

    RowLayout{
        spacing: 0
        Label{
            text: qsTr("RowIndex:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: customizedImplicitWidth
            text: rowColInfo.rowIndex
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                rowColInfo.rowIndex = text
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/add1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.rowIndex += 1
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/reduce1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.rowIndex -= 1
            }
        }
    }

    RowLayout{
        spacing: 0
        Label{
            text: qsTr("ColumnIndex:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: customizedImplicitWidth
            text: rowColInfo.columnIndex
            horizontalAlignment: TextInput.AlignHCenter
            onEditingFinished: {
                rowColInfo.columnIndex = text
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/add1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.columnIndex += 1
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/reduce1.png"
            icon.color: "transparent"
            onClicked: {
                rowColInfo.columnIndex -= 1
            }
        }
    }
}
