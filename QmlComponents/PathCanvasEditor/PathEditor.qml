import QtQuick 2.0
import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Item {

    property real startX
    property real startY
    property real stopX
    property real stopY
    property var paintType: ["line","rect","circle","curve"]
    property bool isMouseMoveEnable: false  //是否允许鼠标移动绘制事件

    function translate(x,y) {
        var newPointsArray = []
        for (var index in canvas.arrpoints) {
            var px = canvas.arrpoints[index]["x"]
            var py = canvas.arrpoints[index]["y"]
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
        var newPointsArray = []
        for (var index in canvas.arrpoints) {
            var theta = t*Math.PI/180;
            var px = canvas.arrpoints[index]["x"]
            var py = canvas.arrpoints[index]["y"]

            var nx = Math.cos(theta)*(px - 0.5) - Math.sin(theta)*(py-0.5) + 0.5
            var ny = Math.sin(theta)*(px - 0.5) + Math.cos(theta)*(py-0.5) + 0.5
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
        title: qsTr("选择加载Image")
        selectExisting: true
        selectFolder: false
        selectMultiple: false

        nameFilters: ["Image文件 (*.jpg || *.bmp)"]
        onAccepted: {
            console.log("Selected " + loadImageDialog.fileUrl)
            canvas.imageUrl = loadImageDialog.fileUrl
            var result = pointsTable.testModel.checkImageSize(canvas.imageUrl)
            canvas.inputWidth = result["width"]
            canvas.inputHeight = result["height"]
            canvas.requestPaint()
        }
    }

    FileDialog {
        id: saveDispensePathDialog
        title: qsTr("Save Dispense Path")
        selectExisting: false
        selectFolder: false
        selectMultiple: false

        onAccepted: {
            console.log("Selected " + fileUrl)
            pointsTable.testModel.saveOutputJson(fileUrl, pathSettingTable.model.getCurrentJsonData())
        }
    }

    FileDialog {
        id: loadDispensePathDialog
        title: qsTr("Load Dispense Path")
        selectExisting: true
        selectFolder: false
        selectMultiple: false

        onAccepted: {
            console.log("Selected " + fileUrl)
            pointsTable.testModel.loadJson(fileUrl)
        }
    }

    RowLayout {
        ColumnLayout {
            RowLayout{
                Button {
                    text: qsTr("Load Image")
                    onClicked: {
                        loadImageDialog.open()
                    }
                }
                TextField {
                    text: canvas.imageUrl
                }
            }

            RowLayout{
                Button {
                    text: qsTr("Save Dispense Path")
                    onClicked: {
                        saveDispensePathDialog.open()
                    }
                }
                Button {
                    text: qsTr("Load Dispense Path")
                    onClicked: {
                        loadDispensePathDialog.open()
                    }
                }
            }

            RowLayout{
                ComboBox {
                    id: toolSelect
                    implicitWidth: 100
                    model: ["Draw", "Select", "Move Image"]
                }
                Button {
                    text: qsTr("Clear All")
                    onClicked: {
                        canvas.arrpoints = []
                        pointsTable.testModel.refreshData(canvas.arrpoints)
                        pathSettingTable.model.clearTable()
                        canvas.requestPaint()
                    }
                }

            }

            GroupBox {
                title: 'Select Step'
                height: 100
                RowLayout{
                    CheckBox {
                        id: stepSelectionBox_1
                        text: qsTr("0.0001")
                        onCheckedChanged: {
                            if (checked) {
                                stepSelectionBox_2.checked = false
                                stepSelectionBox_3.checked = false
                            }
                        }
                    }
                    CheckBox {
                        id: stepSelectionBox_2
                        text: qsTr("0.001")
                        checked: true
                        onClicked: {
                            if (checked) {
                                stepSelectionBox_1.checked = false
                                stepSelectionBox_3.checked = false
                            }
                        }
                    }
                    CheckBox {
                        id: stepSelectionBox_3
                        text: qsTr("0.01")
                        onCheckedChanged: {
                            if (checked) {
                                stepSelectionBox_1.checked = false
                                stepSelectionBox_2.checked = false
                            }
                        }
                    }
                }
            }

            Label{
                text: qsTr(" 图像 XY 拖拽/缩放")
            }

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
                Slider {
                    from: 1
                    value: 10
                    to: canvas.inputWidth - canvas.inputWidth/canvas.scaleFactor
                    onValueChanged: {
                        canvas.imageOffsetX  = value;
                        canvas.requestPaint()
                    }
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

            Label{
                text: qsTr(" 整体拖拽/旋转")
            }

            RowLayout{
                TextField {
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
                Button {
                    text: qsTr("+X")
                    onClicked: {
                        var stepSize = 0
                        if (stepSelectionBox_1.checked) stepSize = 0.0001
                        else if (stepSelectionBox_2.checked) stepSize = 0.001
                        else stepSize = 0.01
                        translate(stepSize, 0)
                    }
                }
                Button {
                    text: qsTr("-X")
                    onClicked: {
                        var stepSize = 0
                        if (stepSelectionBox_1.checked) stepSize = 0.0001
                        else if (stepSelectionBox_2.checked) stepSize = 0.001
                        else stepSize = 0.01
                        translate(-stepSize, 0)
                    }
                }
                Button {
                    text: qsTr("+Y")
                    onClicked: {
                        var stepSize = 0
                        if (stepSelectionBox_1.checked) stepSize = 0.0001
                        else if (stepSelectionBox_2.checked) stepSize = 0.001
                        else stepSize = 0.01
                        translate(0, -stepSize)
                    }
                }
                Button {
                    text: qsTr("-Y")
                    onClicked: {
                        var stepSize = 0
                        if (stepSelectionBox_1.checked) stepSize = 0.0001
                        else if (stepSelectionBox_2.checked) stepSize = 0.001
                        else stepSize = 0.01
                        translate(0, stepSize)
                    }
                }
            }

            PointsTableWidget{
                implicitWidth: 300
                id: pointsTable
                Connections {
                    target: pointsTable.testModel
                    onDataLoaded: {
                        canvas.arrpoints = []
                        for (var index in pointsTable.testModel.initData)
                        {
                            console.log(pointsTable.testModel.initData[index]["x"])
                            canvas.arrpoints.push({"x": pointsTable.testModel.initData[index]["x"],
                                                   "y": pointsTable.testModel.initData[index]["y"],
                                                   "z": pointsTable.testModel.initData[index]["z"],
                                                   "type": "line"})
                        }

                        pathSettingTable.model.refreshData(pointsTable.testModel.velocityData)
                        canvas.requestPaint()
                    }
                    onNewPathAdded: {
                        pathSettingTable.model.addPathVelocity()
                    }
                }
            }

            PathSettingTableWidget{
                implicitWidth: 300
                id: pathSettingTable
            }
        }

        MainCanvas {
            Layout.alignment: Qt.AlignTop
            id: canvas
        }
    }
}
