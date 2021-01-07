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
        //onEntered: {
        //    rectButtonsScenario.visible = true
        //}
        onDoubleClicked: {
            //console.log("$app.scenarioManager().setCurrentScenario(name) exec, name:", name)
            $app.scenarioManager().setCurrentScenario(name)
            //console.log("$app.scenarioManager().load() exec")
            $app.scenarioManager().load()
            //console.log("$app.scenarioManager().load() end")
            //welcome.visible = false
            welcome.visible = false
            $app.areaPlanManager().startEdit()
        }
    }
    Rectangle{
        id:rectButtonsScenario
        x : parent.width - width
        width: buttonWidth
        //visible:false
        Column{
            Button{
                id:btnRemoveFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("removeFavorite")
                visible: favorite
                onClicked: {
                    //console.log("scenarioManager.removeFavorite : " + name)
                    $app.scenarioManager().removeFavorite(name)
                }
            }
            Button{
                id:btnaddFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("addFavorite")
                visible: !favorite
                onClicked: {
                    $app.scenarioManager().addFavorite(name)
                    //console.log("scenarioManager.addFavorite : " + name)
                }
            }
            Button{
                id:btnRemoveScn
                width: buttonWidth
                height:buttonHeight
                text: qsTr("removeScenario")
                visible: true
                onClicked: {
                    // 先提示是否删除
                    //console.log("scenarioManager.removeScenario : " + name)
                    $app.scenarioManager().removeScenario(name)
                }
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

