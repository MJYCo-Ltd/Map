import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle
{
    id:rectNew
    width: defaultStyle.scenarioNewWidth
    height: defaultStyle.scenarioNewHeight
    signal scenarioAdded()
    Text{
        id:scenarioNewNameLabel
        x:margin
        y:margin
        width: 48
        height: 32
        color: defaultStyle.fontColor
        text:" name "
    }
    TextInput{
        id:scenarioNewNameInput
        x:margin * 2 + scenarioNewNameLabel.width
        y:margin
        width:256
        height:32
        color: "#FFFFFF"
        onTextChanged: {
            scenarioNewWarning.visible = false;
        }
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
        width: defaultStyle.scenarioButtonWidth + margin * 2
        Button {
            x:margin
            width: defaultStyle.scenarioButtonWidth
            height: defaultStyle.scenarioButtonHeight
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
            width: defaultStyle.scenarioButtonWidth
            height: defaultStyle.scenarioButtonHeight
            text: "Cancel"
            onClicked: {
                rectNew.visible = false
            }
        }
    }
}

