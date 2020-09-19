import QtQuick 2.0
import QtQuick.Controls 2.15

Rectangle{
    property bool favorite : false
    property int itemWidth: defaultStyle.scnItemWidth
    property int itemHeight: defaultStyle.scnItemHeight
    property int buttonWidth : 80
    property int buttonHeight : 24
    property int margin: defaultStyle.margin
    property var name: ""
    property var imageFilePath: ""

    x : margin / 2
    y : margin / 2
    width:itemWidth - margin
    height:itemHeight - margin
    //color:"transparent"
    property int textHeight: 24
    Image{
        width:parent.width
        height:parent.height - textHeight
        source:"file:///" + imageFilePath
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            if(favorite)
            {
                btnRemoveFav.visible = true
            }
            else{
                btnaddFav.visible = true
                btnRemoveScn.visible = true
            }
        }
        onExited: {
            if(favorite)
            {
                btnRemoveFav.visible = false
            }
            else{
                btnaddFav.visible = false
                btnRemoveScn.visible = false
            }
        }
        onClicked: {
            scenarioManager.setCurrentScenario(name)
            scenarioManager.load()
            //welcome.visible = false
        }
    }
    Rectangle{
        id:rectButtonsScenario
        x : parent.width - width
        width: buttonWidth
        Column{
            Button{
                id:btnRemoveFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("removeFavorite")
                visible: false
                onClicked: {
                    scenarioManager.removeFavorite(name)
                    console.log("scenarioManager.removeFavorite : " + name)
                }
            }
            Button{
                id:btnaddFav
                width: buttonWidth
                height:buttonHeight
                text: qsTr("addFavorite")
                visible: false
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
                visible: false
                onClicked: {
                    scenarioManager.removeScenario(name)
                    console.log("scenarioManager.removeScenario : " + name)
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
        font.family: defaultStyle.fontFamily
        font.pointSize: defaultStyle.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        //anchors.fill: parent
    }
}

