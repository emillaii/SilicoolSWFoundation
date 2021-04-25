import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "../CustomizedUIElement"

Rectangle {
    property var logToViewerHandler: null

    color: "transparent"
    border.width: 1

    function __addLogViewer(categoryName){
        logViewers.addTab(categoryName, logViewComponent)
        logToViewerHandler.updateLogLevel(categoryName)
    }

    function init()
    {
        var defaultCategoryName = logToViewerHandler.getDefaultCategoryName()
        __addLogViewer(defaultCategoryName)
        var warnCategoryName = logToViewerHandler.getWanrCategoryName()
        __addLogViewer(warnCategoryName)
    }

    QtObject{
        id: self
        property var logViewPages: []
    }

    MyTabView{
        id: logViewers

        x: 2
        y: 2
        width: parent.width - 4
        height: parent.height - 4
        deleteUnvisibleTab: false
    }

    Component{
        id: logViewComponent

        LogView{
            Component.onCompleted: {
                category = parent.title
                logHandler = logToViewerHandler
                setLogModel(logToViewerHandler.getLogModel(category))
                if(category === "Warn")
                {
                    loglevelSelector.visible = false
                }
                self.logViewPages[category] = this
            }
        }
    }

    Connections{
        target: logToViewerHandler
        onNewLogModelAttached:{
            __addLogViewer(category)
        }
    }

    Connections{
        target: logToViewerHandler
        onLogLevelPublished: {
            self.logViewPages[category].setLogLevel(logLevel)
        }
    }
}
