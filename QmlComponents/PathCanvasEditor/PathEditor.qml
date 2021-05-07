import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2
import "qrc:/qmlComponents/CustomizedUIElement"

Item {

    property real startX
    property real startY
    property real stopX
    property real stopY
    property var paintType: ["line","rect","circle","curve"]
    property bool isMouseMoveEnable: false  //是否允许鼠标移动绘制事件

    function translate(x,y) {
        x = parseFloat(x)
        y = parseFloat(y)
        var newPointsArray = []
        for (var index in canvas.arrpoints) {
            var px = parseFloat(canvas.arrpoints[index]["x"])
            var py = parseFloat(canvas.arrpoints[index]["y"])
            var nx = px + x
            var ny = py + y
            newPointsArray.push({"x": nx,
                                 "y": ny,
                                 "z": canvas.arrpoints[index]["z"],
                                 "type": canvas.arrpoints[index]["type"]})
        }
        canvas.arrpoints = newPointsArray
        pointsTable.testModel.refreshData(canvas.arrpoints)
        canvas.requestPaint()
    }

    //t is rotatation theta in degree
    function rotate(t) {
        t = parseFloat(t)
        var newPointsArray = []
        for (var index in canvas.arrpoints) {
            var theta = t*Math.PI/180;
            var px = parseFloat(canvas.arrpoints[index]["x"])
            var py = parseFloat(canvas.arrpoints[index]["y"])

            var nx = Math.cos(theta)*(px - canvas.inputWidth/2) - Math.sin(theta)*(py-canvas.inputHeight/2) +  canvas.inputWidth/2
            var ny = Math.sin(theta)*(px - canvas.inputWidth/2) + Math.cos(theta)*(py-canvas.inputHeight/2) +  canvas.inputHeight/2
            newPointsArray.push({"x": nx,
                                 "y": ny,
                                 "z": canvas.arrpoints[index]["z"],
                                 "type": canvas.arrpoints[index]["type"]})
        }
        canvas.arrpoints = newPointsArray
        pointsTable.testModel.refreshData(canvas.arrpoints)
        canvas.requestPaint()
    }

    function doScale(s){
        s = parseFloat(s)
        var newPointsArray = []
        var centerX = canvas.inputWidth / 2
        var centerY = canvas.inputHeight / 2
        for (var index in canvas.arrpoints) {
            var px = parseFloat(canvas.arrpoints[index]["x"])
            var py = parseFloat(canvas.arrpoints[index]["y"])
            var nx = (px - centerX) * s + centerX
            var ny = (py - centerY) * s + centerY
            newPointsArray.push({"x": nx,
                                 "y": ny,
                                 "z": canvas.arrpoints[index]["z"],
                                 "type": canvas.arrpoints[index]["type"]})
        }
        canvas.arrpoints = newPointsArray
        pointsTable.testModel.refreshData(canvas.arrpoints)
        canvas.requestPaint()
    }

    FileDialog {
        id: loadImageDialog
        title: qsTr("选择加载图像")
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        folder: pathEditorConfig.imageDir

        nameFilters: ["图像文件 (*.jpg || *.bmp || *.png)"]
        onAccepted: {
            pathEditorConfig.setImageDir(folder)
            canvas.imageUrl = loadImageDialog.fileUrl
            var result = pointsTable.testModel.checkImageSize(canvas.imageUrl)
            canvas.inputWidth = result["width"]
            canvas.inputHeight = result["height"]
            canvas.requestPaint()
        }
    }

    FileDialog {
        id: saveDispensePathDialog
        title: qsTr("保存点胶路径")
        selectExisting: false
        selectFolder: false
        selectMultiple: false
        folder: pathEditorConfig.pathDir

        onAccepted: {
            pathEditorConfig.setPathDir(folder)
            pointsTable.testModel.saveOutputJson(fileUrl)
            var fileName = fileUrl.toString()
            fileName = fileName.replace("file:///", "");
            fileName =  fileName.replace(".json", "")
            fileName += "_resultPath.jpg"
            canvas.save(fileName)
            pointsTable.testModel.saveRawImage(canvas.imageUrl, fileUrl)
        }
    }

    FileDialog {
        id: loadDispensePathDialog
        title: qsTr("Load Dispense Path")
        selectExisting: true
        selectFolder: false
        selectMultiple: false
        folder: pathEditorConfig.pathDir
        nameFilters: ["Json文件 (*.json)"]

        onAccepted: {
            pathEditorConfig.setPathDir(folder)
            pointsTable.testModel.loadJson(fileUrl)
        }
    }

    RowLayout {
        ColumnLayout {
            Layout.alignment: Qt.AlignTop
            RowLayout{
                Button {
                    text: qsTr("加载图像")
                    onClicked: {
                        loadImageDialog.open()
                    }
                }
            }

            RowLayout{
                Button {
                    text: qsTr("保存点胶路径")
                    onClicked: {
                        saveDispensePathDialog.open()
                    }
                }
                Button {
                    text: qsTr("加载点胶路径")
                    onClicked: {
                        loadDispensePathDialog.open()
                    }
                }
            }

            RowLayout{
                Frame{
                    RowLayout{
                        RadioButton{
                            id: rdbDraw
                            text: qsTr("Draw")
                        }
                        RadioButton{
                            id: rdbSelect
                            text: qsTr("Select")
                        }
                    }
                }

                Button {
                    text: qsTr("Clear All")
                    onClicked: {
                        canvas.arrpoints = []
                        pointsTable.testModel.refreshData(canvas.arrpoints)
                        canvas.requestPaint()
                    }
                }

            }

            GroupBox {
                title: 'Select Step'
                RowLayout{
                    MyIntInput{
                        id: selectedStep
                        text: "5"
                    }

                    Repeater{
                        model: ["1", "5", "10", "50"]
                        delegate: RadioButton
                        {
                            text: modelData
                            onCheckedChanged: {
                                if(checked){
                                    selectedStep.text = modelData
                                }
                            }
                            Component.onCompleted: {
                                checked = (modelData === "5")
                            }
                        }
                    }
                }
            }

            GroupBox{
                title: qsTr("图像操作")
                ColumnLayout{
                    RowLayout{
                        Button {
                            text: qsTr("Zoom in")
                            onClicked: {
                                canvas.scaleFactor += 1
                                canvas.requestPaint()
                            }
                        }
                        Button {
                            text: qsTr("Zoom out")
                            onClicked:  {
                                canvas.scaleFactor -= 1
                                if (canvas.scaleFactor == 0) canvas.scaleFactor = 1
                                canvas.requestPaint()
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text: "X:"
                        }
                        Slider {
                            from: 1
                            value: 10
                            to: canvas.inputWidth - canvas.inputWidth/canvas.scaleFactor
                            onValueChanged: {
                                canvas.imageOffsetX  = value;
                                canvas.requestPaint()
                            }
                        }
                    }

                    RowLayout{
                        Label{
                            text: "Y:"
                        }
                        Slider {
                            from: 1
                            value: 10
                            to: canvas.inputHeight - canvas.inputHeight/canvas.scaleFactor
                            onValueChanged: {
                                canvas.imageOffsetY  = value;
                                canvas.requestPaint()
                            }
                        }
                    }
                }
            }

            GroupBox{
                title: qsTr("路径操作")

                ColumnLayout{
                    RowLayout{
                        MyDoubleInput {
                            id: rotatePattern
                            text: "0"
                        }
                        Button {
                            text: "+T"
                            onClicked: {
                                rotate(rotatePattern.text)
                            }
                        }
                        Button {
                            text: "-T"
                            onClicked: {
                                rotate(-rotatePattern.text)
                            }
                        }
                    }

                    RowLayout{
                        MyDoubleInput {
                            id: txtScale
                            text: "1"
                        }
                        Button {
                            text: qsTr("放大")
                            onClicked: {
                                var s = parseFloat(txtScale.text)
                                if(s === 0){
                                    return
                                }
                                if(s > 1){
                                    doScale(s)
                                }else{
                                    doScale(1.0 / s)
                                }
                            }
                        }
                        Button {
                            text: qsTr("缩小")
                            onClicked: {
                                var s = parseFloat(txtScale.text)
                                if(s === 0){
                                    return
                                }
                                if(s > 1){
                                    doScale(1.0 / s)
                                }else{
                                    doScale(s)
                                }
                            }
                        }
                    }

                    RowLayout{
                        Button {
                            text: qsTr("+X")
                            onClicked: {
                                var stepSize = parseFloat(selectedStep.text)
                                translate(stepSize, 0)
                            }
                        }
                        Button {
                            text: qsTr("-X")
                            onClicked: {
                                var stepSize = parseFloat(selectedStep.text)
                                translate(-stepSize, 0)
                            }
                        }
                        Button {
                            text: qsTr("+Y")
                            onClicked: {
                                var stepSize = parseFloat(selectedStep.text)
                                translate(0, -stepSize)
                            }
                        }
                        Button {
                            text: qsTr("-Y")
                            onClicked: {
                                var stepSize = parseFloat(selectedStep.text)
                                translate(0, stepSize)
                            }
                        }
                    }
                }
            }

            GroupBox{
                title: qsTr("路径")

                PointsTableWidget{
                    implicitWidth: 300
                    implicitHeight: 250
                    id: pointsTable
                    Connections {
                        target: pointsTable.testModel
                        onDataLoaded: {
                            canvas.arrpoints = []
                            for (var index in pointsTable.testModel.initData)
                            {
//                                console.log(pointsTable.testModel.initData[index]["x"])
                                canvas.arrpoints.push({"x": pointsTable.testModel.initData[index]["x"],
                                                       "y": pointsTable.testModel.initData[index]["y"],
                                                       "z": pointsTable.testModel.initData[index]["z"],
                                                       "type": "line"})
                            }

                            canvas.requestPaint()
                        }
                    }
                }
            }
        }

        MainCanvas {
            Layout.alignment: Qt.AlignTop
            id: canvas
        }
    }
}
