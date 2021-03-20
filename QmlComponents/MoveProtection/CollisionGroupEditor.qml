import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../ConfigManager"
import ConfigElementType 1.0

Item {
    property var configModel: null

    QtObject{
        id: self
        property var collisionConditions: null
        property var ignoreCollisionConditions: null
    }

    function init(){
        cfgGroupDescription.configModel = configModel
        cfgEnable.configModel = configModel
        self.collisionConditions = configModel.collisionConditions
        self.ignoreCollisionConditions = configModel.ignoreCollisionConditions
        cmbCollisionConditionType.model = configModel.getCollisionConditionTypes()
        cmbComparisonOperationType.model = configModel.getComparisonOperationTypes()
        cmbIgnoreCollisionConditionType.model = configModel.getIgnoreCollisionConditionTypes()

        var dataCount = self.collisionConditions.count()
        for(var i = 0; i < dataCount; i++){
            collisionConditionsModel.append({"rowIndex": i, "rowData": self.collisionConditions.getConfig(i), "itemWidth": 0, "itemHeight": 0})
        }
        connCollisionConditionInserted.target = self.collisionConditions
        connCollisionConditionRemoved.target = self.collisionConditions

        dataCount = self.ignoreCollisionConditions.count()
        for(var j = 0; j < dataCount; j++){
            ignoreCollisionConditionsModel.append({"rowIndex": j, "rowData": self.ignoreCollisionConditions.getConfig(j)})
        }
        connIgnoreCollisionConditionInserted.target = self.ignoreCollisionConditions
        connIgnoreCollisionConditionRemoved.target = self.ignoreCollisionConditions

        cfgGroupDescription.autoInit()
        cfgEnable.autoInit()
    }

    ListModel{
        id: collisionConditionsModel
    }
    Connections{
        id: connCollisionConditionInserted
        target: null
        onConfigInserted:{
            collisionConditionsModel.insert(index, {"rowIndex": index, "rowData": value, "itemWidth": 0, "itemHeight": 0})
            for(var i = index; i < collisionConditionsModel.count; i++){
                collisionConditionsModel.setProperty(i, "rowIndex", i)
            }
        }
    }
    Connections{
        id: connCollisionConditionRemoved
        target: null
        onConfigRemoved:{
            collisionConditionsModel.remove(index)
            for(var i = index; i < collisionConditionsModel.count; i++){
                collisionConditionsModel.setProperty(i, "rowIndex", i)
            }
            collisionConditionsView.resize()
        }
    }

    ListModel{
        id: ignoreCollisionConditionsModel
    }
    Connections{
        id: connIgnoreCollisionConditionInserted
        target: null
        onConfigInserted:{
            ignoreCollisionConditionsModel.insert(index, {"rowIndex": index, "rowData": value})
            for(var i = index; i < ignoreCollisionConditionsModel.count; i++){
                ignoreCollisionConditionsModel.setProperty(i, "rowIndex", i)
            }
        }
    }
    Connections{
        id: connIgnoreCollisionConditionRemoved
        target: null
        onConfigRemoved:{
            ignoreCollisionConditionsModel.remove(index)
            for(var i = index; i < ignoreCollisionConditionsModel.count; i++){
                ignoreCollisionConditionsModel.setProperty(i, "rowIndex", i)
            }
        }
    }

    ColumnLayout{
        anchors.fill: parent

        RowLayout{
            ConfigRow{
                id: cfgEnable
                configName: "enable"
            }
            ConfigRow{
                id: cfgGroupDescription
                txtWidth: 500
                configName: "groupDescription"
            }
        }

        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout{
                Layout.fillHeight: true

                Rectangle{
                    width: lblCollisionConditions.width + 10
                    height: lblCollisionConditions.height + 6
                    color: "#21be2b"
                    Label {
                        id: lblCollisionConditions
                        text: qsTr("CollisionConditions")
                        anchors.centerIn: parent
                    }
                }

                RowLayout{
                    Label{
                        text: qsTr("collisionConditionType")
                    }
                    ComboBox{
                        implicitWidth: 250
                        id: cmbCollisionConditionType
                    }
                }
                RowLayout{
                    Label{
                        text: qsTr("comparisonOperationType")
                    }
                    ComboBox{
                        implicitWidth: 250
                        id: cmbComparisonOperationType
                    }
                }
                RowLayout{
                    RoundButton{
                        icon.source: "qrc:/commonicons/add.png"
                        icon.color: "transparent"
                        onClicked: {
                            var viewIndex = collisionConditionsView.currentIndex
                            if(viewIndex < 0 || viewIndex >= collisionConditionsView.count){
                                viewIndex = collisionConditionsView.count - 1
                            }
                            self.collisionConditions.add(viewIndex + 1, cmbCollisionConditionType.currentText, cmbComparisonOperationType.currentText)
                        }
                    }
                    RoundButton{
                        icon.source: "qrc:/commonicons/remove.png"
                        icon.color: "transparent"
                        onClicked: {
                            var viewIndex = collisionConditionsView.currentIndex
                            if(viewIndex >= 0 && viewIndex < collisionConditionsView.count){
                                self.collisionConditions.remove(viewIndex)
                            }
                        }
                    }
                }

                GridView{
                    id: collisionConditionsView

                    width: 850
                    height: 500

                    clip: true
                    model: collisionConditionsModel

                    function resize(){
                        var maxItemWidth = 0
                        var maxItemHeight = 0
                        for(var i = 0; i < collisionConditionsModel.count; i++){
                            var listItem = collisionConditionsModel.get(i)
                            if(listItem.itemWidth > maxItemWidth){
                                maxItemWidth = listItem.itemWidth
                            }
                            if(listItem.itemHeight > maxItemHeight){
                                maxItemHeight = listItem.itemHeight
                            }
                        }
                        cellWidth = maxItemWidth + 10
                        cellHeight = maxItemHeight + 10
                    }

                    delegate: Rectangle{
                        id: rectCollisionCondition
                        color: collisionConditionsView.currentIndex == rowIndex ? palette.highlight : "transparent"
                        border.width: 1

                        property var collisionConditionItem: null
                        property var cmpOperationItem: null

                        function resize(){
                            if(collisionConditionItem != null && cmpOperationItem != null){
                                if(collisionConditionItem.width > cmpOperationItem.width){
                                    rectCollisionCondition.width = collisionConditionItem.width + 10
                                }else{
                                    rectCollisionCondition.width = cmpOperationItem.width + 10
                                }
                                rectCollisionCondition.height = collisionConditionItem.height + cmpOperationItem.height + 10

                                var listItem = collisionConditionsModel.get(rowIndex)
                                listItem.itemWidth = rectCollisionCondition.width
                                listItem.itemHeight = rectCollisionCondition.height

                                collisionConditionsView.resize()
                            }
                        }

                        ColumnLayout{
                            anchors.centerIn: parent
                            Loader{
                                source: {
                                    var collisionConditionType = rowData.collisionConditionType
                                    if(collisionConditionType === "SingleAxisCollisionCondition"){
                                        return "CollisionConditionEditor/SingleAxisCollision.qml"
                                    }
                                    if(collisionConditionType === "TwoAxisCollisionCondition"){
                                        return "CollisionConditionEditor/TwoAxisCollision.qml"
                                    }
                                    if(collisionConditionType === "CylinderCollisionCondition"){
                                        return "CollisionConditionEditor/CylinderCollision.qml"
                                    }
                                    if(collisionConditionType === "DICollisionCondition"){
                                        return "CollisionConditionEditor/DiColllision.qml"
                                    }
                                }

                                onLoaded: {
                                    item.configModel = rowData
                                    item.init()
                                    rectCollisionCondition.collisionConditionItem = item
                                    rectCollisionCondition.resize()
                                }
                            }

                            Loader{
                                source: {
                                    var cmpOperationType = rowData.getCmpOperation().cmpOperationType
                                    if(cmpOperationType === "LessThanPos" ||
                                            cmpOperationType === "GreaterThanPos"){
                                        return "CmpOperationEditor/CmpWithPos.qml"
                                    }
                                    if(cmpOperationType === "InPosRange" ||
                                            cmpOperationType === "NotInPosRange"){
                                        return "CmpOperationEditor/CmpWithPosRange.qml"
                                    }
                                    if(cmpOperationType === "NotInState"){
                                        return "CmpOperationEditor/CmpWithCylState.qml"
                                    }
                                }

                                onLoaded: {
                                    item.configModel = rowData.getCmpOperation()
                                    item.collisionCondition = rowData
                                    item.init()
                                    rectCollisionCondition.cmpOperationItem = item
                                    rectCollisionCondition.resize()
                                }
                            }
                        }

                        MouseArea{
                            anchors.fill: parent
                            propagateComposedEvents: true
                            onPressed: {
                                collisionConditionsView.currentIndex = rowIndex
                                mouse.accepted = false
                            }
                            onReleased: {
                                mouse.accepted = false
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar{
                    }
                }
            }

            ColumnLayout{
                Layout.fillHeight: true

                Rectangle{
                    width: lblIgnoreCollisionConditions.width + 10
                    height: lblIgnoreCollisionConditions.height + 6
                    color: "#21be2b"
                    Label {
                        id: lblIgnoreCollisionConditions
                        text: qsTr("IgnoreCollisionConditions")
                        anchors.centerIn: parent
                    }
                }

                RowLayout{
                    Label{
                        text: qsTr("ignoreCollisionConditionType")
                    }
                    ComboBox{
                        id: cmbIgnoreCollisionConditionType
                    }
                }

                RowLayout{
                    RoundButton{
                        icon.source: "qrc:/commonicons/add.png"
                        icon.color: "transparent"
                        onClicked: {
                            var viewIndex = ignoreCollisionConditionsView.currentIndex
                            if(viewIndex < 0 || viewIndex >= ignoreCollisionConditionsView.count){
                                viewIndex = ignoreCollisionConditionsView.count - 1
                            }
                            self.ignoreCollisionConditions.add(viewIndex + 1, cmbIgnoreCollisionConditionType.currentText)
                        }
                    }
                    RoundButton{
                        icon.source: "qrc:/commonicons/remove.png"
                        icon.color: "transparent"
                        onClicked: {
                            var viewIndex = ignoreCollisionConditionsView.currentIndex
                            if(viewIndex >= 0 && viewIndex < ignoreCollisionConditionsView.count){
                                self.ignoreCollisionConditions.remove(viewIndex)
                            }
                        }
                    }
                }

                GridView{
                    id: ignoreCollisionConditionsView

                    width: 400
                    height: 550

                    clip: true

                    model: ignoreCollisionConditionsModel

                    delegate: Rectangle{
                        id: rectIgnoreCollisionCondition
                        color: ignoreCollisionConditionsView.currentIndex == rowIndex ? palette.highlight : "transparent"
                        border.width: 1

                        Loader{
                            anchors.centerIn: parent
                            source: {
                                var ignoreCollisionConditionType = rowData.ignoreCollisionConditionType
                                if(ignoreCollisionConditionType === "Axis"){
                                    return "IgnoreCollisionConditionEditor/AxisIgnoreCollisionCondition.qml"
                                }
                                if(ignoreCollisionConditionType === "Cylinder"){
                                    return "IgnoreCollisionConditionEditor/CylIgnoreCollisionCondition.qml"
                                }
                            }

                            onLoaded: {
                                item.configModel = rowData
                                item.init()
                                rectIgnoreCollisionCondition.width = item.width + 10
                                rectIgnoreCollisionCondition.height = item.height + 10
                                if(rectIgnoreCollisionCondition.width > ignoreCollisionConditionsView.cellWidth){
                                    ignoreCollisionConditionsView.cellWidth = rectIgnoreCollisionCondition.width + 10
                                }
                                if(rectIgnoreCollisionCondition.height > ignoreCollisionConditionsView.cellHeight){
                                    ignoreCollisionConditionsView.cellHeight = rectIgnoreCollisionCondition.height + 10
                                }
                            }
                        }

                        MouseArea{
                            anchors.fill: parent
                            propagateComposedEvents: true
                            onPressed: {
                                ignoreCollisionConditionsView.currentIndex = rowIndex
                                mouse.accepted = false
                            }
                            onReleased: {
                                mouse.accepted = false
                            }
                        }
                    }

                    ScrollBar.vertical: ScrollBar{
                    }
                }
            }
        }
    }
}
