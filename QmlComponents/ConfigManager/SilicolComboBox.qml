import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ComboBox {
    property var dataModel: null
    property var identity: null

    editable: true
    QtObject{
        id: self
        property var options: []
        property bool isInit: false
    }
    Label{
        id: widthCalc
        visible: false
        function getWidth(txt){
            text = txt
            return contentWidth * 1.5
        }
    }

    Connections{
        id: connAuthority
        target: null
        onCurrentAuthorityChanged: {
            updateAuthority()
        }
    }

    function init(dataModelIsConfigObj=true)
    {
        self.options = dataModel.getOptions(identity)
        model = self.options

        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }
        if(dataModel.needEngineerAuthority(identity)){
            updateAuthority()
            connAuthority.target = userManagement
        }

        var maxTxtWidth = 0
        for(var i = 0; i < self.options.length; i++){
            var txtWidth = widthCalc.getWidth(self.options[i])
            if(txtWidth > maxTxtWidth){
                maxTxtWidth = txtWidth
            }
        }
        if(maxTxtWidth > width){
            popup.width = maxTxtWidth + 10
        }

        updateSelf()
        self.isInit = true
    }

    function updateAuthority(){
        enabled = (userManagement.currentAuthority >= 2)
    }

    function updateSelf()
    {
        var propertyValue = dataModel.getConfig(identity)
        for(var i = 0; i < self.options.length; i++){
            if(propertyValue === self.options[i]){
                currentIndex = i;
                return;
            }
        }
        currentIndex = -1;  //did not find correct option
    }

    onCurrentIndexChanged: {
        if(!self.isInit){
            return
        }
        if(!dataModel.setConfig(identity, self.options[currentIndex])){
            updateSelf()
        }
    }
}
