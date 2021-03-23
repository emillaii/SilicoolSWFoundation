import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import "../ConfigManager"

Item {
    ColumnLayout{
        ObjectEditor{
            configModel: laserSerialPortConfig
            sectionName: qsTr("激光测高仪串口配置")
            Component.onCompleted: {
                init()
            }
        }
        RowLayout{
            LaserStation{
                laserName: qsTr("左激光测高仪")
                laserConfig: leftLaserConfig
                laser: leftLaser
                Component.onCompleted: {
                    init()
                }
            }
            LaserStation{
                laserName: qsTr("右激光测高仪")
                laserConfig: rightLaserConfig
                laser: rightLaser
                Component.onCompleted: {
                    init()
                }
            }
        }
    }
}
