import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5

TableView{
    implicitWidth: 1500
    implicitHeight: 800
    TableViewColumn{
        role: "timeStamp"
        title: qsTr("时间")
        width: 200
        horizontalAlignment: Qt.AlignHCenter
    }
    TableViewColumn{
        role: "module"
        title: qsTr("模块")
        width: 200
        horizontalAlignment: Qt.AlignHCenter
    }
    TableViewColumn{
        role: "alarmMessage"
        title: qsTr("报警消息")
        width: 1050
        horizontalAlignment: Qt.AlignHCenter
    }

    model: alarmModel
}
