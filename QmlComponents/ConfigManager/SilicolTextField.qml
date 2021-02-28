import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import ConfigElementType 1.0
import QtQuick.Dialogs 1.2

RowLayout{
    id: root
    property var dataModel: null
    property var identity: null
    property int txtWidth: -1

    property alias txt: txt

    QtObject{
        id: self
        property bool selectFileOrFolder: false
        property bool isSelectFile: false
        property bool isDouble: false
        property string shortDoubleString: ""
        property string fullDoubleString: ""
        property var dialog: null
    }

    function init(dataModelIsConfigObj=true)
    {
        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }
        if(dataModel.isReadOnly(identity)){
            txt.readOnly = true
        }
        var isSelectFile = dataModel.isSelectFile(identity)
        var isSelectFolder = dataModel.isSelectFolder(identity)
        self.selectFileOrFolder = isSelectFile || isSelectFolder
        self.isSelectFile = isSelectFile

        txt.implicitHeight = 40
        var propertyType = dataModel.getConfigType(identity)
        if(propertyType === ConfigElementType.Int) {
            txt.validator = intValidator
            txt.implicitWidth = txtWidth > 0 ? txtWidth : 80
        }else if(propertyType === ConfigElementType.Double){
            txt.validator = doubleValidator
            txt.implicitWidth = txtWidth > 0 ? txtWidth : 120
            self.isDouble = true
        }else if(propertyType === ConfigElementType.Other){
            if(self.selectFileOrFolder){
                txt.implicitWidth = txtWidth > 0 ? txtWidth : 300
                createDialog()
                createButton()
            }else{
                txt.implicitWidth = txtWidth > 0 ? txtWidth : 200
            }
        }

        updateSelf()
    }

    function updateSelf()
    {
        if(self.isDouble){
            var v = dataModel.getConfig(identity)
            self.shortDoubleString = v.toLocaleString()
            self.fullDoubleString = v.toString()
            txt.text = self.shortDoubleString
        }else{
            txt.text = dataModel.getConfig(identity)
        }
    }

    function createButton()
    {
        Qt.createQmlObject('import QtQuick 2.0;
                            import QtQuick.Controls 2.5;
                            Button{
                                    text: "...";
                                    implicitWidth:40;
                                    implicitHeight:40;
                                    onClicked: {
                                        self.dialog.open()
                                    }
                                }', root, "dynamicCreation")
    }

    function createDialog()
    {
        self.dialog = Qt.createQmlObject('import QtQuick 2.0;
                                          import QtQuick.Dialogs 1.2;
                                          FileDialog{
                                                title: "Please select " + identity;
                                                selectFolder: !self.isSelectFile;
                                                onAccepted: {
                                                    var selectedPath = ""
                                                    if(self.isSelectFile){
                                                    selectedPath = fileUrl.toString();
                                                    }else{
                                                    selectedPath = folder.toString();
                                                    }
                                                    selectedPath = selectedPath.substring(8);   // remove file:///
                                                    selectedPath = dataModel.convertToGivenPathFormat(identity, selectedPath)
                                                    if(!dataModel.setConfig(identity, selectedPath)){
                                                        updateSelf()
                                                    }
                                                  }
                                            }', root, "dynamicCreation")
    }

    TextField {

        id: txt
        onEditingFinished: {
            if(!dataModel.setConfig(identity, text)){
                updateSelf()
            }
        }

        IntValidator{
            id: intValidator
        }
        DoubleValidator{
            id: doubleValidator
            notation: DoubleValidator.StandardNotation
        }
        ToolTip{
            text: self.selectFileOrFolder ? parent.text : self.fullDoubleString
            visible: parent.hovered && (self.selectFileOrFolder || (self.isDouble && self.fullDoubleString != self.shortDoubleString))
        }

        selectByMouse: true
    }
}
