import QtQuick 2.15
import QtQuick.Layouts 1.15

Rectangle
{
    property int itemWidth: 132
    property int itemHeight: 132
    property int margin: defaultStyle.margin
    property int columnCount: (width - 2 * margin)/ itemWidth
    //width : itemWidth * columnCount + 2 * margin
    color: "transparent"
    Component
    {
        id:item
        Rectangle{
            x : margin / 2
            y : margin / 2
            width:itemWidth - margin
            height:itemHeight - margin
            radius:8
            color: Qt.rgba(30 * index / 255, (200 - 20 * index) / 255, (150 - 8 * index) / 255, 1)
            Text {
                color: defaultStyle.fontColor
                text: scenarioManager.scenarios[index]
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                anchors.fill: parent
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: {
                }
                onExited: {
                }
                onClicked: {
                }
            }
        }
    }
    Rectangle
    {
        x:margin
        y:margin
        width: parent.width - margin * 2
        height:rectFavorite.height + rectRecent.height + margin * 2
        color:"transparent"
        Rectangle
        {
            id:rectNew
            width: parent.width
            height:textNew.height + gridViewFavorite.height
            color:"transparent"
            Text
            {
                id:textNew
                anchors.left: rectNew.left
                color: defaultStyle.fontColor
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                height:defaultStyle.titleHeight
                text:"New Scenario"
            }
            Rectangle{
                x : margin / 2
                y : margin / 2
                anchors.top: textNew.bottom
                width:itemWidth - margin
                height:itemHeight - margin
                radius:8
                color: Qt.rgba(93 / 255, 220 / 255, 158 / 255, 1)
                Text {
                    color: defaultStyle.fontColor
                    text: "+ New Scenario"
                    font.family: defaultStyle.fontFamily
                    font.pointSize: defaultStyle.fontSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.fill: parent
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                    }
                    onExited: {
                    }
                    onClicked: {
                    }
                }
            }
        }
        Rectangle
        {
            id:rectFavorite
            anchors.top: rectNew.bottom
            width: parent.width
            height:textFavorite.height + gridViewFavorite.height
            color:"transparent"
            Text
            {
                id:textFavorite
                anchors.left: rectFavorite.left
                color: defaultStyle.fontColor
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                height:defaultStyle.titleHeight
                text:"My Favorite"
            }
            Text
            {
                anchors.right: rectFavorite.right
                color: defaultStyle.fontColorLight
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                height:defaultStyle.titleHeight
                text:"More>"
            }
            GridView {
                id:gridViewFavorite
                anchors.top: textFavorite.bottom
                width: parent.width
                height:cellHeight + margin * 2
                cellWidth: itemWidth
                cellHeight: itemHeight
                model: columnCount
                delegate: item
            }
        }
        Rectangle
        {
            id:rectRecent
            anchors.top: rectFavorite.bottom
            width: parent.width
            height:textRecent.height + gridViewRecent.height
            color:"transparent"
            Text
            {
                id:textRecent
                anchors.left: rectRecent.left
                color: defaultStyle.fontColor
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                height:defaultStyle.titleHeight
                text:"My Scenarios"
            }
            Text
            {
                anchors.right: rectRecent.right
                color: defaultStyle.fontColorLight
                font.family: defaultStyle.fontFamily
                font.pointSize: defaultStyle.fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                height:defaultStyle.titleHeight
                text:"More>"
            }
            GridView {
                id:gridViewRecent
                anchors.top: textRecent.bottom
                width: parent.width
                height:cellHeight + margin * 2
                cellWidth: itemWidth
                cellHeight: itemHeight
                model: columnCount
                delegate: item
            }
        }
    }
}

