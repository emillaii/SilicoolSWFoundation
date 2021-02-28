import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

RowLayout{
    property var rowColInfo: null
    property bool unitIndexVisible: true
    property int customizedImplicitWidth: 60

    Label{
        visible: unitIndexVisible
        text: qsTr("UnitRowIndex:")
    }
    TextField{
        visible: unitIndexVisible
        selectByMouse: true
        validator: IntValidator{}
        implicitWidth: customizedImplicitWidth
        text: rowColInfo.unitRow
        horizontalAlignment: TextInput.AlignHCenter
        onEditingFinished: {
            rowColInfo.unitRow = text
        }
    }

    Label{
        visible: unitIndexVisible
        text: qsTr("UnitColumnIndex:")
    }
    TextField{
        visible: unitIndexVisible
        selectByMouse: true
        validator: IntValidator{}
        implicitWidth: customizedImplicitWidth
        text: rowColInfo.unitColumn
        horizontalAlignment: TextInput.AlignHCenter
        onEditingFinished: {
            rowColInfo.unitColumn = text
        }
    }

    Label{
        text: unitIndexVisible ? qsTr("RowIndexInsideUnit:") : qsTr("Row:")
    }
    TextField{
        selectByMouse: true
        validator: IntValidator{}
        implicitWidth: customizedImplicitWidth
        text: rowColInfo.row
        horizontalAlignment: TextInput.AlignHCenter
        onEditingFinished: {
            rowColInfo.row = text
        }
    }

    Label{
        text: unitIndexVisible ? qsTr("ColumnIndexInsideUnit:") : qsTr("Col:")
    }
    TextField{
        selectByMouse: true
        validator: IntValidator{}
        implicitWidth: customizedImplicitWidth
        text: rowColInfo.column
        horizontalAlignment: TextInput.AlignHCenter
        onEditingFinished: {
            rowColInfo.column = text
        }
    }
}
