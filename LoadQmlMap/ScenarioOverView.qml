import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle
{
    property int itemWidth: defaultStyle.scnItemWidth
    property int itemHeight: defaultStyle.scnItemHeight
    property int margin: defaultStyle.margin
    color: "transparent"
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
                text:" New Scenario"
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
                text:" My Favorite"
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
                text:"More >> "
            }
            GridView {
                id:gridViewFavorite
                anchors.top: textFavorite.bottom
                width: parent.width
                height:cellHeight + margin * 2
                cellWidth: itemWidth
                cellHeight: itemHeight
                model: scenarioManager.favorites
                delegate: ScenarioOverviewItem{
                    favorite:true
                    name: modelData.name
                    imageFilePath: modelData.imageFilePath
                }
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
                text:" My Scenarios"
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
                text:"More >> "
            }
            GridView {
                id:gridViewRecent
                anchors.top: textRecent.bottom
                width: parent.width
                height:cellHeight + margin * 2
                cellWidth: itemWidth
                cellHeight: itemHeight
                model: scenarioManager.scenarios
                delegate: ScenarioOverviewItem{
                    favorite:false
                    name: modelData.name
                    imageFilePath: modelData.imageFilePath
                }
            }
        }
    }
}

