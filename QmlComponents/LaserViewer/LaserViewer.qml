import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import "../ConfigManager"

Item {
    ColumnLayout{
        ObjectEditor{
            configModel: laserSerialPortConfig
            sectionName: qsTr("LaserSerialPortConfig")
            Component.onCompleted: {
                init()
            }
        }
        RowLayout{
            LaserStation{
                laserName: qsTr("LeftLaser")
                laserConfig: leftLaserConfig
                laser: leftLaser
                Component.onCompleted: {
                    init()
                }
            }
            LaserStation{
                laserName: qsTr("RightLaser")
                laserConfig: rightLaserConfig
                laser: rightLaser
                Component.onCompleted: {
                    init()
                }
            }
        }
    }
}
