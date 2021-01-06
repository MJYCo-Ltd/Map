import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import MyItem 1.0

Rectangle
{
    id:buildprocess
    property int buttonWidth : defaultStyle.buttonWidth
    property int buttonHeight : defaultStyle.buttonHeight
    property int labelWidth : 128
    property int textInputWidth : 512
    property int itemHeight : buttonHeight
    color: "transparent"

    //FileDialog{
    //    id:fileDlg
    //    nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
    //    Component.onCompleted: visible = false
    //    onAccepted: {
    //         console.log("You chose: " + fileDlg.fileUrls)
    //         visible = false
    //     }
    //    onRejected: {
    //        console.log("Canceled")
    //        visible = false
    //    }
    //}
    // -- 模型文件 --------------------------------
    Row{
        x:margin
        y:margin
        Text{
            id:buildModelLabel
            width: labelWidth
            height: itemHeight
            color: defaultStyle.fontColor
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            text:" model "
        }
        TextInput{
            id:buildModelInput
            width:textInputWidth
            height:itemHeight
            color: "#FFFFFF"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            onTextChanged: {
            }
            Rectangle{
                y:parent.height - 1
                width:parent.width
                height:1
                color: "#FFFFFF"
            }
        }
        Button {
            width: buttonHeight
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "..."
            onClicked: {
                //fileDlg.visible = true
            }
            Rectangle{
                y:parent.height - 1
                width:parent.width
                height:1
                color: "#FFFFFF"
            }
        }
    }
    // -- 进度文件 --------------------------------
    Row{
        x:margin
        y:margin + itemHeight
        Text{
            id:buildProcessLabel
            width: labelWidth
            height: itemHeight
            color: defaultStyle.fontColor
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            text:" process "
        }
        TextInput{
            id:buildProcessInput
            width:textInputWidth
            height:itemHeight
            color: "#FFFFFF"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            onTextChanged: {
            }
            Rectangle{
                y:parent.height - 1
                width:parent.width
                height:1
                color: "#FFFFFF"
            }
        }
        Button {
            width: buttonHeight
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "..."
            onClicked: {
                //fileDlg.visible = true
            }
            Rectangle{
                y:parent.height - 1
                width:parent.width
                height:1
                color: "#FFFFFF"
            }
        }
    }
    // -- 错误提示、取消/确认按钮 ----------------
    Row{
        x:margin
        y:margin * 2 + itemHeight * 2
        Text{
            id:buildWarning
            width: textInputWidth + labelWidth - buttonWidth * 2 + buttonHeight
            color: defaultStyle.fontColorMsgWarning
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignLeft
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text:" Process file (.json) is invalid! "
            //visible:false
        }
        Button {
            id:btnCancel
            width: buttonWidth
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Cancel"
            onClicked: {
            }
        }
        Rectangle{
            width:1
            height: buttonHeight
        }
        Button {
            id:btnOK
            width: buttonWidth
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "OK"
            onClicked: {
                //processBuild.init()
                simulation.addProcess(processBuild)
                simulation.start()
            }
        }
    }
}

