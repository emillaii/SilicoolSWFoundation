import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"

ColumnLayout {
    VisionLocationViewer{
        visionLocationName: "GetMaterialLocation"
        calibrationName: "CalibrationForScale"
        calibrateBtnVisible: false
    }

}
