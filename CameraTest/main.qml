import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import LanguageConfig 1.0
import SMD 1.0
import "qrc:/qmlComponents"
import "qrc:/qmlComponents/UserManagement"
import "qrc:/qmlComponents/MoveProtection"
import "qrc:/qmlComponents/MotionViewer"
import "qrc:/qmlComponents/LogViewer"
import "qrc:/qmlComponents/ConfigManager"
import "qrc:/qmlComponents/CustomizedUIElement"
import "qrc:/qmlComponents/CustomizedUIElement/RowColumnSelectorView"
import "qrc:/qmlComponents/VisionViewer"
import "qrc:/qmlComponents/LaserViewer"
import "qrc:/qmlComponents/XYZRDebugger"

ApplicationWindow {
    id: mainWindow

    RowColInfo {
        id: trayRowColInfo
    }

    visible: true
    width: 1500
    height: 900

    property string __blank: "          "
    title: qsTr("Dummy") + __blank + qsTr("Version: ") + appVersion + __blank
           + qsTr("User: ") + userManagement.currentUserName + __blank
           + qsTr("Authority: ") + userManagement.currentAuthorityName

    SilicolMsgBox {
        id: silicolMsgBox

        uiOperationImplementation: uiOperationImpl
        Component.onCompleted: {
            uiOperation.setGrabMouseItem(silicolMsgBox.contentItem)
        }
    }
	DispatcherBuzyIndicator{}

    Connections {
        target: msgBoxModel
        onMsgBoxCountChanged: {
            if (count > 0) {
                silicolMsgBox.msgBoxCount = count
                silicolMsgBox.open()
            } else {
                silicolMsgBox.close()
            }
        }
    }

    Repeater {
        id: xyzrDebuggerContainer

        property var __dbgs: []
        model: xyzrDebuggerNames
        delegate: XYZRDebugger {
            xyzrDebugger: xyzrDebuggerManager.debugger(modelData)
            xyzrDebuggerConfig: xyzrDebuggerManager.debuggerConfig(modelData)
            x: (mainWindow.width - implicitWidth) / 2
            y: (mainWindow.height - implicitHeight) / 2 - header.implicitHeight
               - menuBar.implicitHeight
            Component.onCompleted: {
                xyzrDebuggerContainer.__dbgs[modelData] = this
            }
        }

        function getDebugger(dbgName) {
            if (dbgName in __dbgs) {
                return __dbgs[dbgName]
            } else {
                console.error("Undefined XYZRDebugger: " + dbgName)
                return null
            }
        }
    }

    Repeater {
        id: xyzrDebuggerShortcutsContainer

        function getShortcut(shortcutName) {
            if (shortcutName === "") {
                return null
            }
            if (shortcutName in __shortcuts) {
                return __shortcuts[shortcutName]
            } else {
                console.error("Undefined shortcut: " + shortcutName)
                return null
            }
        }

        property var __shortcuts: []
        model: debuggerShortcuts
        delegate: Item {
            Action {
                shortcut: modelData
                Component.onCompleted: {
                    xyzrDebuggerShortcutsContainer.__shortcuts[modelData] = this
                }
            }
        }
    }

    Login {
        id: popupLogin
    }
    UserManagement {
        id: popupUserManagement
    }
    Popup {
        id: popupDutTypeManager
        contentItem: DutTypeManagerViewer {
        }
        x: 100
        y: 100
    }
    Popup {
        id: popupMotionElementDef
        contentItem: MotionElementDefinition {
            configModel: motionElementDefs
            tip.text: qsTr("Press 'ESC' to exit")
            Component.onCompleted: {
                init()
            }
        }

        contentWidth: 900
        contentHeight: 800
        closePolicy: Popup.CloseOnEscape
    }
    Popup {
        id: popupMotionConfigger
        contentItem: MotionConfigger {
            lblTip.text: qsTr("Press 'ESC' to exit")
        }
        contentWidth: 1250
        contentHeight: 800
        closePolicy: Popup.CloseOnEscape
    }
    Popup {
        id: popupMoveProtectionEditor
        contentItem: MoveProtectionEditor {
            configModel: collisionGroupConfigs
            tip.text: qsTr("Press ESC to exit")
            btnUpdateMoveProtection.onClicked: {
                masterMotionManager.updateMoveProtections()
            }
            Component.onCompleted: {
                init()
            }
        }
        contentWidth: 1300
        contentHeight: 850
        closePolicy: Popup.CloseOnEscape
    }
    Popup {
        id: popupMotionDebugger
        contentItem: SCMotionDebugger {
        }
        contentWidth: 800
        contentHeight: 880
        onOpened: {
            masterMotionManager.setMotionStateReporters(true)
        }
        onClosed: {
            masterMotionManager.setMotionStateReporters(false)
        }
    }
    Popup {
        id: popupVisionElementDefinition
        contentItem: ObjectEditor {
            configModel: visionElementDefinition
            sectionName: qsTr("VisionElementDefinition")
            Component.onCompleted: {
                init()
            }
        }
    }
    Popup {
        id: popupXYZRDefinition
        contentItem: ArrayEditor {
            configModel: xyzrDebuggerDefs
            sectionName: qsTr("XYZRDebuggerDefinition")
            Component.onCompleted: {
                init()
            }
        }
    }
    Popup {
        id: popupMachineStateIndicatorConfig
        contentItem: ObjectEditor {
            listMaxHeight: 450
            configModel: msiConfig
            sectionName: qsTr("MachineStateIndicatorConfig")
            Component.onCompleted: {
                init()
            }
        }
    }

    Popup {
        id: popupVisionConfigger
        contentItem: VisionConfigger {
        }
        contentWidth: 1250
        contentHeight: 800
    }
    Popup {
        id: popupWordopLight
        contentItem: WordopLscViewer {
        }
    }
    Popup {
        id: popupVisionToolConfigger
        contentItem: ArrayEditor {
            sectionName: qsTr("VisionToolConfig")
            configModel: externExeConfigs
            Component.onCompleted: {
                init()
            }
        }
    }
    Popup {
        id: popupLanguagePackageNames
        contentItem: ArrayEditor {
            sectionName: "LanguagePackageNames"
            configModel: languagePackageNames
            Component.onCompleted: {
                init()
            }
        }
        x: 100
        y: 100
    }
    Popup {
        id: popupRunParameter
        anchors.centerIn: parent
        contentItem: ColumnLayout {
            GroupBox {
                title: qsTr("RunMode")
                RowLayout {
                    RadioButton {
                        id: rdbNormal
                        text: qsTr("Normal")
                        checked: true
                    }
                    RadioButton {
                        id: rdbDryRun
                        text: qsTr("DryRun")
                    }
                }
            }
            RowLayout {
                Button {
                    text: qsTr("Ok")
                    onClicked: {
                        popupRunParameter.close()
                        var runParameter = {

                        }
                        if (rdbNormal.checked) {
                            runParameter["RunMode"] = "Normal"
                        } else {
                            runParameter["RunMode"] = "DryRun"
                        }
                        stateMachine.setRunningParameter(runParameter)
                        stateMachine.postEvent(SMD.Start)
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    onClicked: {
                        popupRunParameter.close()
                    }
                }
            }
        }
    }
    WorkerRunStatusViewer {id: popupWorkersRunStatus}

    menuBar: MenuBar {
        Menu {
            title: qsTr("Config")
            MenuItem {
                width: implicitWidth
                enabled: userManagement.currentAuthority >= 3
                text: qsTr("MotionElementDefinition")
                onClicked: {
                    popupMotionElementDef.open()
                }
            }
            MenuItem {
                width: implicitWidth
                enabled: userManagement.currentAuthority >= 3
                text: qsTr("VisionElementDefinition")
                onClicked: {
                    popupVisionElementDefinition.open()
                }
            }
            MenuItem {
                width: implicitWidth
                enabled: userManagement.currentAuthority >= 3
                text: qsTr("XYZRDebuggerDefinition")
                onClicked: {
                    popupXYZRDefinition.open()
                }
            }
            MenuItem {
                width: implicitWidth
                enabled: userManagement.currentAuthority >= 3
                text: qsTr("MachineStateIndicator")
                onClicked: {
                    popupMachineStateIndicatorConfig.open()
                }
            }
            MenuItem {
                text: qsTr("UserManagement")
                onClicked: {
                    popupUserManagement.clearText()
                    popupUserManagement.open()
                }
            }
            MenuItem {
                text: qsTr("DutTypeManager")
                onClicked: {
                    popupDutTypeManager.open()
                }
            }
            MenuItem {
                text: qsTr("MotionConfig")
                onClicked: {
                    popupMotionConfigger.open()
                }
            }
            MenuItem {
                text: qsTr("MoveProtection")
                onClicked: {
                    popupMoveProtectionEditor.open()
                }
            }
            MenuItem {
                text: qsTr("VisionConfig")
                onClicked: {
                    popupVisionConfigger.open()
                }
            }
            MenuItem {
                text: "WordopLight"
                onClicked: {
                    popupWordopLight.open()
                }
            }
        }

        Menu {
            title: qsTr("Tool")
            MenuItem {
                enabled: userManagement.currentAuthority >= 3
                text: qsTr("ToolConfig")
                onClicked: {
                    popupVisionToolConfigger.open()
                }
            }
            Repeater {
                model: exeLaucher.externExeNames()
                delegate: MenuItem {
                    text: modelData
                    onClicked: {
                        exeLaucher.open(modelData)
                    }
                }
            }
        }

        Menu {
            title: qsTr("XYZRDebugger")

            Repeater {
                model: xyzrDebuggerNames
                delegate: MenuItem {
                    id: __menuItem
                    text: modelData
                    onClicked: {
                        var dbg = xyzrDebuggerContainer.getDebugger(modelData)
                        dbg.open()
                    }
                    Component.onCompleted: {
                        var sc = xyzrDebuggerManager.debuggerConfig(
                                    modelData).shortcut
                        if (sc !== "") {
                            __menuItem.text += "  " + sc
                            action = xyzrDebuggerShortcutsContainer.getShortcut(
                                        sc)
                        }
                    }
                }
            }
        }

        Menu {
            title: qsTr("Language")
            MenuItem {
                text: qsTr("LanguagePackageNames")
                enabled: userManagement.currentAuthority >= 3
                onClicked: {
                    popupLanguagePackageNames.open()
                }
            }
            MenuItem {
                text: "简体中文"
                onClicked: {
                    languageConfig.setLanguage(LanguageConfig.Chinese)
                }
            }
            MenuItem {
                text: "English"
                onClicked: {
                    languageConfig.setLanguage(LanguageConfig.English)
                }
            }
        }

        Menu {
            title: qsTr("Skin")

            Repeater {
                model: ["Default", "Fusion", "Material", "Universal"]
                delegate: MenuItem {
                    text: modelData + (basicConfig.skin == modelData ? "    √" : "")
                    onClicked: {
                        basicConfig.setSkin(modelData)
                    }
                    ToolTip {
                        text: qsTr("You have to restart the software to apply the skin change!")
                        visible: parent.hovered
                    }
                }
            }
        }

        Menu {
            title: qsTr("Advanced")
            MenuItem {
                text: qsTr("WorkersRunStatus") + " Ctrl+R"
                action: Action {
                    shortcut: "Ctrl+R"
                }
                onClicked: {
                    popupWorkersRunStatus.open()
                }
            }
        }
    }

    header: ToolBar {
        RowLayout {
            MyToolButton {
                text: {
                    if (userManagement.hasLogin) {
                        return qsTr("Logout")
                    } else {
                        return qsTr("Login")
                    }
                }
                icon.source: "qrc:/commonicons/login.png"
                icon.color: userManagement.hasLogin ? "limegreen" : "red"
                onClicked: {
                    if (userManagement.hasLogin) {
                        userManagement.logout()
                    } else {
                        popupLogin.clearText()
                        popupLogin.open()
                    }
                }
            }
            MyToolButton {
                text: qsTr("Undo")
                icon.source: "qrc:/commonicons/undo.png"
                icon.color: configManager.commandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.undo()
                }
            }
            MyToolButton {
                text: qsTr("Redo")
                icon.source: "qrc:/commonicons/redo.png"
                icon.color: configManager.undoCommandCount > 0 ? "transparent" : "gray"
                onClicked: {
                    configManager.redo()
                }
            }
            MyToolButton {
                text: qsTr("Motion")
                icon.source: "qrc:/commonicons/debug.png"
                icon.color: "transparent"
                onClicked: {
                    popupMotionDebugger.open()
                }
            }
            MyToolButton {
                text: qsTr("急停")
                icon.source: "qrc:/commonicons/emo.png"
                icon.color: "transparent"
                onClicked: {
                    motionManager.stopAllAxis()
                }
                ToolTip{
                    visible: parent.hovered
                    text: qsTr("停止所有轴")
                }
            }
            MyToolButton {
                text: qsTr("清报警")
                icon.source: "qrc:/commonicons/resetDriver.png"
                icon.color: "transparent"
                onClicked: {
                    motionManager.clearAllAxisAlarm()
                }
                ToolTip{
                    visible: parent.hovered
                    text: qsTr("清除所有驱动报警")
                }
            }
            MyToolButton {
                text: qsTr("InitDev")
                icon.source: "qrc:/commonicons/initial.png"
                icon.color: "blue"
                onClicked: {
                    tem.runSingleCmd(dispatcher, "initDevice", [true])
                }
                ToolTip {
                    text: qsTr("OnlyInitDevice")
                    visible: parent.hovered
                }
            }
            MyToolButton {
                text: qsTr("Home")
                icon.source: "qrc:/commonicons/home.png"
                icon.color: "blue"
                onClicked: {
                    tem.runSingleCmd(dispatcher, "home", [false, true])
                }
            }
            Rectangle {
                color: "pink"
                width: 2
                height: 45
            }
            MyToolButton {
                text: qsTr("Init")
                icon.source: "qrc:/commonicons/initial.png"
                enabled: stateMachine.initEnabled
                icon.color: enabled ? "limegreen" : "gray"
                onClicked: {
                    stateMachine.postEvent(SMD.Init)
                }
                ToolTip {
                    text: qsTr("ReadyForAutoRun")
                    visible: parent.hovered
                }
            }
            MyToolButton {
                text: qsTr("Start")
                icon.source: "qrc:/commonicons/start.png"
                enabled: stateMachine.startEnabled
                         || stateMachine.continueEnabled
                icon.color: enabled ? "limegreen" : "gray"
                onClicked: {
                    masterMotionManager.setMoveProtectionsEnable(true)
                    if (stateMachine.startEnabled) {
                        popupRunParameter.open()
                    }
                    if (stateMachine.continueEnabled) {
                        stateMachine.postEvent(SMD.Continue)
                    }
                }
            }
            MyToolButton {
                text: qsTr("Pause")
                icon.source: "qrc:/commonicons/pause.png"
                enabled: stateMachine.pauseEnabled
                icon.color: enabled ? "limegreen" : "gray"
                onClicked: {
                    stateMachine.postEvent(SMD.Pause)
                }
            }
            MyToolButton {
                text: qsTr("Stop")
                icon.source: "qrc:/commonicons/stop.png"
                enabled: stateMachine.stopEnabled
                icon.color: enabled ? "limegreen" : "gray"
                onClicked: {
                    stateMachine.postEvent(SMD.Stop)
                }
            }
            MyToolButton {
                text: qsTr("Abort")
                icon.source: "qrc:/commonicons/abort.png"
                icon.color: "red"
                onClicked: {
                    stateMachine.forceAbort()
                }
            }
            MyToolButton {
                text: qsTr("Reset")
                icon.source: "qrc:/commonicons/reset.png"
                enabled: stateMachine.resetEnabled
                icon.color: enabled ? "limegreen" : "gray"
                onClicked: {
                    stateMachine.postEvent(SMD.Reset)
                }
            }
            Rectangle {
                width: 600
                height: 60
                Text {
                    font.pixelSize: 35
                    anchors.fill: parent
                    text: stateMachine.stateStringDescription
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                color: stateMachine.stateColorDescription
            }
            Rectangle {
                width: 30
                height: 30
                color: "transparent"
            }
            Text {
                font.pixelSize: 22
                text: qsTr("DUT: ") + basicConfig.dutType
            }
        }
    }

    GridView {
        id: cameraViewers
        width: parent.width - 10
        x: 10
        y: 10
        height: parent.height - logViewer.height - 30
        model: cameraNames
        cellWidth: width / 4 - 5
        cellHeight: height / 2 - 5
        delegate: CameraViewer {
            id: nonFullScreenView

            width: cameraViewers.cellWidth
            height: cameraViewers.cellHeight
            isFullScreenView: false
            updateImage: true
            cameraName: modelData
            imageSource: "image://" + modelData
            camera: visionManager.getCamera(modelData)
            onReqShowRelatedDebugger: {
                var xyzrDbg = xyzrDebuggerContainer.getDebugger(dbg)
                if (xyzrDbg !== null) {
                    xyzrDbg.x = pos.x - xyzrDbg.implicitWidth
                    xyzrDbg.y = pos.y - header.implicitHeight - menuBar.implicitHeight
                    xyzrDbg.open()
                }
            }
            Component.onCompleted: {
                init()
            }
            Popup {
                id: fullScreenCameraWindow

                width: mainWindow.width
                height: mainWindow.height
                contentWidth: width - 20
                contentHeight: height - 20
                anchors.centerIn: Overlay.overlay

                onClosed: {
                    fullScreenCameraView.updateImage = false
                    nonFullScreenView.updateImage = true
                }

                contentItem: CameraViewer {
                    id: fullScreenCameraView

                    isFullScreenView: true
                    updateImage: false
                    cameraName: modelData
                    imageSource: "image://" + modelData
                    camera: visionManager.getCamera(modelData)

                    Connections {
                        target: nonFullScreenView
                        onReqShowFullScreen: {
                            nonFullScreenView.updateImage = false
                            fullScreenCameraView.updateImage = true
                            fullScreenCameraWindow.open()
                        }
                    }
                    onReqExitFullScreen: {
                        fullScreenCameraWindow.close()
                    }
                    Component.onCompleted: {
                        init()
                    }
                    onReqShowRelatedDebugger: {
                        var xyzrDbg = xyzrDebuggerContainer.getDebugger(dbg)
                        if (xyzrDbg !== null) {
                            xyzrDbg.x = pos.x
                            xyzrDbg.y = pos.y - header.implicitHeight - menuBar.implicitHeight
                            xyzrDbg.open()
                        }
                    }
                }
            }
        }
    }
    LogViewContainer {
        id: logViewer
        width: 800
        height: 300
        z: 100
        x: parent.width - width - 10
        y: parent.height - height - 10
        Component.onCompleted: {
            logToViewerHandler = logHandler
            init()
        }
    }

    onClosing: {
        if (stateMachine.currentState === SMD.Running || stateMachine.currentState
                === SMD.Purging || stateMachine.currentState
                === SMD.Stopping || stateMachine.currentState
                === SMD.Pausing || stateMachine.currentState
                === SMD.Paused || stateMachine.currentState
                === SMD.Error || stateMachine.currentState === SMD.Aborting) {
            close.accepted = false
            uiOperation.showError(
                        qsTr("Software can not be closed while current state is ")
                        + stateMachine.stateStringDescription)
        }
    }

    Component.onCompleted: {
        showMaximized()
    }
}
