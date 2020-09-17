import QtQuick 2.2
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0

Rectangle {
    id:root
    width: defaultStyle.menuWidth
    height: defaultStyle.menuHeight // 64
    color: "transparent"
    signal clicked(var string);
    property string text:"button"
    property int fontSize: 12
    property color colorOverlay : defaultStyle.borderColor
    Rectangle
    {
        id:hiliteTop
        width: parent.width
        height: 8
        anchors.top: parent.top
        color: "transparent"
        //visible:false
        Image{
            id:hiliteImgTop
            width: parent.width
            height: parent.height
            source: "Image/Menu_Button_Hilite.png"
        }
        ColorOverlay {
            anchors.fill: hiliteImgTop
            source: hiliteImgTop
            color: colorOverlay
        }
    }
    Rectangle
    {
        id:hiliteBottom
        width: parent.width
        height: 8
        anchors.bottom: parent.bottom
        color: "transparent"
        //visible:false
        Image{
            id:hiliteImgBottom
            width: parent.width
            height: parent.height
            source: "Image/Menu_Button_Hilite.png"
        }
        ColorOverlay {
            anchors.fill: hiliteImgBottom
            source: hiliteImgBottom
            color: colorOverlay
        }
    }
    Rectangle
    {
        id:leftBorder
        x:defaultStyle.margin
        y:defaultStyle.height
        color: "transparent"
    }
    Rectangle
    {
        id:label
        anchors.fill: parent
        width: defaultStyle.menuWidth - defaultStyle.menuHeight - 2 * defaultStyle.margin
        height:defaultStyle.menuHeight
        color: "transparent"
        Text {
            id:btnText
            color: defaultStyle.fontColor
            text: root.text
            font.family: defaultStyle.fontFamily
            font.pointSize: defaultStyle.fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
        }
    }
    Rectangle
    {
        id:rightBorder
        anchors.left: label.right
        x:defaultStyle.margin
        y:defaultStyle.height
        color: "transparent"
    }
    MouseArea {
        id: mouseAreaBtn
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            //hiliteTop.visible = true
            //hiliteBottom.visible = true
            colorOverlay = defaultStyle.borderColorPressed
        }
        onExited: {
            //hiliteTop.visible = false
            //hiliteBottom.visible = false
            colorOverlay = defaultStyle.borderColor
        }
        onClicked: {
            root.clicked(btnText.text)
        }
    }
}
