import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0

BasicConfigEditor{
    QtObject{
        id: self
        property bool isObjectArray: false
        property var events: []
        property int elementType: -1
        property var configNames: []
        property var configTitleToConfigName: []
        property bool arrayItemAddRemovable: true
    }

    Popup{
        id: batchSetCfg

        property string selectedConfig: ""

        anchors.centerIn: Overlay.overlay

        onOpened: {
            txtBatchSetValue.clear()
        }

        contentItem: ColumnLayout{
            spacing: 20
            Label {
                Layout.fillWidth: true
                font.pixelSize: 20
                color: "Black"
                text: qsTr("配置批量设置")
                background: Rectangle{
                    color: "lightGreen"
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            RowLayout{
                spacing: 10
                Label{
                    text: batchSetCfg.selectedConfig + ":"
                }
                TextField{
                    id: txtBatchSetValue
                    implicitWidth: 300
                    selectByMouse: true
                }
                Button{
                    text: qsTr("设置")
                    onClicked: {
                        configModel.batchSetConfig(self.configTitleToConfigName[batchSetCfg.selectedConfig], txtBatchSetValue.text)
                        batchSetCfg.close()
                    }
                }
            }
        }
    }

    function init(_isExpand=true)
    {
        isExpand = _isExpand
        if(configModel.configType() === ConfigElementType.ConfigObjArr){
            self.isObjectArray = true
            self.events = configModel.getEvents()
            self.configNames = configModel.getTitles()
            self.arrayItemAddRemovable = configModel.uiAddRemovable()
            var configTitles = []
            configTitles.push("index")
            for(var i in self.configNames){
                var cfgTitle = configModel.translate(self.configNames[i])
                configTitles.push(cfgTitle)
                self.configTitleToConfigName[cfgTitle] = self.configNames[i]
            }
            titleRepeter.model = configTitles
        }else{
            self.isObjectArray = false
            self.elementType = configModel.getConfigType()
        }

        var dataCount = configModel.count()
        for(var i = 0; i < dataCount; i++){
            listModel.append({"rowIndex": i, "rowData": configModel.getConfig(i)})
        }

        connConfigInserted.target = configModel
        connConfigRemoved.target = configModel
        if(!self.isObjectArray){
            connConfigChanged.target = configModel
        }
    }

    ListModel{
        id: listModel
    }

    Connections{
        id: connConfigChanged
        target: null
        onConfigChanged:{
            listModel.setProperty(index, "rowData", value)
        }
    }
    Connections{
        id: connConfigInserted
        target: null
        onConfigInserted:{
            listModel.insert(index, {"rowIndex": index, "rowData": value})
            for(var i = index; i < listModel.count; i++){
                listModel.setProperty(i, "rowIndex", i)
            }
        }
    }
    Connections{
        id: connConfigRemoved
        target: null
        onConfigRemoved:{
            listModel.remove(index)
            for(var i = index; i < listModel.count; i++){
                listModel.setProperty(i, "rowIndex", i)
            }
            configNodes.resize()
        }
    }

    RowLayout{
        id: rowAddRemove
        visible: isExpand && self.arrayItemAddRemovable
        RoundButton{
            icon.source: "qrc:/commonicons/add.png"
            icon.color: "transparent"
            onClicked: {
                var listviewIndex = configNodes.currentIndex
                if(listviewIndex >= 0 && listviewIndex < configNodes.count){
                    configModel.add(listviewIndex + 1)
                }else{
                    configModel.add(configNodes.count)
                }
            }
        }
        RoundButton{
            icon.source: "qrc:/commonicons/remove.png"
            icon.color: "transparent"
            onClicked: {
                var listviewIndex = configNodes.currentIndex
                if(listviewIndex >= 0 && listviewIndex < configNodes.count){
                    configModel.remove(listviewIndex)
                }
            }
        }
    }

    RowLayout{
        id: rowEvents
        visible: isExpand
        Repeater{
            id: eventRepeter
            delegate: Button{
                text: modelData
                onClicked: {
                    var objIndex = configNodes.currentIndex
                    if(objIndex >= 0 && objIndex < configNodes.count) {
                        configModel.handleEvent(objIndex, modelData)
                    }
                }
            }
        }
    }

    Flickable{
        visible: self.isObjectArray && isExpand && listModel.count > 0
        implicitWidth: configNodes.implicitWidth
        height: 15
        clip: true
        interactive: false
        contentX: hbar.position * configNodes.contentWidth

        Row{
            id: titleRow
            Repeater{
                id: titleRepeter
                delegate: Rectangle{
                    width: 30
                    height: 18
                    color: index % 2 == 0 ? "lightGreen" : "lightGray"
                    Text {
                        anchors.fill: parent
                        text: modelData
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideMiddle
                    }
                    ToolTip{
                        text: modelData
                        visible: mouseArea.containsMouse
                    }
                    MouseArea{
                        id: mouseArea
                        hoverEnabled: true
                        anchors.fill: parent
                        onDoubleClicked: {
                            if(index == 0){
                                return
                            }
                            batchSetCfg.selectedConfig = modelData
                            batchSetCfg.open()
                        }
                    }
                }
            }

            function resize(item)
            {
                var subItemsWidth = item.getItemsWidth()
                var itemsSpacing = item.getItemsSpacing()
                for(var i in subItemsWidth){
                    children[i].width = subItemsWidth[i] + itemsSpacing
                    children[i].implicitWidth = subItemsWidth[i] + itemsSpacing
                }
            }
        }
    }

    ListView{
        id: configNodes

        clip: true
        spacing: 5
        Layout.fillHeight: true
        model: listModel
        visible: isExpand
        implicitHeight: rowAddRemove.width
        implicitWidth: rowAddRemove.height
        contentX: hbar.position * contentWidth

        property bool isItemCreated: false
        property int itemWidth: 0
        property int itemHeight: 0
        property bool isResizeTitle: false

        function resize()
        {
            if(listModel.count == 0){
                implicitWidth = rowAddRemove.width
                implicitHeight = rowAddRemove.height
            }else{
                if(configNodes.isItemCreated){
                    contentWidth = configNodes.itemWidth + 10
                    contentHeight = (configNodes.itemHeight + spacing) * listModel.count + 5

                    if(contentWidth > listMaxWidth){
                        implicitWidth = listMaxWidth
                    }else if(contentWidth < rowAddRemove.width){
                        implicitWidth = rowAddRemove.width
                    }else{
                        implicitWidth = contentWidth
                    }
                    if(contentHeight > listMaxHeight){
                        implicitHeight = listMaxHeight
                    }else if(contentHeight < rowAddRemove.height){
                        implicitHeight = rowAddRemove.height
                    }else{
                        implicitHeight = contentHeight
                    }
                }
            }
            rectHScrollBar.width = implicitWidth
        }

        delegate: Rectangle{
            id: rect
            color: configNodes.currentIndex == rowIndex ? "darkgray" : "transparent"

            Loader{
                source: {
                    if(self.isObjectArray)
                    {
                        return "ObjectArrayRow.qml";
                    } else {
                        return "SimpleTypeArrayRow.qml";
                    }
                }

                onLoaded: {
                    item.rowModel = listModel.get(rowIndex)
                    item.index = rowIndex
                    if(self.isObjectArray){
                        item.configModel = rowData
                        item.init(self.configNames)
                        if(self.events.length > 0){
                            eventRepeter.model = self.events
                        }
                    }else{
                        item.configModel = configModel
                        item.init(self.elementType)
                    }

                    rect.width = item.width
                    rect.height = item.height

                    if(!configNodes.isItemCreated){
                        configNodes.itemWidth = rect.width
                        configNodes.itemHeight = rect.height
                        configNodes.isItemCreated = true
                    }
                    configNodes.resize()
                    if(self.isObjectArray && !configNodes.isResizeTitle){
                        titleRow.resize(item)
                        configNodes.isResizeTitle = true
                    }
                }
            }

            MouseArea{
                anchors.fill: parent
                propagateComposedEvents: true
                onPressed: {
                    configNodes.currentIndex = rowIndex
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

    Rectangle{
        id: rectHScrollBar
        height: 15
        width: 200
        visible: isExpand
        color: "transparent"

        ScrollBar{
            id: hbar
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Horizontal
            size: configNodes.implicitWidth/configNodes.contentWidth
            anchors.fill: parent
        }
    }
}
