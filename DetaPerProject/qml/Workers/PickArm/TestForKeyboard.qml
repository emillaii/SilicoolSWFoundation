import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/AxisModulePos"
import "qrc:/qmlComponents/VisionViewer"
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/MotionViewer"

 GroupBox
 {
     id:iii
     width: 680
     height: 500
     ColumnLayout
     {
         width: 680
         height: 500
         Rectangle {
             width: 100; height: 200
             focus: true
             color: activeFocus ? "red" : "lightblue"

             MouseArea {
                 anchors.fill: parent
                 onClicked: parent.forceActiveFocus()
                 Keys.onPressed:
                 {
                     console.log("MouseArea:"+event.key);
                     switch(event.key)
                     {
                     case Qt.Key_0:
                         //event.accepted = true;
                         console.log("click key 0.");
                         break;

                     case Qt.Key_Home:
                         //console.log("click Key_Home");
                         break;

                     case Qt.Key_End:
                         console.log("click Key_End");
                         break;

                     case Qt.Key_PageUp:
                         console.log("click Key_PageUp");
                         break;

                     case Qt.Key_PageDown:
                         console.log("click Key_PageDown");
                         break;

                     case Qt.Key_Up:

                         break;

                     case Qt.Key_Down:

                         break;

                     case Qt.Key_Left:

                         break;

                     case Qt.Key_Right:

                         break;

                     }
                 }
             }
             Keys.onPressed:
             {
                 console.log("Test:"+event.key);
                 switch(event.key)
                 {
                 case Qt.Key_0:
                     //event.accepted = true;
                     console.log("click key 0.");
                     break;

                 case Qt.Key_Home:
                     //console.log("click Key_Home");
                     break;

                 case Qt.Key_End:
                     console.log("click Key_End");
                     break;

                 case Qt.Key_PageUp:
                     console.log("click Key_PageUp");
                     break;

                 case Qt.Key_PageDown:
                     console.log("click Key_PageDown");
                     break;

                 case Qt.Key_Up:

                     break;

                 case Qt.Key_Down:

                     break;

                 case Qt.Key_Left:

                     break;

                 case Qt.Key_Right:

                     break;

                 }
             }
         }
     Rectangle{
         color:"green"
         width: parent.width
         height: parent.height
     MouseArea
     {
         id:mouse;
                 anchors.fill:parent
                 hoverEnabled: true; // true-鼠标进入变色 false-鼠标进入后点击才变色，甚至焦点不在应用上
                 onEntered: {

                 }
        onClicked:
        {
           iii.activeFocus
           focus: true
        }
        Button
        {
            id:idPickMaterial
            text: qsTr("取料模板设置")
            Layout.fillWidth: true
            onClicked:
            {
                console.log("123");
            }
        }
     }
}
 }
     }


