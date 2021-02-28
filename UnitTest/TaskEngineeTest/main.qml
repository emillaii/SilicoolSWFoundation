import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import TaskEngineeManager 1.0
import "JsCommands/LensLoaderCmds.js" as LLCmds
import "JsCommands/LutCmds.js" as LutCmds

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ColumnLayout{
        Rectangle{
            radius: 25
            width: radius * 2
            height: width
            color: {
                switch(tem.engineesState)
                {
                case TaskEngineeManager.AllIdle:
                    return "gray"
                case TaskEngineeManager.SomeIdleSomeBuzy:
                    return "green"
                case TaskEngineeManager.AllBuzy:
                    return "yellow"
                }
            }
        }

        RowLayout
        {
            ColumnLayout{
                Button{
                    text: "moveToLensPos"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.moveToLensPos)
                    }
                }
                Button{
                    text: "moveToLensPos"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.moveToLensPos, [1])
                    }
                }
                Button{
                    text: "moveToLensPos"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.moveToLensPos, [1, 2])
                    }
                }
                Button{     //report error
                    text: "moveToLensPos1"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.moveToLensPos, [1, 2, 3])
                    }
                }
                Button{     //report error
                    text: "moveToLensPos1"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, "moveTo  LensPos", [1, 2])
                    }
                }
                Button{     //type conversion
                    text: "moveToLensPos2"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.moveToLensPos, [1.0, "2"])
                    }
                }
                Button{     //type conversion
                    text: "pickLens"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.pickLens, [1])
                    }
                }
                Button{     //type conversion
                    text: "pickLens"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.pickLens, [0])
                    }
                }
                Button{     //type conversion
                    text: "pickLens"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.pickLens, [true])
                    }
                }
                Button{     //type conversion
                    text: "pickLens"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.pickLens, [""])
                    }
                }
                Button{     //type conversion
                    text: "pickLens"
                    onClicked: {
                        tem.runSingleCmd(lensLoader, LLCmds.pickLens, ["123"])
                    }
                }
            }
            ColumnLayout{
                Button{
                    text: "pickAndPlaceLens"
                    onClicked: {
                        tem.runMultiCmdsNoArg(lensLoader, [LLCmds.moveToLensPos,
                                              LLCmds.pickLens,
                                              LLCmds.moveToPlaceLensPos,
                                              LLCmds.placeLens])
                    }
                }
                Button{
                    text: "pickAndPlaceLens"
                    onClicked: {
                        tem.runMultiCmdsHasArg(lensLoader, [
                                               [LLCmds.moveToLensPos, 1],
                                               [LLCmds.pickLens, false],
                                               [LLCmds.moveToPlaceLensPos],
                                               [LLCmds.placeLens]])
                    }
                }
                Button{
                    text: "pickAndPlaceLens"
                    onClicked: {
                        tem.runMultiCmdsHasArg(lensLoader, [
                                               [LLCmds.moveToLensPos, 1, 3],
                                               [LLCmds.pickLens, 3.0],
                                               [LLCmds.moveToPlaceLensPos],
                                               [LLCmds.placeLens]])
                    }
                }
                Button{
                    text: "pickAndPlaceLens"
                    onClicked: {
                        tem.runMultiCmdsHasArg(lensLoader, [
                                               [LLCmds.moveToLensPos, 1, 3],
                                               [LLCmds.pickLens, 1, 2],
                                               [LLCmds.moveToPlaceLensPos],
                                               [LLCmds.placeLens]])
                    }
                }
                Button{
                    text: "pickLensToAA"
                    onClicked: {
                        tem.runOnMultiObjects([[lut, LutCmds.moveToLoadLensPos],
                                              [lensLoader, LLCmds.moveToLensPos, 3],
                                              [lensLoader, LLCmds.pickLens],
                                              [lensLoader, LLCmds.moveToPlaceLensPos],
                                              [lensLoader, LLCmds.placeLens],
                                              [lut, LutCmds.moveToAAPos],
                                              [lut, LutCmds.openVacuum]])
                    }
                }
                Button{     //report error
                    text: "pickLensToAA"
                    onClicked: {
                        tem.runOnMultiObjects([[lut, LutCmds.moveToLoadLensPos],
                                              [lensLoader, LLCmds.moveToLensPos, 3],
                                              [lensLoader, LLCmds.pickLens],
                                              [lensLoader, LLCmds.moveToPlaceLensPos],
                                              [lensLoader, LLCmds.placeLens],
                                              [lut, LLCmds.pickLens]])
                    }
                }
                Button{     //report error
                    text: "pickLensToAA"
                    onClicked: {
                        tem.runOnMultiObjects([[lut, LutCmds.moveToLoadLensPos],
                                              [lensLoader, LLCmds.moveToLensPos, 3],
                                              [lensLoader, LLCmds.pickLens],
                                              [lensLoader, LLCmds.moveToPlaceLensPos],
                                              ["lensLoader", ""],
                                              [lut, LutCmds.moveToAAPos]])
                    }
                }
                Button{     //report error
                    text: "pickLensToAA"
                    onClicked: {
                        tem.runOnMultiObjects([[lut, LutCmds.moveToLoadLensPos],
                                              [lensLoader, LLCmds.moveToLensPos, 3],
                                              [lensLoader, LLCmds.pickLens],
                                              [lensLoader, LLCmds.moveToPlaceLensPos],
                                              ["lensLoader"],
                                              [lut, LutCmds.moveToAAPos]])
                    }
                }
            }
        }
    }
}
