import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle{
    id:areaPlan
    property int margin: defaultStyle.margin
    property int planWidth: layerWidth
    property int planHeight: 32
    property int layerWidth: legendWidth + textWidth + margin
    property int layerHeight: legendHeight
    property int legendWidth: 64
    property int legendHeight: 32
    property int polygonWidth: 64
    property int polygonHeight: 24
    property int textWidth: 128
    property int textHeight: defaultStyle.scenarioTextHeight
    property bool editMode: checkBoxAreaPlanEdit.checked // default mode : show
    color: "transparent"

    Rectangle{
        x:margin
        y:margin
        width: planWidth
        height: planHeight
        Row{
            Button {
                id : buttonNew
                width: planWidth / 2
                height:planHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSizeSmall
                text: qsTr("New")
                onClicked: {
                    //$app.areaPlanManager.newPlan()
                }
            }
            Text {
                id: textEdit
                width: planWidth / 2 - planHeight
                height:planHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: qsTr("Edit")
            }
            CheckBox{
                id:checkBoxAreaPlanEdit
                width:planHeight
                height:planHeight
                checked:false
            }
        }
    }
    ComboBox {
        id:comboxAreaPlan
        x:margin
        y:margin+planHeight
        currentIndex: -1
        model: $app.areaPlanManager().planNames
        width: planWidth
        height:planHeight
        delegate:ItemDelegate {
            width: planWidth
            contentItem: Text {
                text: modelData
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        contentItem: Text {
            leftPadding: 5
            text: comboxAreaPlan.displayText
            font.family: defaultStyle.fontFamilyCN
            font.pointSize: defaultStyle.fontSizeSmall
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        onCurrentIndexChanged: {
            $app.areaPlanManager().setCurrentPlan(comboxAreaPlan.currentIndex)
        }
    }
    ListView {
        id:listViewPlanLayer
        x:margin
        y:margin*2 + planHeight*2
        currentIndex: -1
        width: layerWidth + margin * 2
        height:areaPlan.height - y
        highlight: Rectangle {
            color: "lightsteelblue";
            //border.color: "lightsteelblue"; border.width: 1
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
                id:textLayer
                anchors.left: rectSpace.right
                width: textWidth
                height: textHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSizeSmall
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
                    $app.areaPlanManager().setEditMode(areaPlan.editMode)
                    if (editMode)
                    {
                        $app.areaPlanManager().startEdit()
                    }

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
            function onStartEditMode() {
                areaPlan.visible = false
            }
        }
    }
    Rectangle{
        x: listViewPlanLayer.width + margin
        y: margin
        width:1
        height:areaPlan.height - y
        color:"white"
    }
    ListView {
        id:listViewAreaPolygon
        x: listViewPlanLayer.width + margin * 2 + 1
        y: listViewPlanLayer.y
        width: polygonWidth
        height:areaPlan.height - y - margin
        currentIndex: -1
        model:$app.areaPlanManager().areaCount()
        delegate: Rectangle{
            id:rectPolygon
            width: polygonWidth
            height: polygonHeight
            color: $app.areaPlanManager().currentLayerColor()
            Text{
            //    id:textPolygon
                anchors.centerIn: parent
                width: textWidth
                height: textHeight
                font.family: defaultStyle.fontFamilyCN
                font.pointSize: defaultStyle.fontSizeSmall
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#000000"
                text: index.toString()
            }
            Rectangle{
                anchors.bottom: parent.bottom
                color: "white"
                height: 1
            }
            //MouseArea {
            //    anchors.fill: parent
            //    onClicked: {
            //        rectPolygon.ListView.view.currentIndex = index;
            //    }
            //}
        }
        Connections {
            target: $app.areaPlanManager()
            function onAreaListChanged() {
                listViewAreaPolygon.model = $app.areaPlanManager().areaCount()
                console.log("listViewAreaPolygon onAreaListChanged")
            }
        }
    }

    Component.onCompleted:{
        $app.areaPlanManager().setCurrentPlan(comboxAreaPlan.currentIndex)
    }
}
