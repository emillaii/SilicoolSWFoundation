import QtQuick 2.0

Canvas{
    id:canvas
    property real radius: 4
    property var arrpoints : []  //Store the normalized point from 0 to 1
    property var selectedPointIndex: 0
    property var imageUrl: "file:///C:/Users/emil/Documents/build-untitled19-Desktop_Qt_5_12_3_MSVC2017_64bit-Debug/debug/test.jpg"
    property bool isPressed: false

    property real imageOffsetX: 0
    property real imageOffsetY: 0

    property real scaleFactor: 1

    property int inputWidth: 3840
    property int inputHeight: 2748

    focus: true

    width: 1280
    height: 960

    Keys.onPressed: {
        console.log(event.key)
        if (toolSelect.currentText === "Select") {

            var stepSize = 0
            if (stepSelectionBox_1.checked) stepSize = parseFloat(stepSelectionBox_1.text)
            else if (stepSelectionBox_2.checked) stepSize = parseFloat(stepSelectionBox_2.text)
            else stepSize = parseFloat(stepSelectionBox_3.text)

            if (event.key === 16777235){
                var result = parseFloat(canvas.arrpoints[canvas.selectedPointIndex]["y"]) - stepSize
                canvas.arrpoints[canvas.selectedPointIndex]["y"] = result.toFixed(4)
            } else if (event.key === 16777237) {
                var result = parseFloat(canvas.arrpoints[canvas.selectedPointIndex]["y"]) + stepSize
                canvas.arrpoints[canvas.selectedPointIndex]["y"] = result.toFixed(4)
            } else if (event.key === 16777236) {
                var result = parseFloat(canvas.arrpoints[canvas.selectedPointIndex]["x"]) + stepSize
                canvas.arrpoints[canvas.selectedPointIndex]["x"] = result.toFixed(4)
            } else if (event.key === 16777234) {
                var result = parseFloat(canvas.arrpoints[canvas.selectedPointIndex]["x"]) - stepSize
                canvas.arrpoints[canvas.selectedPointIndex]["x"] = result.toFixed(4)
            } else if (event.key === 16777223) {  //Delete
                var newPointsArray = []
                for (var index in canvas.arrpoints) {
                    if (index !== canvas.selectedPointIndex) {
                        newPointsArray.push(canvas.arrpoints[index])
                    }
                }
                canvas.arrpoints = newPointsArray
            }
            pointsTable.testModel.refreshData(canvas.arrpoints)
            canvas.requestPaint()
        } else if (toolSelect.currentText === "Move Image") {
            var stepSize = 0
            if (stepSelectionBox_1.checked) stepSize = parseFloat(stepSelectionBox_1.text)
            else if (stepSelectionBox_2.checked) stepSize = parseFloat(stepSelectionBox_2.text)
            else stepSize = parseFloat(stepSelectionBox_3.text)

            if (event.key == 16777235){
                canvas.imageOffsetY -= stepSize;
                if (canvas.imageOffsetY < 0) canvas.imageOffsetY = 0
            } else if (event.key == 16777237) {
                canvas.imageOffsetY += stepSize;
                if (canvas.imageOffsetY < 0) canvas.imageOffsetY = 0
            } else if (event.key == 16777236) {
                canvas.imageOffsetX += stepSize
                if (canvas.imageOffsetX < 0) canvas.imageOffsetX = 0
            } else if (event.key == 16777234) {
                canvas.imageOffsetX -= stepSize
                if (canvas.imageOffsetX < 0) canvas.imageOffsetX = 0
            }
            console.log('canvas.imageOffset: ' + canvas.imageOffsetX + ' ' + canvas.imageOffsetY)
            canvas.requestPaint()
        }
    }

    Component.onCompleted: {
        var image = loadImage(imageUrl)
    }

    onImageLoaded: {
        console.log('finish')
        requestPaint()
    }

    function clear() {
        arrpoints = []
        var ctx = getContext("2d");
        ctx.reset();
        canvas.requestPaint();
    }


    function getCross(p1x, p1y, p2x, p2y, p3x, p3y) {
        return (p2x - p1x)*(p3y - p1y) - (p3x - p1x)*(p2y - p1y)
    }

    onPaint: {
        var context = getContext("2d")
        context.reset()

        if (scaleFactor == 1) {
            imageOffsetX = 0
            imageOffsetY = 0
        }

        var sx = (imageOffsetX)
        var sy = (imageOffsetY)

        var sw = canvas.inputWidth/canvas.scaleFactor
        var sh = canvas.inputHeight/canvas.scaleFactor

        context.drawImage(imageUrl, sx, sy, sw, sh, 0, 0, canvas.width, canvas.height)

        if(arrpoints.length > 0){

            //Drawing path between two consective points
            context.beginPath()
            var start = arrpoints[0]

            context.moveTo((start["x"]*canvas.width - sx*canvas.width)*canvas.scaleFactor/canvas.inputWidth, (start["y"]*canvas.height - sy*canvas.height)*canvas.scaleFactor/canvas.inputHeight)
            for(var j=1; j < arrpoints.length; j++){
                var end= arrpoints[j]
                context.lineTo((end["x"]*canvas.width - sx*canvas.width)*canvas.scaleFactor/canvas.inputWidth, (end["y"]*canvas.height - sy*canvas.height)*canvas.scaleFactor/canvas.inputHeight)
                context.moveTo((end["x"]*canvas.width - sx*canvas.width)*canvas.scaleFactor/canvas.inputWidth, (end["y"]*canvas.height - sy*canvas.height)*canvas.scaleFactor/canvas.inputHeight)
            }
            context.closePath()
            context.strokeStyle = "red"
            context.lineWidth = 2
            context.stroke()

            //End of drawing path between two consecutive points

            for(var i=0; i < arrpoints.length; i++){
                var point = arrpoints[i]
                var x = (point["x"]*canvas.width - sx*canvas.width)*canvas.scaleFactor/canvas.inputWidth
                var y = (point["y"]*canvas.height - sy*canvas.height)*canvas.scaleFactor/canvas.inputHeight

                context.ellipse(x-radius, y-radius, 2*radius, 2*radius)

                //Drawing cross mark
                context.beginPath()
                context.lineWidth = 2;
                context.moveTo(x - 5, y - 5)
                context.lineTo(x + 5, y + 5)
                context.moveTo(x - 5 , y + 5)
                context.lineTo(x + 5 , y - 5)
                if (selectedPointIndex == i)
                {
                    context.strokeStyle = "lime"
                }
                else
                    context.strokeStyle = "cyan"
                context.stroke()

                //End of drawing cross mark

                //Drawing label
                context.beginPath();
                context.font = "20px sans-serif";
                context.text(i , x+10 , y-10);
                context.stroke(); //设置 ↑ 的text风格为stroke

            }
        }
    }

    MouseArea{
        id: area
        anchors.fill: parent
        onClicked: {
            canvas.focus = true

            var sx = (imageOffsetX)
            var sy = (imageOffsetY)

            if (toolSelect.currentText === "Draw") {
                var nx = ( mouseX/canvas.scaleFactor + sx*canvas.width/canvas.inputWidth )/canvas.width
                var ny = ( mouseY/canvas.scaleFactor + sy*canvas.height/canvas.inputHeight )/canvas.height
                var px = nx*canvas.inputWidth
                var py = ny*canvas.inputHeight
                //ToDo: Add paint type
                pointsTable.testModel.addPoint(px.toFixed(2), py.toFixed(2), "line")
                canvas.arrpoints.push({"x": px.toFixed(2), "y": py.toFixed(2), "z": 0, "type": "line"})
                canvas.requestPaint()
            } else if (toolSelect.currentText === "Select") {
                var minDist = 999999;
                for (var index in canvas.arrpoints) {
                    var distX = ( mouseX/canvas.scaleFactor + sx*canvas.width/canvas.inputWidth )/canvas.width - canvas.arrpoints[index]["x"]/canvas.inputWidth
                    var distY = ( mouseY/canvas.scaleFactor + sy*canvas.height/canvas.inputHeight )/canvas.height - canvas.arrpoints[index]["y"]/canvas.inputHeight
                    var radius = Math.sqrt(Math.pow(distX,2) + Math.pow(distY,2))
                    if (radius < 0.02 && radius < minDist) {
                        canvas.selectedPointIndex = index
                        minDist = radius
                    }
                }
                canvas.requestPaint()
            }
        }

        onDoubleClicked: {
            if (toolSelect.currentText === "Select") {
                var minDist = 999999;
                var nx = ( mouseX/canvas.scaleFactor + imageOffsetX*canvas.width/canvas.inputWidth )/canvas.width
                var ny = ( mouseY/canvas.scaleFactor + imageOffsetY*canvas.height/canvas.inputHeight )/canvas.height
                var px = nx*canvas.inputWidth
                var py = ny*canvas.inputHeight
                var output = []
                var needUpdate = false
                for (var i = 0; i < canvas.arrpoints.length -1; i++) {
                    output.push({"x": canvas.arrpoints[i]["x"], "y": canvas.arrpoints[i]["y"], "z": canvas.arrpoints[i]["z"], "type": canvas.arrpoints[i]["type"]})
                    var dist_x = parseFloat(canvas.arrpoints[i]["x"]) - parseFloat(canvas.arrpoints[i+1]["x"])
                    if (Math.abs(dist_x) > 10)
                    {
                        var slope = (canvas.arrpoints[i]["y"] - canvas.arrpoints[i+1]["y"]) / (canvas.arrpoints[i]["x"] - canvas.arrpoints[i+1]["x"])
                        var c = canvas.arrpoints[i]["y"] - slope*canvas.arrpoints[i]["x"]
                        var dist = Math.abs(slope*px - py + c) / Math.sqrt(slope*slope + 1)

                        var p1y = parseFloat(canvas.arrpoints[i]["y"]) + 10/(Math.sqrt(1+slope*slope))
                        var p2y = parseFloat(canvas.arrpoints[i]["y"]) - 10/(Math.sqrt(1+slope*slope))

                        var p1x = parseFloat(canvas.arrpoints[i]["x"]) - slope*(p1y - parseFloat(canvas.arrpoints[i]["y"]))
                        var p2x = parseFloat(canvas.arrpoints[i]["x"]) - slope*(p2y - parseFloat(canvas.arrpoints[i]["y"]))

                        var p4y = parseFloat(canvas.arrpoints[i+1]["y"]) + 10/(Math.sqrt(1+slope*slope))
                        var p3y = parseFloat(canvas.arrpoints[i+1]["y"]) - 10/(Math.sqrt(1+slope*slope))

                        var p4x = parseFloat(canvas.arrpoints[i+1]["x"]) - slope*(p4y - parseFloat(canvas.arrpoints[i+1]["y"]))
                        var p3x = parseFloat(canvas.arrpoints[i+1]["x"]) - slope*(p3y - parseFloat(canvas.arrpoints[i+1]["y"]))

                        console.log("p1x : " + p1x + " p1y: " + p1y)
                        console.log("p2x : " + p2x + " p2y: " + p2y)
                        console.log("p3x : " + p3x + " p3y: " + p3y)
                        console.log("p4x : " + p4x + " p4y: " + p4y)

                        var cross1 = getCross(p1x, p1y, p2x, p2y, px, py)*getCross(p3x, p3y, p4x, p4y, px, py)
                        var cross2 = getCross(p2x, p2y, p3x, p3y, px, py)*getCross(p4x, p4y, p1x, p1y, px, py)


                        if (dist < 10 && !needUpdate && cross1 >=0 && cross2>=0) {
                            output.push({"x": px.toFixed(2), "y": py.toFixed(2), "z": 0 ,"type": "line"})
                            needUpdate = true
                        }
                    } else { //Vertical line
                        var p1x = parseFloat(canvas.arrpoints[i]["x"]) - 10
                        var p1y = parseFloat(canvas.arrpoints[i]["y"])

                        var p4x = parseFloat(canvas.arrpoints[i]["x"]) + 10
                        var p4y = parseFloat(canvas.arrpoints[i]["y"])


                        var p2x = parseFloat(canvas.arrpoints[i+1]["x"]) - 10
                        var p2y = parseFloat(canvas.arrpoints[i+1]["y"])

                        var p3x = parseFloat(canvas.arrpoints[i+1]["x"]) + 10
                        var p3y = parseFloat(canvas.arrpoints[i+1]["y"])

                        var cross1 = getCross(p1x, p1y, p2x, p2y, px, py)*getCross(p3x, p3y, p4x, p4y, px, py)
                        var cross2 = getCross(p2x, p2y, p3x, p3y, px, py)*getCross(p4x, p4y, p1x, p1y, px, py)

                        if (!needUpdate && cross1 >=0 && cross2>=0) {
                            output.push({"x": px.toFixed(2), "y": py.toFixed(2), "z": 0 ,"type": "line"})
                            needUpdate = true
                        }
                    }
                }
                if (needUpdate){
                    if (canvas.arrpoints.length-1 !== output.length) {
                        output.push({"x": canvas.arrpoints[canvas.arrpoints.length-1]["x"], "y": canvas.arrpoints[canvas.arrpoints.length-1]["y"], "z": canvas.arrpoints[canvas.arrpoints.length-1]["z"] ,"type": "line"})
                        canvas.arrpoints = []
                        pointsTable.testModel.refreshData(canvas.arrpoints)
                        pathSettingTable.model.clearTable()
                    }
                    for (var index in output){
                        pointsTable.testModel.addPoint(output[index]["x"], output[index]["y"], "line")
                        canvas.arrpoints.push({"x": output[index]["x"], "y": output[index]["y"], "z": 0, "type": "line"})
                    }
                }

                canvas.requestPaint()
            }
        }

        onPressAndHold: {
            if (toolSelect.currentText === "Select") {
                canvas.focus = true
                canvas.isPressed = true
            }
        }

        onReleased: {
            canvas.isPressed = false
        }

        onMouseXChanged: {
            var sx = (imageOffsetX)
            var sy = (imageOffsetY)
            if (toolSelect.currentText === "Select" && canvas.isPressed) {
                var nx = ( mouseX/canvas.scaleFactor + sx*canvas.width/canvas.inputWidth )/canvas.width
                var ny = ( mouseY/canvas.scaleFactor + sy*canvas.height/canvas.inputHeight )/canvas.height
                var px = nx*canvas.inputWidth
                var py = ny*canvas.inputHeight
                canvas.arrpoints[canvas.selectedPointIndex]["x"] = px.toFixed(2)
                canvas.arrpoints[canvas.selectedPointIndex]["y"] = py.toFixed(2)
                pointsTable.testModel.refreshData(canvas.arrpoints)
                canvas.requestPaint()
            }
        }
    }
}
