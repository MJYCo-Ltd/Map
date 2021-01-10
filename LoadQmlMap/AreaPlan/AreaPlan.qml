import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id:areaPlan
    property int margin: defaultStyle.margin
    property int textHeight: defaultStyle.scenarioTextHeight
    property int planWidth: 200
    property int planHeight: 42
    property int layerWidth: legendWidth * 2 + margin
    property int layerHeight: legendHeight
    property int legendWidth: 128
    property int legendHeight: 32
    color: "transparent"
    ListView {
        id:listViewPlan
        x:margin
        y:margin
        width: planWidth + margin
        height:areaPlan.height - margin
        highlight: Rectangle { color: "lightsteelblue"; radius: 1 }
        highlightFollowsCurrentItem: true;
        focus: true
        model: $app.areaPlanManager().plans
        delegate: Rectangle{
            id:rectPlan
            width: planWidth
            height: planHeight
            color: "transparent"
            Text{
                anchors.fill: parent
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: modelData.name
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    $app.areaPlanManager().setCurrentPlan(modelData.name)
                    console.log(modelData.name)
                    rectPlan.ListView.view.currentIndex = index;
                }
            }
        }
        Connections {
            target: $app.areaPlanManager()
            function onPlanListChanged() {
                listViewPlan.model = $app.areaPlanManager().plans
            }
        }
    }
    Rectangle{
        id:areaPlanSplitLine
        anchors.left: listViewPlan.right
        width:1
        height:areaPlan.height
        color:"#FFFFFF"
    }
    ListView {
        id:listViewPlanLayer
        x:listViewPlan.width + areaPlanSplitLine.width + margin * 2
        y:margin
        width: layerWidth + margin * 2
        height:areaPlan.height - margin * 2
        highlight: Rectangle {
            color: "darkgray";
            border.color: "lightsteelblue"; border.width: 1
        }
        highlightFollowsCurrentItem: true;
        focus: true
        model: $app.areaPlanManager().layers
        delegate: Rectangle{
            id:rectLayer
            width: layerWidth
            height: layerHeight
            color: "transparent"
            Image{
                id:listViewPlanLayerImage
                width: legendWidth
                height: legendHeight
                source: "file:///" + modelData.legend
            }
            Rectangle{
                id:rectSpace
                anchors.left: listViewPlanLayerImage.right
                width: margin
                color: "transparent"
            }
            Text{
                id:listViewPlanLayerText
                anchors.left: rectSpace.right
                width: legendWidth
                height: legendHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                color:modelData.color
                text: modelData.name
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    rectLayer.ListView.view.currentIndex = index;
                    $app.areaPlanManager().setCurrentLayer(modelData.name)
                    $app.areaPlanManager().startEdit()
                    edit.visible = false
                }
            }
        }
        Connections {
            target: $app.areaPlanManager()
            function onCurrentPlanChanged() {
                listViewPlanLayer.model = $app.areaPlanManager().layers
            }
        }
        Connections {
            target: $app.areaPlanManager()
            function layerListChanged() {
                listViewPlan.model = $app.areaPlanManager().plans
            }
        }
    }

}
