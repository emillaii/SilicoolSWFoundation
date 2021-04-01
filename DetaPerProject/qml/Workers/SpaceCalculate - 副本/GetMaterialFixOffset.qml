import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/MotionViewer"

ColumnLayout
{
    Label{
        Layout.minimumWidth :1000
        Layout.maximumWidth :1000
        Layout.minimumHeight: 20
        Layout.maximumHeight: 20
        text: qsTr("第一步:")
        font.pixelSize: 22 //字体大小
        color: "pink"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        background: Rectangle{color: "Transparent"}
    }
}

