import QtQuick 2.15
import QtQuick.Controls 2.15
import MyItem 1.0

Rectangle
{
    property int itemWidth: defaultStyle.scenarioItemWidth
    property int itemHeight: defaultStyle.scenarioItemHeight
    property int margin: defaultStyle.margin
    color: "transparent"

    BuildProcess{
        anchors.fill: parent
    }
}

