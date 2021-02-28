import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.11
import QtQuick.Controls 2.5

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World") 

    SwipeView{
        id: view
        anchors.fill: parent
        Page1Form{
            rdbChinese.onClicked: {
                languageConfig.setLanguage(0)
            }
            rdbEnglish.onClicked: {
                languageConfig.setLanguage(1)
            }
        }
        Page2Form{

        }
    }
    PageIndicator{
        id: indicator

        count: view.count

        anchors.bottom: view.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

}
