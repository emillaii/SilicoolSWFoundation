import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout{
    property var rowColInfo: null
    property bool unitIndexVisible: true
    property int customizedImplicitWidth: 60

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
}
