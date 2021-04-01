import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/MotionViewer"
import "qrc:/qml/Workers/SpaceCalculate"

Item
{
    Flickable
    {
        implicitWidth: 1200
        implicitHeight: 800
        clip: true
        ColumnLayout
        {
            width: 1100
            Button
            {
                id:idPickArmClibration
                text: qsTr("剥单部分的物料间距计算")
                Layout.fillWidth: true
                onClicked:
                {
                    idMaterialSpace.visible = !idMaterialSpace.visible
                }
            }
            GroupBox
            {
                Layout.fillWidth: true
                id:idMaterialSpace
                visible: false
                MaterialSpace
                {
                    x:60
                }
            }
            Button
            {
                id:id
                text: qsTr("物料PR点和实际吸料点间距计算")
                Layout.fillWidth: true
                onClicked:
                {
                    idGetMaterialFixOffset.visible = !idGetMaterialFixOffset.visible
                }
            }
            GroupBox
            {
                Layout.fillWidth: true
                id:idGetMaterialFixOffset
                visible: false
                GetMaterialFixOffset{}
            }
        }
    }
}
