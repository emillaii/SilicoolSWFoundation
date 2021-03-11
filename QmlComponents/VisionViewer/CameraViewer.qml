import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Flickable {
    property var camera: null
    property string imageSource: ""
    property string cameraName: "UnDefined"
    property bool isFullScreenView: false
    property bool updateImage: true

    property int __currentLightSourceBrightness: -999
    property int __currentSecondLightSourceBrightness: -999

    property bool __updateXYAndValue: true
    property int __x: -1
    property int __y: -1
    property int __value: -1
    property string __imgProcessingRes: ""
    property double __lblBackgrountOpacity: 0.6
    property bool __needRefreshImage: false

    onUpdateImageChanged: {
        if(updateImage && __needRefreshImage){
            image.source = ""
            image.source = imageSource
            __needRefreshImage = false
        }
    }

    function init(){
        cameraConn.target = camera
        __currentLightSourceBrightness = camera.lightSourceBrightness
        slider.value = __currentLightSourceBrightness
        __currentSecondLightSourceBrightness = camera.secondLightSourceBrightness
        slider2.value = __currentSecondLightSourceBrightness
    }

    signal reqShowFullScreen()
    signal reqExitFullScreen()
    signal reqShowRelatedDebugger(string dbg, var pos)

    function __calculateOffset(w, h, px, py, rw, rh)
    {
        var delta = {};
        delta.x = (px - w/2)*rw;
        delta.y = (py - h/2)*rh;
        return delta;
    }

    clip: true

    ToolButton{
        implicitWidth: 40
        implicitHeight: 40
        display: AbstractButton.IconOnly
        icon.color: "limegreen"
        icon.source: "qrc:/commonicons/debug.png"
        onClicked: {
            reqShowRelatedDebugger(cameraName, mapToGlobal(x, y))
        }
        ToolTip{
            text: qsTr("OpenMotorViewer")
            visible: parent.hovered
        }
        z: 10
    }

    Image {
        id: image

        width: parent.width
        height: parent.height

        source: "qrc:/commonicons/no-image.png"
        fillMode: Image.PreserveAspectFit
        cache: false
        smooth: false
        clip: true
    //    asynchronous: true

        Rectangle {
            color: "pink"
            opacity: 0.8
            x: (image.width - image.paintedWidth)/2
            y: image.height/2
            width: image.paintedWidth
            height: 1
        }
        Rectangle {
            color: "pink"
            opacity: 0.8
            x: image.width/2
            y: (image.height - image.paintedHeight)/2
            width: 1
            height: image.paintedHeight
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            drag.target: image

            onDoubleClicked: {
                var borderX = (image.width - image.paintedWidth) / 2
                var borderY = (image.height - image.paintedHeight) / 2
                if (mouseX < borderX || mouseX > image.width - borderX ||
                        mouseY < borderY || mouseY > image.height - borderY)
                {
                    return
                }
                var delta = __calculateOffset(image.width, image.height, mouseX, mouseY,
                                            image.sourceSize.width/image.paintedWidth,
                                            image.sourceSize.height/image.paintedHeight)
                tem.runSingleCmd(visionManager, "moveCamera", [cameraName, delta.x, delta.y])
            }
            onWheel: {
                var delta = wheel.angleDelta.y / 120
                if(delta > 0){
                    image.scale = image.scale / 0.9
                }
                if(delta < 0){
                    if(image.scale < 0.5){
                        return
                    }
                    image.scale = image.scale * 0.9
                }
            }
            onMouseXChanged: {
                mouseXyChanged()
            }
            onMouseYChanged: {
                mouseXyChanged()
            }

            function mouseXyChanged(){
                if(__updateXYAndValue){
                    var borderX = (image.width - image.paintedWidth) / 2
                    var borderY = (image.height - image.paintedHeight) / 2
                    if (mouseX < borderX || mouseX > image.width - borderX ||
                            mouseY < borderY || mouseY > image.height - borderY)
                    {
                        return
                    }
                    __x = image.sourceSize.width / image.paintedWidth * (mouseX - borderX)
                    __y = image.sourceSize.height / image.paintedHeight * (mouseY - borderY)
                    __value = camera.pixelValue(__x, __y)
                }
            }
        }

        Connections{
            id: cameraConn
            target: null
            onLightSourceBrightnessChanged: {
                __currentLightSourceBrightness = lightSourceBrightness
                if(!slider.pressed){
                    slider.value = lightSourceBrightness
                }
            }
            onSecondLightSourceBrightnessChanged: {
                __currentSecondLightSourceBrightness = secondLightSourceBrightness
                if(!slider2.pressed){
                    slider2.value = secondLightSourceBrightness
                }
            }
            onImageChanged: {
                __imgProcessingRes = imgProcessingResult
                if(updateImage){
                    image.source = ""
                    image.source = imageSource
                }else{
                    __needRefreshImage = true
                }
            }
        }
        onStatusChanged: {
            if(status == Image.Ready){
                __updateXYAndValue = true
            }else{
                __updateXYAndValue = false
            }
        }
    }

    Label {
        id: lblCameraName
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            color: "black"
            opacity: __lblBackgrountOpacity
        }
        y: 5
        text: cameraName + "    " + "x:" + __x + " y:" + __y + " value:" + __value
        font.pixelSize: 13
    }
    Label {
        visible: __imgProcessingRes != ""
        anchors.horizontalCenter: parent.horizontalCenter
        background: Rectangle{
            color: "black"
            opacity: __lblBackgrountOpacity
        }
        anchors.top: lblCameraName.bottom
        text: __imgProcessingRes
        font.pixelSize: 13
    }
    ColumnLayout{
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        spacing: 0

        RowLayout{
            Slider {
                id: slider
                implicitWidth: 120
                implicitHeight: 25
                stepSize: 1
                from: 0
                to: 255
                onValueChanged: {
                    if(__currentLightSourceBrightness != value) {
                        tem.runSingleCmd(camera, "setLightSourceBrightness", [value])
                    }
                }
            }
            Label {
                text: slider.value
                background: Rectangle{
                    color: "black"
                    opacity: __lblBackgrountOpacity
                }
            }
        }
        RowLayout{
            Slider {
                id: slider2

                visible: camera.hasSecondLightSourceChannel
                implicitWidth: 120
                implicitHeight: 25
                stepSize: 1
                from: 0
                to: 255
                onValueChanged: {
                    if(__currentSecondLightSourceBrightness != value) {
                        tem.runSingleCmd(camera, "setSecondLightSourceBrightness", [value])
                    }
                }
            }
            Label {
                visible: camera.hasSecondLightSourceChannel
                text: slider2.value
                background: Rectangle{
                    color: "black"
                    opacity: __lblBackgrountOpacity
                }
            }
        }
    }

    RowLayout{
        anchors.right: parent.right
        anchors.rightMargin: 15
        anchors.bottom: parent.bottom
        spacing: 2

        property int btnSize: 30

        ToolButton {
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.color: "limegreen"
            icon.source: "qrc:/commonicons/reset.png"
            onClicked: {
                image.scale = 1
                image.x = 0
                image.y = 0
            }
            ToolTip{
                text: qsTr("Reset image anchor and scale")
                visible: parent.hovered
            }
        }
        ToolButton {
            id: cameraOnOff
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.color: camera.isOpened ? "limegreen" : "red"
            icon.source: "qrc:/commonicons/cameraOnOff.png"
            onClicked: {
                if (camera.isOpened) {
                    camera.close()
                } else {
                    tem.runSingleCmd(camera, "open")
                }
            }
            ToolTip{
                text: qsTr("Open/Close")
                visible: parent.hovered
            }
        }
        ToolButton {
            id: cameraStartStop
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.color: camera.isShowRealtimeImage ? "limegreen" : "red"
            icon.source: "qrc:/commonicons/pause.png"
            onClicked: {
                if(camera.isShowRealtimeImage){
                    camera.stopShowRealtimeImage()
                }else{
                    camera.startShowRealtimeImage()
                }
            }
            ToolTip{
                text: qsTr("LiveView")
                visible: parent.hovered
            }
        }
        ToolButton {
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.source: "qrc:/commonicons/single.png"
            icon.color: "limegreen"
            onClicked: {
                visionManager.handleSingleCameraLiveView(cameraName)
            }
            ToolTip{
                text: qsTr("SingleCameraLiveView")
                visible: parent.hovered
            }
        }
        ToolButton {
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.color: "lightGreen"
            icon.source: "qrc:/commonicons/save.png"
            onClicked: {
                camera.saveImage()
            }
            ToolTip{
                text: qsTr("SaveImage")
                visible: parent.hovered
            }
        }
        ToolButton {
            implicitWidth: parent.btnSize
            implicitHeight: parent.btnSize
            display: AbstractButton.IconOnly
            icon.color: "lightGreen"
            icon.source: isFullScreenView ? "qrc:/commonicons/fullScreenCancel.png" : "qrc:/commonicons/fullScreen.png"
            onClicked: {
                if(isFullScreenView){
                    reqExitFullScreen()
                }else{
                    reqShowFullScreen()
                }
            }
            ToolTip{
                text: isFullScreenView ? qsTr("CancelFullScreen") : qsTr("FullScreen")
                visible: parent.hovered
            }
        }
    }
}
