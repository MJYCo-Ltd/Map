import QtQuick 2.15
import QtQuick.Controls 2.15
import MyItem 1.0
import "../Common"

Rectangle
{
    id:editAnimation
    property int itemWidth: defaultStyle.scenarioItemWidth
    property int itemHeight: defaultStyle.scenarioItemHeight
    property int buttonHeight: defaultStyle.buttonHeight
    property int margin: defaultStyle.margin
    property var allID:[editText,editCamera,editPicture,editModel]
    color: "transparent"

    ButtonGroup {
        buttons: row.children
        onClicked: {
            switch(button.text)
            {
            case qsTr("Text"):
                hideAllShowOne(editText)
                break;
            case qsTr("Camera"):
                hideAllShowOne(editCamera)
                break;
            case qsTr("Picture"):
                hideAllShowOne(editPicture)
                break;
            case qsTr("Model"):
                hideAllShowOne(editModel)
                break;
            }
        }
    }
    Row{
        id:row
        Button{
            id: btnText
            height:buttonHeight
            checkable:true
            checked:true // default
            text:qsTr("Text")
        }
        Rectangle{
            color: "#FFFFFF"
            width:1
            height:buttonHeight
        }
        Button{
            id: btnCamera
            height:buttonHeight
            checkable:true
            checked:false
            text:qsTr("Camera")
        }
        Rectangle{
            color: "#FFFFFF"
            width:1
            height:buttonHeight
        }
        Button{
            id: btnPicture
            height:buttonHeight
            checkable:true
            checked:false
            text:qsTr("Picture")
        }
        Rectangle{
            color: "#FFFFFF"
            height:buttonHeight
            width:1
        }
        Button{
            id: btnModel
            height:buttonHeight
            checkable:true
            checked:false
            text:qsTr("Model")
        }
    }
    Rectangle
    {
        id:rectEdit
        y:row.height
        width: editAnimation.width
        height: editAnimation.height - y
        color: "transparent"
        border.color: "#FFFFFF"
        border.width: 1

        // -- 测试，代替换 --
        Rectangle{
            id:editText
            anchors.fill: rectEdit
            visible:true
        }
        Rectangle{
            id:editCamera
            anchors.fill: rectEdit
            visible:false
        }
        Rectangle{
            id:editPicture
            anchors.fill: rectEdit
            visible:false
        }
        // -- 导入模型和进度数据 --
        BuildProcess{
            id:editModel
            anchors.fill: rectEdit
            visible:false
        }
    }
    function hideAllShowOne(one)
    {
        for(var i = 0; i < allID.length; i++)
        {
            if(allID[i] !== one)
            {
                allID[i].visible = false;
            }
        }
        one.visible = true;
    }
}

