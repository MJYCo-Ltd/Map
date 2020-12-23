import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id:rectNew
    property int buttonWidth : 160
    property int buttonHeight : 42
    width: defaultStyle.scenarioNewWidth
    height: defaultStyle.scenarioNewHeight
    signal scenarioAdded()

    border.color: "#FFA0B0"
    border.width: 10

    Text{
        id:scenarioNewNameLabel
        x:margin
        y:margin
        width: 48
        height: 32
        color: defaultStyle.fontColor
        font.family: defaultStyle.fontFamilyCN
        font.pointSize: defaultStyle.fontSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        text:" name "
    }
    TextInput{
        id:scenarioNewNameInput
        x:margin * 2 + scenarioNewNameLabel.width
        y:margin
        width:256
        height:32
        color: "#FFFFFF"
        font.family: defaultStyle.fontFamilyCN
        font.pointSize: defaultStyle.fontSize
        onTextChanged: {
            scenarioNewWarning.visible = false;
        }
    }
    Rectangle{ // 下划线
        x:scenarioNewNameInput.x
        y:scenarioNewNameInput.y+scenarioNewNameInput.height
        width:256
        height:1
        color: "#FFFFFF"
    }

    Text{
        id:scenarioNewWarning
        x:margin
        y:margin + scenarioNewNameLabel.height
        width: 48
        height: 32
        color: defaultStyle.bgColorMsgAlert
        text:" This name is already exist! "
        visible:false
    }
    Column {
        id:scenarioNewButtons
        anchors.left: scenarioNewNameInput.right
        anchors.top: scenarioNewNameInput.top
        spacing:margin
        width: defaultStyle.buttonWidth + margin * 2
        Button {
            x:margin
            width: buttonWidth
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "OK"
            onClicked: {
                //rectNew.scenarioNew(scenarioNewNameInput.text)
                if(scenarioManager.addScenario(scenarioNewNameInput.text))
                    scenarioNewWarning.visible = false
                else
                    scenarioNewWarning.visible = true
            }
        }
        Button {
            x:margin
            width: buttonWidth
            height: buttonHeight
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSize
            text: "Cancel"
            onClicked: {
                rectNew.visible = false
            }
        }
    }
}

