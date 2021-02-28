import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    property var rowColInfo: null

    RowLayout{
        Label{
            text: qsTr("UnitRowIndex:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: 60
            text: rowColInfo.unitRow
            onEditingFinished: {
                rowColInfo.unitRow = text
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("UnitColumnIndex:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: 60
            text: rowColInfo.unitColumn
            onEditingFinished: {
                rowColInfo.unitColumn = text
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("RowIndexInsideUnit:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: 60
            text: rowColInfo.row
            onEditingFinished: {
                rowColInfo.row = text
            }
        }
    }
    RowLayout{
        Label{
            text: qsTr("ColumnIndexInsideUnit:")
        }
        TextField{
            selectByMouse: true
            validator: IntValidator{}
            implicitWidth: 60
            text: rowColInfo.column
            onEditingFinished: {
                rowColInfo.column = text
            }
        }
    }
}
