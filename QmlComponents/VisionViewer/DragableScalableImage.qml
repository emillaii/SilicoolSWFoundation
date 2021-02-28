import QtQuick 2.0

Image {
    id: img

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        drag.target: img

        onWheel: {
            var delta = wheel.angleDelta.y / 120
            if(delta > 0){
                img.scale = img.scale / 0.9
            }
            if(delta < 0){
                if(img.scale < 0.5){
                    return
                }
                img.scale = img.scale * 0.9
            }
        }
        onDoubleClicked: {
            img.scale = 1
            img.x = 0
            img.y = 0
        }
    }
}
