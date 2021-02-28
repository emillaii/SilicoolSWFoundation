import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Item {
    id: motionDebugger
    property var pageInfos: []
    property var diDebuggers: []
    property var doDebuggers: []
    property var axisDebuggers: []
    property var cylDebuggers: []
    property var vacuumDebuggers: []

    function init(){
        var pageCount = masterMotionManager.motionElementUIPageCount()
        for(var i = 0; i < pageCount; i++){
            var pageInfo = masterMotionManager.getMotionElementUIPageInfo(i)
            pageInfos[pageInfo["pageName"]] = pageInfo
            var elementType = pageInfo["elementType"]
            if(elementType === "DI" || elementType === "DO"){
                tabview.addTab(pageInfo["pageName"], ioComponent)
            }
            if(elementType === "Axis"){
                tabview.addTab(pageInfo["pageName"], axisComponent)
            }
            if(elementType === "Cylinder"){
                tabview.addTab(pageInfo["pageName"], cylComponent)
            }
            if(elementType === "Vacuum"){
                tabview.addTab(pageInfo["pageName"], vacuumComponent)
            }
        }
        masterMotionManager.subscribeMotionState(this, "updateMotionState")
    }

    function updateMotionState(motionState){
        var i = 0
        var axisStates = motionState["axisStates"]
        for(i = 0; i < axisDebuggers.length; i++){
            axisDebuggers[i].updateMotionState(axisStates)
        }

        var diStates = motionState["diStates"]
        for(i = 0; i < diDebuggers.length; i++){
            diDebuggers[i].updateMotionState(diStates)
        }

        var doStates = motionState["doStates"]
        for(i = 0; i < doDebuggers.length; i++){
            doDebuggers[i].updateMotionState(doStates)
        }

        var cylStates = motionState["cylStates"]
        for(i = 0; i < cylDebuggers.length; i++){
            cylDebuggers[i].updateMotionState(cylStates)
        }

        var vacuumStates = motionState["vacuumStates"]
        for(i = 0; i < vacuumDebuggers.length; i++){
            vacuumDebuggers[i].updateMotionState(vacuumStates)
        }
    }

    Component{
        id: axisComponent
        AxisDebugger{
            Component.onCompleted: {
                x = 5
                y = 10
                width = motionDebugger.width - 20
                height = motionDebugger.height - 40
                var pageName = parent.title
                init(pageInfos[pageName]["pageElements"])
                axisDebuggers.push(this)
            }
        }
    }

    Component{
        id: ioComponent
        IODebugger{
            Component.onCompleted: {
                x = 5
                y = 10
                width = motionDebugger.width - 20
                height = motionDebugger.height - 40
                var pageName = parent.title
                isDigitalOutput = (pageInfos[pageName]["elementType"] === "DO")
                init(pageInfos[pageName]["pageElements"])
                if(isDigitalOutput){
                    doDebuggers.push(this)
                }else{
                    diDebuggers.push(this)
                }
            }
        }
    }

    Component{
        id: cylComponent
        CylDebugger{
            Component.onCompleted: {
                x = 5
                y = 10
                width = motionDebugger.width - 20
                height = motionDebugger.height - 40
                var pageName = parent.title
                init(pageInfos[pageName]["pageElements"])
                cylDebuggers.push(this)
            }
        }
    }

    Component{
        id: vacuumComponent
        VacuumDebugger{
            Component.onCompleted: {
                x = 5
                y = 10
                width = motionDebugger.width - 20
                height = motionDebugger.height - 40
                var pageName = parent.title
                init(pageInfos[pageName]["pageElements"])
                vacuumDebuggers.push(this)
            }
        }
    }

    MyTabView{
        id: tabview

        anchors.fill: parent
    }

    Component.onCompleted: {
        init()
    }
}
