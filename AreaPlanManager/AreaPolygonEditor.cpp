#include "AreaPolygonEditor.h"
#include <SceneGraph/ISceneGraph.h>
#include <Plot/Map/IMap.h>
#include <QDebug>

AreaPolygonEditor* AreaPolygonEditor::_self = nullptr;

AreaPolygonEditor::AreaPolygonEditor() : QObject()
{

}

// 设置场景
void AreaPolygonEditor::setSceneGraph(ISceneGraph* sg)
{
    _sceneGraph = sg;
}

//ISceneGraph* AreaPolygonEditor::SceneGraph()
//{
//    return _sceneGraph;
//}

void AreaPolygonEditor::start()
{
    setEnable(true);
}

void AreaPolygonEditor::stop()
{
    setEnable(false);
    if (_points.count() > 2)
    {
        // 编辑完成，发出多边形数据
        AreaPolygon* ap = new AreaPolygon;
        QList<QVector3D> vList;
        for(int i = 0; i < _points.count(); i++)
        {
            MapGeoPos pos = _points[i]->GeoPos();
            vList.append(QVector3D(pos.fLon, pos.fLat, pos.fHeight));
        }
        ap->setVertices(vList);
        emit addArea(ap);
    }
    // 清空编辑缓存
    clear();
}

void AreaPolygonEditor::clear()
{
    // 删除点
    // YTY ...
    _points.clear();
    // 删除线
    // YTY ...
    _lines.clear();
}

void AreaPolygonEditor::setEnable(bool e)
{
    _enable = e;
}

bool AreaPolygonEditor::isEnable()
{
    return _enable;
}

void AreaPolygonEditor::MouseDown(MouseButtonMask, int, int)
{
    if (!_enable)
        return;
    /*
    /// 绘制点
    auto m_pPoint = dynamic_cast<IMapPoint*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    pos.fLon = 121;
    pos.fLat = 25;
    pos.fHeight = 0;
    m_pPoint->SetGeoPos(pos);
    //SceneColor color;
    //color.fG = .0f;
    //color.fB = .0f;
    //m_pPoint->SetPointColor(color);
    pLayer->AddSceneNode(m_pPoint);
    */
}

void AreaPolygonEditor::MouseMove(MouseButtonMask, int, int)
{
    if (!_enable)
        return;
    if (_lines.count() == 0)
        return;
    // 更新最后一条线
    IMapLine* _line = _lines[_lines.count() - 1];
    if (_line == nullptr)
        return;

    // 更新
    /*
    auto m_pLine = dynamic_cast<IMapLine*>(m_pSceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
    m_pLine->AddPoint(0,pos);
    pos.fLon = 122;
    pos.fLat = 26;
    m_pLine->AddPoint(0,pos);
    pos.fLon = 123;
    pos.fLat = 27;
    m_pLine->AddPoint(0,pos);
    m_pLine->SetLineColor(color);
    pLayer->AddSceneNode(m_pLine);*/
}

void AreaPolygonEditor::createPolygon(AreaPolygon* ap, AreaPlanLayer* layer)
{
    //qDebug() << "create polygon:";
    QList<QVector3D> vertices = ap->vertices();
    auto iMapPolygon = dynamic_cast<IMapPolygon*>(_sceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));    
    SceneColor color;
    color.fR = layer->color().red() / 255.0;
    color.fG = layer->color().green() / 255.0;
    color.fB = layer->color().blue() / 255.0;
    color.fA = layer->color().alpha() / 255.0;
    iMapPolygon->SetPolygonColor(color);

    IMap* map = _sceneGraph->GetMap();
    IMapLayer* pLayer = map->CreateLayer(layer->name().toStdString());
    pLayer->AddSceneNode(iMapPolygon);
    for (int i = 0; i < vertices.count(); i++)
    {
        //qDebug() << vertices[i].x() << "," << vertices[i].y() << "," << vertices[i].z();

        MapGeoPos pos;
        pos.fLon = vertices[i].x();
        pos.fLat = vertices[i].y();
        pos.fHeight = vertices[i].z();
        iMapPolygon->AddPoint(0, pos);
    }
}

void AreaPolygonEditor::deletePolygon(AreaPolygon*, AreaPlanLayer*)
{
    // ...
}
