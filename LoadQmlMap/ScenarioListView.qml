import QtQuick 2.15

Rectangle
{
    property int itemWidth: width - 2 * margin
    property int itemHeight: defaultStyle.menuHeight
    property int margin: defaultStyle.margin
    width : 500
    //height: 410
    color: "transparent"
    //color: defaultStyle.backgroundColor
    //BorderImg3{
    //    id: borderScenarioListView
    //    anchors.fill: parent
    //    //margin: parent.margin
    //}
    Button{
        id:btnAdd
        x : margin
        y : margin
        text:"+"
        width: itemWidth
        height: itemHeight
        border.width: 0
        onClicked: {
            Qt.quit()
        }
    }
    ListView {
        id:listView
        x: margin
        y: margin + btnAdd.height
        height: parent.height - btnAdd.height - margin
        model: scenarioManager.scenarios
        delegate: Rectangle {
            width: itemWidth
            height: itemHeight
            color: "transparent"
            Button{
                id:scenarioButton
                x:0
                width:itemWidth// - itemHeight
                height:itemHeight
                text: modelData
                onClicked: {
                }
            }
            //ButtonX{
            //    anchors.left: scenarioButton.right
            //    width:itemHeight
            //    height:itemHeight
            //    text: modelData
            //}
        }
        //ScrollBar.vertical: ScrollBar {       //滚动条
        //    anchors.right: listView.right
        //    active: true
        //    background:Rectangle {
        //            anchors.centerIn: parent
        //            height: parent.height
        //            width: 10
        //            color: 'grey'
        //        }
        //    contentItem: Rectangle {
        //        radius: width/3        //bar的圆角
        //        color: 'yellow'
        //    }
        //}
    }
}

