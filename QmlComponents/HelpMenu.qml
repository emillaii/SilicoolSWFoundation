import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

Menu{
    title: qsTr("帮助")

    MenuItem {
        text: qsTr("制造商")
        onClicked: {
            popupManufacturerInfo.open()
        }

        Popup{
            id: popupManufacturerInfo

            ColumnLayout{
                Image {
                    source: "qrc:/commonicons/silicool1.png"
                }
                Image {
                    source: "qrc:/commonicons/silicool2.png"
                }
            }
        }
    }
}
