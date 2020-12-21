import QtQuick 2.2
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0

Rectangle {
    id:root
    width: defaultStyle.menuWidth
    height: defaultStyle.menuHeight // 64
    color: "transparent"
    property var iconSource : "qrc:/Image/Common/NormalShadow.png"
    property int margin : 32
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
            source: "qrc:/Image/Common/Menu_Button_Hilite.png"
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
    }
    Rectangle
    {
        id:iconRect
        anchors.left: leftBorder.right
        width: defaultStyle.menuHeight
        height: defaultStyle.menuHeight
        color: "transparent"
        Image{
            id:icon
            anchors.centerIn: parent
            width: defaultStyle.iconSize
            height: defaultStyle.iconSize
            source: iconSource
        }
    }
    Rectangle
    {
        id:label
        anchors.left: iconRect.right
        width: defaultStyle.menuWidth - defaultStyle.menuHeight - 2 * margin
        height:defaultStyle.menuHeight
        color: "transparent"
        Text {
            id:btnText
            color: defaultStyle.fontColorGold
            text: root.text
            font.family: "华文中宋"
            font.pointSize: fontSize
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            anchors.fill: parent
        }
    }
    Rectangle
    {
        id:rightBorder
        anchors.left: label.right
        x:margin
        y:defaultStyle.height
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
