import QtQuick 2.0

BorderMsg{
    property string text: "Well Done!"

    borderColor : defaultStyle.borderColorMsgGood
    bgColor : defaultStyle.bgColorMsgGood
    bgColorLight : defaultStyle.bgColorMsgGoodLight
    Text{
        font.family: "Helvetica"
        font.pointSize: 13
        anchors.centerIn: parent
        color: defaultStyle.fontColorMsgGood
        text:parent.text
    }
}
