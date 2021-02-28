import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import "../ConfigManager"

ColumnLayout{
    property string laserName: ""
    property var laserConfig: null
    property var laser: null

    function init(){
        laserConfigEditor.init()
        connLaserHeightChanged.target = laser
    }

    ObjectEditor{
        id: laserConfigEditor
        configModel: laserConfig
        sectionName: laserName
    }
    Button{
        text: qsTr("Init")
        onClicked: {
            tem.runSingleCmd(laser, "init")
        }
    }
    RowLayout{
        Label{
            text: qsTr("ReadHeightInterval ms")
        }
        TextField{
            id: txtReadDataInterval
            validator: DoubleValidator{
                bottom: 0
                notation: DoubleValidator.StandardNotation
            }
            selectByMouse: true
            text: "30"
        }
    }
    RowLayout{
        Label{
            text: qsTr("Height:")
        }
        TextField{
            id: txtHeight
            readOnly: true
            Connections{
                id: connLaserHeightChanged
                target: null
                onCurrentHeightChanged:{
                    txtHeight.text = currentHeight
                }
            }
        }
    }
    RowLayout{
        Button{
            text: qsTr("StartReadHeight")
            onClicked: {
                laser.startContinuousReadHeight(txtReadDataInterval.text)
            }
        }
        Button{
            text: qsTr("Stop")
            onClicked: {
                laser.stopContinuousReadHeight()
            }
        }
    }
}
