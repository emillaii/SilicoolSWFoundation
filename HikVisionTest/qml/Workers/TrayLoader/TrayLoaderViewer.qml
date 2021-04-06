import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import ConfigElementType 1.0
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"
import "qrc:/qmlComponents/CustomizedUIElement/RowColumnSelectorView"
import "qrc:/qmlComponents/CustomizedUIElement"

Item {
    ColumnLayout {
        RowLayout {
            XYModulePos {
                moduleName: "TLXY"
                posName: "StandbyPos"
            }
            XYModulePos {
                moduleName: "TLXY"
                posName: "PrLeftTop"
            }
            XYModulePos {
                moduleName: "TLXY"
                posName: "ChangeTrayPos"
            }
        }

        HikVisionLocationViewer {
            calibrationName: "DutDownlookCalibration"
            visionLocationName: "DutDownlookLocation1"
            performOriginPrResultVisible: true
        }
        HikVisionLocationViewer {
            calibrationName: "DutDownlookCalibration"
            visionLocationName: "DutDownlookLocation2"
            performOriginPrResultVisible: true
        }
        Button{
            text: "PrTest"
            onClicked: {
                tl.prTest()
            }
        }

        ConfigRow {
            configModel: tlConfig
            configName: "nChangeTrayAfterContinuallyPrFailed"
            Component.onCompleted: {
                autoInit()
            }
        }

        RowColumnSelector {
            id: rowColSelector
            rowColInfo: trayRowColInfo
        }
        Button {
            text: qsTr("MoveToPRPos")
            onClicked: {
                tem.runSingleCmd(
                            tl, "moveToPrPos",
                            [trayRowColInfo.unitRow, trayRowColInfo.unitColumn, trayRowColInfo.row, trayRowColInfo.column])
            }
        }
    }
}
