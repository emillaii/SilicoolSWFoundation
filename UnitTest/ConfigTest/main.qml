import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "qrc:/qmlComponents/ConfigManager"

Window {
    visible: true
    width: 1000
    height: 800
    title: qsTr("Hello World")

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            RoundButton {
                icon.source: "qrc:/commonicons/undo.png"
                icon.color: configManager.commandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.undo()
                }
            }
            RoundButton {
                icon.source: "qrc:/commonicons/redo.png"
                icon.color: configManager.undoCommandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.redo()
                }
            }
        }

        SwipeView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true

            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ObjectEditor {
                            sectionName: "testClassB"
                            configModel: testClassB
                            listMaxWidth: 600
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ObjectEditor {
                            sectionName: "testClassB"
                            configModel: testClassB
                            listMaxWidth: 600
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ArrayEditor {
                            configModel: intArray
                            sectionName: "intArray"
                            listMaxWidth: 250
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ArrayEditor {
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

            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ArrayEditor {
                            sectionName: "testClassCArray"
                            configModel: testClassCArray
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ArrayEditor {
                            sectionName: "testClassCArray"
                            configModel: testClassCArray
                            Component.onCompleted: {
                                init()
                            }
                        }
                    }
                }
            }

            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ObjectEditor {
                            sectionName: "testClassC"
                            configModel: testClassC
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ObjectEditor {
                            sectionName: "testClassC"
                            configModel: testClassC
                            Component.onCompleted: {
                                init()
                            }
                        }
                    }
                }
            }

            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ObjectEditor {
                            sectionName: "testClassD"
                            configModel: testClassD
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ObjectEditor {
                            sectionName: "testClassD"
                            configModel: testClassD
                            Component.onCompleted: {
                                init()
                            }
                        }
                    }
                }
            }
            Page {
                ScrollView {
                    anchors.fill: parent
                    clip: true

                    RowLayout {
                        ArrayEditor {
                            configModel: testClassEArray
                            sectionName: "testClassEArray"
                            listMaxWidth: 800
                            Component.onCompleted: {
                                init()
                            }
                        }

                        ArrayEditor {
                            configModel: testClassEArray
                            sectionName: "testClassEArray"
                            listMaxWidth: 800
                            Component.onCompleted: {
                                init()
                            }
                        }
                    }
                }
            }
        }

        PageIndicator {
            id: indicator

            count: view.count
            currentIndex: view.currentIndex
            Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
        }
    }
}
