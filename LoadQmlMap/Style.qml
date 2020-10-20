import QtQuick 2.0

Item {
    id : defaultStyle
    property int margin: 10
    property color primaryColor : borderColorMsgInfo
    property color secondaryColor : bgColorMsgGood
    property color fontColor : fontColorMsgGood
    property color backgroundColor:Qt.rgba(14/255,53/255,70/255, 65/255)
    // 绿
    property color fontColorMsgDone:Qt.rgba(10/255,212/255,6/255, 1)
    property color borderColorMsgDone:Qt.rgba(10/255,212/255,6/255, 1)
    property color bgColorMsgDone:Qt.rgba(27/255,40/255,14/255, 1)
    property color bgColorMsgDoneLight:Qt.rgba(2/255,61/255,50/255, 1)
    // 蓝
    property color fontColorMsgInfo:Qt.rgba(3/255,150/195,233/255, 1)
    property color borderColorMsgInfo:Qt.rgba(3/255,150/195,233/255, 1)
    property color bgColorMsgInfo:Qt.rgba(3/255,21/195,58/255, 1)
    property color bgColorMsgInfoLight:Qt.rgba(3/255,34/195,81/255, 1)
    // 红
    property color fontColorMsgAlert:Qt.rgba(255/255,3/255,4/255, 1)
    property color borderColorMsgAlert:Qt.rgba(255/255,3/255,4/255, 1)
    property color bgColorMsgAlert:Qt.rgba(38/255,15/255,39/255, 1)
    property color bgColorMsgAlertLight:Qt.rgba(74/255,16/255,32/255, 1)
    // 黄
    property color fontColorMsgWarning:Qt.rgba(255/255,177/255,3/255, 1)
    property color borderColorMsgWarning:Qt.rgba(255/255,177/255,3/255, 1)
    property color bgColorMsgWarning:Qt.rgba(38/255,33/255,39/255, 1)
    property color bgColorMsgWarningLight:Qt.rgba(72/255,51/255,27/255, 1)
    // 青
    property color fontColorMsgGood:Qt.rgba(3/255,208/255,255/255, 1)
    property color borderColorMsgGood:Qt.rgba(3/255,208/255,255/255, 1)
    property color bgColorMsgGood:Qt.rgba(3/255,46/255,74/255, 1)
    property color bgColorMsgGoodLight:Qt.rgba(0/255,76/255,99/255, 1)

    property int titleWidth : 240
    property int titleHeight : 60
    property int lineWidth: 2
}