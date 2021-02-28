import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "./qml/ConfigManager"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Client")

    SwipeView{
        id: view
        anchors.fill: parent

        Page{
            ScrollView{
                anchors.fill: parent
                clip: true

                RowLayout{

                    ObjectEditor{
                        sectionName: "tca"
                        configModel: tca
                        listMaxWidth: 1000
                        Component.onCompleted: {
                            init()
                        }
                    }

                    ArrayEditor{
                        configModel: intArray
                        sectionName: "intArray"
                        listMaxWidth: 250
                        Component.onCompleted: {
                            init()
                        }
                    }
                }
            }
        }

        Page{
            ScrollView{
                anchors.fill: parent
                clip: true

                ObjectEditor{
                    sectionName: "tcc"
                    configModel: tcc
                    Component.onCompleted: {
                        init()
                    }
                }
            }
        }
    }

    PageIndicator{
        id: indicator

        count: view.count
        currentIndex: view.currentIndex
        Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
    }
}
