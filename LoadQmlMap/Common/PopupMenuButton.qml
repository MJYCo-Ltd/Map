import QtQuick 2.2
import QtQuick.Controls 1.2
import QtGraphicalEffects 1.0

Rectangle {
    id:popupMenuButton
    width: defaultStyle.menuWidth
    height: defaultStyle.menuHeight // 64
    color: "transparent"    
    property int margin : defaultStyle.margin
    signal clicked(var string);
    property string text:"button"
    property int fontSize: 12
    Rectangle
    {
        color: "transparent"
        anchors.fill: parent
        //Image{
        //    id:bgImg
        //    anchors.fill: parent
        //    source: "qrc:/Image/Common/TalkboxBG.png"
        //}
        LinearGradient{
            id:bgLG
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(popupMenuButton.width, 0)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.5; color: "black" }
                GradientStop { position: 1.0; color: "transparent" }
            }
            opacity: 0.7
        }
        //Blend{
        //    anchors.fill: parent
        //    source: bgImg
        //    foregroundSource: bgLG
        //    mode: "add"
        //}
    }
    Rectangle
    {
        id:hilitePopupMenuButton
        anchors.fill: parent
        color: "transparent"
        visible:false
        Image{
            anchors.top: parent.top
            width: parent.width
            height: 16
            rotation:180
            source: "qrc:/Image/Common/Menu_Button_Hilite.png"
        }
        Image{
            anchors.bottom: parent.bottom
            width: parent.width
            height: 16
            source: "qrc:/Image/Common/Menu_Button_Hilite.png"
        }
    }
    Text {
        x:defaultStyle.margin
        width: defaultStyle.menuWidth - margin
        height:defaultStyle.menuHeight
        color: defaultStyle.fontColorGold
        text: popupMenuButton.text
        font.family: defaultStyle.fontFamilyCN
        font.pointSize: fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            hilitePopupMenuButton.visible = true
        }
        onExited: {
            hilitePopupMenuButton.visible = false
        }
        onClicked: {
            popupMenuButton.clicked(popupMenuButton.text)
        }
    }
}
