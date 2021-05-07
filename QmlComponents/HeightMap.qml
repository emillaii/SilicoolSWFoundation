import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ColumnLayout{
    id: root
    property int tableMaxWidth: 500
    property int tableMaxHeight: 400
    property int cellMaxSize: 40
    property alias viewer: viewer
    property string cellColor: "red"

    property int __cellSize: Math.min(Math.min(tableMaxWidth / viewer.columns, tableMaxHeight/ viewer.rows), cellMaxSize)

    Label{
        text: "Min: " + viewer.model.minValueRowIndex + ", " + viewer.model.minValueColIndex + ", "
              + viewer.model.minValue + "  Max: " + viewer.model.maxValueRowIndex + ", " + viewer.model.maxValueColIndex + ", "
              + viewer.model.maxValue + "  Range: " + (viewer.model.maxValue - viewer.model.minValue)
        Layout.alignment: Qt.AlignHCenter
    }

    TableView{
        id: viewer

        implicitWidth: __cellSize * rows
        implicitHeight: __cellSize * columns

        delegate: Rectangle{
            implicitWidth: __cellSize
            implicitHeight: __cellSize
            color: isValid ? cellColor : "gray"
            opacity: isValid ? processedHeight : 1
            ToolTip.visible: isValid && mouseArea.containsMouse
            ToolTip.text: rawHeight
            MouseArea{
                id: mouseArea
                anchors.fill: parent
            }
        }
    }

    Connections{
        id: connSaveImage
        target: null
        onReqSaveToImage: {
            root.grabToImage(function(result){
                if(!result.saveToFile(imageName)){
                    console.error("Save height map image failed!")
                }
            })
        }
    }
    Component.onCompleted: {
        connSaveImage.target = viewer.model
    }
}
