import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ColumnLayout{
    property var configModel: null
    property string sectionName: "name"
    property int listMaxWidth: 1200
    property int listMaxHeight: 600
    property bool isExpand: false
    property bool titleVisible: true

    RowLayout{
        visible: titleVisible || (!isExpand)
        Label{
            text: sectionName
        }
        RoundButton{
            radius: 8
            icon.source: isExpand ? "qrc:/commonicons/collapse.png" : "qrc:/commonicons/expand.png"
            icon.color: "deeppink"
            display: AbstractButton.IconOnly
            icon.width: 45
            icon.height: 45
            padding: 0
            onClicked: {
                isExpand = !isExpand
            }
        }
    }
}
