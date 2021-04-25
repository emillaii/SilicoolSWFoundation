import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

TableView{
    id: trayMapEditor

    property int tableMaxWidth: 400
    property int tableMaxHeight: 250
    property int cellMaxSize: 30
    property string defaultColor: "red"
    property var trayConfig: null
    property var trayMap: null
    property var status2OpacityMap: []
    property var status2ColorMap: []
    property var statusEnumeration: []

    property int __spliterLineHalfWidth: 1
    property int __selectedRowIndexStart: -1
    property int __selectedColIndexStart: -1
    property int __selectedRowIndexEnd: -1
    property int __selectedColIndexEnd: -1
    property var __releaseMousePos: "0,0"
    property var __items: []

    signal materialStateSetted()

    Popup{
        id: popupSetStatus

        contentItem: ColumnLayout{
            spacing: 5
            Repeater{
                model: statusEnumeration
                delegate: Button{
                    implicitHeight: 35
                    topInset: 3
                    topPadding: 4
                    bottomInset: 3
                    bottomPadding: 4
                    text: modelData
                    onClicked: {
                        trayMap.setMaterialState(__selectedRowIndexStart, __selectedColIndexStart,
                                                 __selectedRowIndexEnd, __selectedColIndexEnd, modelData)
                        materialStateSetted()
                        popupSetStatus.close()
                    }
                }
            }
        }

        onClosed: {
            for(var i in __items){
                __items[i].__isSelected = false
            }
        }
    }

    function startEdit(){
        if(__selectedRowIndexEnd >= __selectedRowIndexStart && __selectedColIndexEnd >= __selectedColIndexStart
                && __selectedRowIndexStart >=0 && __selectedColIndexStart >= 0){
            for(var r = __selectedRowIndexStart; r <= __selectedRowIndexEnd; r++){
                for(var c = __selectedColIndexStart; c <= __selectedColIndexEnd; c++){
                    __items[index2string(r, c)].__isSelected = true
                }
            }
            popupSetStatus.x = __releaseMousePos.x
            popupSetStatus.y = __releaseMousePos.y
            popupSetStatus.open()
        }
    }

    function index2string(rowIndex, columnIndex){
        return String(rowIndex) + "_" + String(columnIndex)
    }

    implicitWidth: Math.min(tableMaxWidth, trayConfig.columns * cellMaxSize)
    implicitHeight: Math.min(tableMaxHeight, trayConfig.rows * cellMaxSize)
    model: trayMap
    delegate: Rectangle{
        z: 5
        property string __status: status
        property bool __isSelected: false

        implicitWidth: trayMapEditor.width / trayConfig.columns
        implicitHeight: trayMapEditor.height / trayConfig.rows
        border.width: 1

        Rectangle{
            z: 6
            anchors.centerIn: parent
            radius: 3
            width: radius * 2
            height: width
            visible: parent.__isSelected
            color: "black"
        }

        MouseArea{
            z: 7
            anchors.fill: parent
            hoverEnabled: true
            ToolTip.text: status
            ToolTip.visible: containsMouse
            acceptedButtons: {
                Qt.LeftButton | Qt.RightButton
            }
            onClicked: {
                if(mouse.button === Qt.LeftButton){
                    __selectedRowIndexStart = row
                    __selectedColIndexStart = column
                }else if(mouse.button === Qt.RightButton){
                    __selectedRowIndexEnd = row
                    __selectedColIndexEnd = column
                    __releaseMousePos = mapToItem(trayMapEditor, mouseX, mouseY)
                    startEdit()
                }
            }
        }

        function updateColorOpacity(){
            try{
                opacity = status2OpacityMap[status]
            }
            catch(e1){
                opacity = 1
            }
            try{
                color = status2ColorMap[status]
            }
            catch(e2){
                color = defaultColor
            }
        }

        on__StatusChanged: {
            updateColorOpacity()
        }
        Component.onCompleted: {
            updateColorOpacity()
            __items[index2string(row, column)] = this
        }
    }
    Repeater{
        model: trayConfig.unitColumns - 1
        Rectangle{
            x: trayMapEditor.implicitWidth / trayConfig.unitColumns * (modelData + 1) - __spliterLineHalfWidth
            y: 0
            z: 10
            width: __spliterLineHalfWidth * 2
            height: trayMapEditor.implicitHeight
            color: "pink"
        }
    }
    Repeater{
        model: trayConfig.unitRows - 1
        Rectangle{
            x: 0
            y: trayMapEditor.implicitHeight / trayConfig.unitRows * (modelData + 1) - __spliterLineHalfWidth
            z: 10
            width: trayMapEditor.implicitWidth
            height: __spliterLineHalfWidth * 2
            color: "pink"
        }
    }
    Connections{
        target: trayMap
        onReqGrabMapToImage: {
            var size = Qt.size(trayMapEditor.width, trayMapEditor.height)
            if(imageWidth > 0 && imageHeight > 0){
                size = Qt.size(imageWidth, imageHeight)
            }
            if(!trayMapEditor.grabToImage(function(result){
                if(!result.saveToFile(imageFileFullName)){
                    console.error("Save tray map image failed!")
                }
                trayMap.grabMapToImageDone()
            }, size)){
                console.error("Grab tray map to image failed!")
                trayMap.grabMapToImageDone()
            }
        }
    }
}
