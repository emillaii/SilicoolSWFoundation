import QtQuick 2.0
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11

Item {
    GroupBox
    {
        title: "Motion Time Calculator"
        ColumnLayout
        {
            RowLayout
            {
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("EncRes: ")
                }
                TextField {
                    id: encRes
                    implicitWidth: 50
                    implicitHeight: 40
                    text: "0.1"
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("Dist(mm): ")
                }
                TextField {
                    id: dist
                    implicitWidth: 50
                    implicitHeight: 40
                    text: "0.005"
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("MaxVel(mm/s): ")
                }
                TextField {
                    id: maxVel
                    implicitWidth: 50
                    implicitHeight: 40
                    text: "10"
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("MaxAcc(mm/s2): ")
                }
                TextField {
                    id: maxAcc
                    implicitWidth: 50
                    implicitHeight: 40
                    text: "500"
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    text: qsTr("MaxDec(mm/s2): ")
                }
                TextField {
                    id: maxDec
                    implicitWidth: 50
                    implicitHeight: 40
                    text: "500"
                    selectByMouse: true
                    horizontalAlignment: TextInput.AlignHCenter
                    validator: DoubleValidator {
                        notation: DoubleValidator.StandardNotation
                    }
                }
                Button{
                    text: qsTr("计算")
                    onClicked: {
                        var Dist = dist.text / encRes.text * 1000
                        var MaxAcc = maxAcc.text / encRes.text / 1000
                        var MaxDec = maxDec.text / encRes.text / 1000
                        var MaxVel = maxVel.text / encRes.text
                        tem.runSingleCmd(motionCore, "calculateProfileTime", [Dist, MaxVel, MaxAcc, MaxDec]);
                    }
                }
            }
        }
    }
}
