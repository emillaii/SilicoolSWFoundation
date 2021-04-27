import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout {
    property string leftIcon: ""
    property string leftLeftIcon: ""
    property string rightIcon: ""
    property string rightRightIcon: ""
    property string upIcon: ""
    property string upUpIcon: ""
    property string downIcon: ""
    property string downDownIcon: ""

    property string horizontalAxisName: "x"
    property string verticalAxisName: "y"
    property bool horizontalVisible: true
    property bool verticalVisible: true

    function onUpKeyPressed(){
        if(verticalVisible){
            stepMove(verticalAxisName, 1)
        }
    }
    function onDownKeyPressed(){
        if(verticalVisible){
            stepMove(verticalAxisName, -1)
        }
    }
    function onLeftKeyPressed(){
        if(horizontalVisible){
            stepMove(horizontalAxisName, -1)
        }
    }
    function onRightKeyPressed(){
        if(horizontalVisible){
            stepMove(horizontalAxisName, 1)
        }
    }

    signal jogMove(string axisType, int dir);
    signal stopJogMove(string axisType)
    signal stepMove(string axisType, int dir)

    property int __btnWH: 30

    width: __btnWH * 4 +40
    height: width

    ColumnLayout{
        Layout.alignment: Qt.AlignHCenter
        visible: verticalVisible
        MyRoundBtn{
            widthHeight: __btnWH
            iconSource: upUpIcon
            btn.onPressed: {
                jogMove(verticalAxisName, 1)
            }
            btn.onReleased: {
                stopJogMove(verticalAxisName)
            }
        }
        MyRoundBtn{
            widthHeight: __btnWH
            iconSource: upIcon
            btn.onClicked: {
                stepMove(verticalAxisName, 1)
            }
        }
    }

    RowLayout{
        RowLayout{
            visible: horizontalVisible
            MyRoundBtn{
                widthHeight: __btnWH
                iconSource: leftLeftIcon
                btn.onPressed: {
                    jogMove(horizontalAxisName, 0)
                }
                btn.onReleased: {
                    stopJogMove(horizontalAxisName)
                }
            }
            MyRoundBtn{
                widthHeight: __btnWH
                iconSource: leftIcon
                btn.onClicked: {
                    stepMove(horizontalAxisName, -1)
                }
            }
        }
        Rectangle{
            width: __btnWH
            height: __btnWH
            visible: horizontalVisible || verticalVisible
            color: "transparent"
            Label{
                anchors.fill: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                text: (horizontalVisible ? horizontalAxisName : "") + (verticalVisible ? verticalAxisName : "")
            }
        }
        RowLayout{
            visible: horizontalVisible
            MyRoundBtn{
                widthHeight: __btnWH
                iconSource: rightIcon
                btn.onClicked: {
                    stepMove(horizontalAxisName, 1)
                }
            }
            MyRoundBtn{
                widthHeight: __btnWH
                iconSource: rightRightIcon
                btn.onPressed: {
                    jogMove(horizontalAxisName, 1)
                }
                btn.onReleased: {
                    stopJogMove(horizontalAxisName)
                }
            }
        }
    }

    ColumnLayout{
        Layout.alignment: Qt.AlignHCenter
        visible: verticalVisible
        MyRoundBtn{
            widthHeight: __btnWH
            iconSource: downIcon
            btn.onClicked: {
                stepMove(verticalAxisName, -1)
            }
        }
        MyRoundBtn{
            widthHeight: __btnWH
            iconSource: downDownIcon
            btn.onPressed: {
                jogMove(verticalAxisName, 0)
            }
            btn.onReleased: {
                stopJogMove(verticalAxisName)
            }
        }
    }
}
