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
    Rectangle
    {
        id:hilite
        width: parent.width
        height: 16
        anchors.bottom: parent.bottom
        color: "transparent"
        visible:false
        Image{
            id:hiliteImg
            width: parent.width
            height: parent.height
            source: "Image/Menu_Button_Hilite.png"
        }
        ColorOverlay {
            anchors.fill: hiliteImg
            source: hiliteImg
            color: defaultStyle.primaryColor
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
            hilite.visible = true
        }
        onExited: {
            hilite.visible = false
        }
        onClicked: {
            root.clicked(btnText.text)
        }
    }
}
