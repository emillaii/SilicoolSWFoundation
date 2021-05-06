import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

CheckBox {
    property var dataModel: null
    property var identity: null

    implicitHeight: 40

    Connections{
        id: connAuthority
        target: null
        onCurrentAuthorityChanged: {
            updateAuthority()
        }
    }

    function init(dataModelIsConfigObj=true)
    {
        if(dataModelIsConfigObj){
            dataModel.connectNotifySignal(identity, this, "updateSelf")
        }
        if(dataModel.isReadOnly(identity)){
            enabled = false
        }else{
            if(dataModel.needEngineerAuthority(identity)){
                updateAuthority()
                connAuthority.target = userManagement
            }
        }

        updateSelf()
    }

    function updateAuthority(){
        enabled = (userManagement.currentAuthority >= 2)
    }

    function updateSelf()
    {
        checked = dataModel.getConfig(identity)
    }

    onClicked: {
        if(!dataModel.setConfig(identity, checked)){
            updateSelf()
        }
    }
}
