import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle{
    property bool favorite : false
    property int itemWidth: defaultStyle.scenarioItemWidth
    property int itemHeight: defaultStyle.scenarioItemHeight
    property int buttonWidth : defaultStyle.scenarioButtonWidth
    property int buttonHeight : defaultStyle.scenarioButtonHeight
    property int margin: defaultStyle.margin
    property var name: ""
    property var imageFilePath: ""

    x : margin / 2
    y : margin / 2
    width:itemWidth - margin
    height:itemHeight - margin
    //color:"transparent"
    property int textHeight: defaultStyle.scenarioTextHeight
    Image{
        width:parent.width
        height:parent.height// - textHeight
        source:"file:///" + imageFilePath
        visible:false
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            rectButtonsScenario.visible = true
        }
        onExited: {
        }
        onDoubleClicked: {
            scenarioManager.setCurrentScenario(name)
            scenarioManager.load()
            //welcome.visible = false
        }
    }
    Rectangle{
        id:rectButtonsScenario
        x : parent.width - width
        width: buttonWidth
        visible:false
        Column{
            Button{
                id:btnRemoveFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("removeFavorite")
                visible: favorite
                onClicked: {
                    // 先提示是否删除
                    scenarioManager.removeFavorite(name)
                    console.log("scenarioManager.removeFavorite : " + name)
                }
            }
            Button{
                id:btnaddFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("addFavorite")
                visible: !favorite
                onClicked: {
                    scenarioManager.addFavorite(name)
                    console.log("scenarioManager.addFavorite : " + name)
                }
            }
            Button{
                id:btnRemoveScn
                width: buttonWidth
                height:buttonHeight
                text: qsTr("removeScenario")
                visible: true
                onClicked: {
                    scenarioManager.removeScenario(name)
                    console.log("scenarioManager.removeScenario : " + name)
                }
            }
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true
            onExited: {
                rectButtonsScenario.visible = false
            }
        }
    }
    Text {
        id:text
        y:parent.height - text.height
        width:parent.width
        height:textHeight
        //color: defaultStyle.fontColor
        text: name
        font.family: defaultStyle.fontFamilyCN
        font.pointSize: defaultStyle.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        //anchors.fill: parent
    }
}
