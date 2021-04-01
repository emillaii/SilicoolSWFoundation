import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"

ColumnLayout {
    RowLayout
    {
        VisionLocationViewer
        {
            visionLocationName: "PlaceMaterialLocation"
            calibrationName: "CalibrationForScale"
            calibrateBtnVisible: false
        }
        GroupBox
        {
            width: 680
            height: 500
            ColumnLayout
            {
                spacing: 5
                RowLayout
                {
                    spacing: 10
                    XYModulePos
                    {
                        moduleName: "PAXY"
                        posName: "TakePictureForTrayPos"
                        Layout.minimumWidth: 325
                        Layout.maximumWidth: 325
                        Layout.minimumHeight: 140
                        Layout.maximumHeight: 140
                    }
                    ColumnLayout
                    {
                        spacing: 5
                        CheckBox
                        {
                            id:idIsLeftNozzle2
                            text: qsTr("IsLeftNozzle")
                            checked: true
                        }
                        Button
                        {
                            text: qsTr("XY拍照并到放料位")
                            Layout.minimumWidth: 160
                            Layout.maximumWidth: 160
                            onClicked:
                            {
                                tem.runSingleCmd(mPickArm, qsTr("PlaceMaterialTest"),[idIsLeftNozzle2.checked])
                            }
                        }
                    }
                }
            }
        }
    }
}
