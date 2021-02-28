import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

RowLayout{
    property string title: "title"
    property var relatedDebugger: null

    function xPos(){
        return Number(txtXPos.text)
    }
    function yPos(){
        return Number(txtYPos.text)
    }

    Label{
        text: title
    }
    Rectangle{
        width: 10
        height: 1
        color: "transparent"
    }
    Label{
        text: "X:"
    }
    MyDoubleInput{
        id: txtXPos
        readOnly: true
        text: "0"
    }
    Label{
        text: "Y:"
    }
    MyDoubleInput{
        id: txtYPos
        readOnly: true
        text: "0"
    }
    Button{
        text: qsTr("Read")
        onClicked: {
            var xyPos = relatedDebugger.getCurrentXYPos()
            txtXPos.text = xyPos.x
            txtYPos.text = xyPos.y
        }
    }
}
