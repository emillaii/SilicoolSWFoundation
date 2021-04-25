import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ListView {
    id: magazineMapEditor

    property int listWidth: 100
    property int listMaxHeight: 250
    property int cellMaxHeight: 25
    property string defaultColor: "red"
    property var magazineConfig: null
    property var magazineMap: null
    property var status2OpacityMap: []
    property var status2ColorMap: []
    property var statusEnumeration: []

    property int __selectedIndexStart: -1
    property int __selectedIndexEnd: -1
    property var __releaseMousePos: "0,0"
    property int __popupButtonHeight: 35
    property var __items: []

    signal trayStateSetted()

    Popup {
        id: popupSetStatus

        contentItem: ColumnLayout {
            spacing: 5
            Repeater {
                model: statusEnumeration
                delegate: Button {
                    implicitHeight: __popupButtonHeight
                    topInset: 3
                    topPadding: 4
                    bottomInset: 3
                    bottomPadding: 4
                    text: modelData
                    onClicked: {
                        magazineMap.setMaterialState(__selectedIndexStart,
                                                     __selectedIndexEnd,
                                                     modelData)
                        trayStateSetted()
                        popupSetStatus.close()
                    }
                }
            }
        }

        onClosed: {
            for (var i in __items) {
                __items[i].__isSelected = false
            }
        }
    }

    function startEdit() {
        if (__selectedIndexEnd >= __selectedIndexStart
                && __selectedIndexStart >= 0) {
            for (var i = __selectedIndexStart; i <= __selectedIndexEnd; i++) {
                __items[i].__isSelected = true
            }

            popupSetStatus.x = __releaseMousePos.x
            var popupHeight = (__popupButtonHeight + 5) * statusEnumeration.length
            if(__releaseMousePos.y + popupHeight >= magazineMapEditor.implicitHeight){
                popupSetStatus.y = __releaseMousePos.y - popupHeight
            }else{
                popupSetStatus.y = __releaseMousePos.y
            }
            popupSetStatus.open()
        }
    }

    implicitWidth: listWidth
    implicitHeight: Math.min(listMaxHeight, magazineConfig.trayCount * cellMaxHeight)
    model: magazineMap
    delegate: Rectangle {
        z: 5
        property string __status: status
        property bool __isSelected: false

        implicitWidth: magazineMapEditor.listWidth
        implicitHeight: magazineMapEditor.height / magazineConfig.trayCount
        border.width: 1

        Rectangle {
            z: 6
            anchors.centerIn: parent
            radius: 3
            width: radius * 2
            height: width
            visible: parent.__isSelected
            color: "black"
        }

        MouseArea {
            z: 7
            anchors.fill: parent
            hoverEnabled: true
            ToolTip.text: status
            ToolTip.visible: containsMouse
            acceptedButtons: {
                Qt.LeftButton | Qt.RightButton
            }
            onClicked: {
                if (mouse.button === Qt.LeftButton) {
                    __selectedIndexStart = index
                } else if (mouse.button === Qt.RightButton) {
                    __selectedIndexEnd = index
                    __releaseMousePos = mapToItem(magazineMapEditor,
                                                  mouseX, mouseY)
                    startEdit()
                }
            }
        }

        function updateColorOpacity() {
            try {
                opacity = status2OpacityMap[status]
            } catch (e1) {
                opacity = 1
            }
            try {
                color = status2ColorMap[status]
            } catch (e2) {
                color = defaultColor
            }
        }

        on__StatusChanged: {
            updateColorOpacity()
        }
        Component.onCompleted: {
            updateColorOpacity()
            __items[index] = this
        }
    }
}
