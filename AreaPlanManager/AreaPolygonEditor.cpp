#include "AreaPolygonEditor.h"
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IViewPort.h>
#include <Plot/Map/IMap.h>
#include <QDebug>

AreaPolygonEditor* AreaPolygonEditor::_self = nullptr;

AreaPolygonEditor::AreaPolygonEditor() : QObject()
{
    _line               = nullptr;
    _polygon            = nullptr;
    _enable             = false;

    _pointColor.fR      = .1f;
    _pointColor.fG      = .8f;
    _pointColor.fB      = .1f;
    _lineColor.fR       = .9f;
    _lineColor.fG       = .0f;
    _lineColor.fB       = .2f;
    _polygonColor.fR    = .9f;
    _polygonColor.fG    = .85f;
    _polygonColor.fB    = .2f;
}

// 设置场景
void AreaPolygonEditor::setSceneGraph(ISceneGraph* sg)
{
    _sceneGraph = sg;
    _sceneGraph->GetMainWindow()->SubMessage(this);

    _layer = _sceneGraph->GetMap()->CreateLayer("temp");
}

//ISceneGraph* AreaPolygonEditor::SceneGraph()
//{
//    return _sceneGraph;
//}

void AreaPolygonEditor::setColor(const SceneColor& c)
{
    _polygonColor = c;
}

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
    if (_layer)
        _layer->Clear();
    _points.clear();
    _line = nullptr;
    _polygon = nullptr;
}

void AreaPolygonEditor::setEnable(bool e)
{
    _enable = e;
}

bool AreaPolygonEditor::isEnable()
{
    return _enable;
}

void AreaPolygonEditor::MouseDown(MouseButtonMask mask, int x, int y)
{
    if (!_enable)
        return;
    if (mask == LEFT_MOUSE_BUTTON)
    {
        // 在temp图层绘制点
        MapGeoPos pos;
        if(_sceneGraph->GetMap()->ConvertCoord(x, y, pos, 0))
        {
            addPoint(pos);
        }
    }
    else if (mask == RIGHT_MOUSE_BUTTON)
    {
        // -- 使用缓存点数据，创建多边形 --------------------------
        IMapPolygon* polygon = dynamic_cast<IMapPolygon*>(_sceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
        AreaPolygon* ap = new AreaPolygon;
        ap->setIMapPolygon(polygon);
        QList<QVector3D> list;
        for(int i = 0; i < _points.count(); i++)
        {
            MapGeoPos pos = _points[i]->GeoPos();
            list.append(QVector3D(pos.fLon, pos.fLat, pos.fHeight));
        }
        ap->setVertices(list);
        emit addArea(ap);
        // -- 清除缓存 ----------------------------------------
        clear();
        //----------------------------------------------------
        //setEnable(false);
    }
}

void AreaPolygonEditor::MouseMove(MouseButtonMask, int x, int y)
{
    if (!_enable)
        return;
    MapGeoPos pos;
    if(_sceneGraph->GetMap()->ConvertCoord(x, y, pos, 0))
    {
        updatePoint(pos);
    }
}

void AreaPolygonEditor::KeyDown(char key)
{
    qDebug() << "key:" << key;
    // 如果按下z // Ctrl+Z
    if (key == 'z')
    {
        if (_polygon && _polygon->GetPointCount() > 0)
            _polygon->RemovePoint(_polygon->GetPointCount() - 1);
        if (_line && _line->GetPointCount() > 0)
            _line->RemovePoint(_line->GetPointCount() - 1);
        if (_points.count() > 0)
        {
            _layer->RemoveSceneNode(_points.last());
            _points.removeLast();
        }
    }
}

void AreaPolygonEditor::addPoint(MapGeoPos pos)
{
    // 绘制点
    auto point = dynamic_cast<IMapPoint*>(_sceneGraph->GetPlot()->CreateSceneNode("IMapPoint"));
    point->SetGeoPos(pos);
    point->SetPointColor(_pointColor);
    point->SetPointSize(6.0);
    _layer->AddSceneNode(point);
    _points.append(point);
    // 绘制多边形
    if (_polygon == nullptr)
    {
        _polygon = dynamic_cast<IMapPolygon*>(_sceneGraph->GetPlot()->CreateSceneNode("IMapPolygon"));
        _polygon->SetPolygonColor(_polygonColor);
        _layer->AddSceneNode(_polygon);
    }
    if (_polygon->GetPointCount() == 0)
    {
        //qDebug() << "_line->AddPoint(0)";
        _polygon->AddPoint(0, pos);
//        _polygon->AddPoint(1, pos);    // 第二个点用于编辑
    }
    else if (_polygon->GetPointCount() > 0)    // 插入到倒数第二位置（末尾的点MouseMove编辑用）
    {
        //qDebug() << QString("_line->AddPoint(%1)").arg(_polygon->GetPointCount() - 1);
        _polygon->AddPoint(_polygon->GetPointCount(), pos);
    }
    // 绘制线
    if (_line == nullptr)
    {
        _line = dynamic_cast<IMapLine*>(_sceneGraph->GetPlot()->CreateSceneNode("IMapLine"));
        _line->SetLineColor(_lineColor);
        _line->SetLineWidth(4.0);
        _layer->AddSceneNode(_line);
    }
    //qDebug() << "_line->GetPointCount()：" << _line->GetPointCount();
    if (_line->GetPointCount() == 0)
    {
        //qDebug() << "_line->AddPoint(0)";
        _line->AddPoint(0, pos);
//        _line->AddPoint(1, pos);    // 第二个点用于编辑
    }
    else if (_line->GetPointCount() > 0)    // 插入到倒数第二位置（末尾的点MouseMove编辑用）
    {
        //qDebug() << QString("_line->AddPoint(%1)").arg(_line->GetPointCount() - 1);
        _line->AddPoint(_line->GetPointCount(), pos);
    }
}

void AreaPolygonEditor::updatePoint(MapGeoPos pos)
{
    if (_line->GetPointCount() == 0)
        return;
    else if (_line->GetPointCount() >= 2)
    {
        // 更新最后一点
        //qDebug() << QString("_line->UpdatePoint(%1)").arg(_line->GetPointCount() - 1);
        _line->UpdatePoint(_line->GetPointCount() - 1, pos);
    }

    if (_polygon->GetPointCount() == 0)
        return;
    else if (_polygon->GetPointCount() >= 3)
    {
        // 更新最后一点
        //qDebug() << QString("_polygon->UpdatePoint(%1)").arg(_polygon->GetPointCount() - 1);
        _polygon->UpdatePoint(_polygon->GetPointCount() - 1, pos);
    }
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
        MapGeoPos pos;
        pos.fLon = vertices[i].x();
        pos.fLat = vertices[i].y();
        pos.fHeight = vertices[i].z();
        iMapPolygon->AddPoint(0, pos);
    }
    ap->setIMapPolygon(iMapPolygon);
}

void AreaPolygonEditor::removePolygon(AreaPolygon* polygon, AreaPlanLayer* layer)
{
    _sceneGraph->GetMap()->CreateLayer(layer->name().toStdString())->RemoveSceneNode(polygon->getIMapPolygon());
}

void AreaPolygonEditor::locatePolygon(AreaPolygon* polygon)
{
    if (polygon == nullptr)
        return;
    if (polygon->vertices().count() < 1)
        return;
    SceneViewPoint svp;
    int vc = polygon->vertices().count();
    float m = 1.0 / vc;
    for (int i = 0; i < vc; i++)        // 取平均数
    {
        svp.stPos.fX += m * polygon->vertices()[i].x();
        svp.stPos.fY += m * polygon->vertices()[i].y();
        svp.stPos.fZ += m * polygon->vertices()[i].z();
    }
    svp.fElev = 45.0;
    //svp.fAzimuth = 89.0;
    svp.fDistance = 3000;

    IViewPort* vp = _sceneGraph->GetMainWindow()->GetMainViewPoint();
    vp->SetViewPoint(svp);
}

bool AreaPolygonEditor::isVisible(AreaPlanLayer* layer)
{
    IMap* map = _sceneGraph->GetMap();
    if (map == nullptr)
        return false;
    IMapLayer* pLayer = map->CreateLayer(layer->name().toStdString());
    if (pLayer == nullptr)
        return false;
    return pLayer->IsVisible();
}

void AreaPolygonEditor::setVisible(AreaPlanLayer* layer, bool visible)
{
    IMap* map = _sceneGraph->GetMap();
    IMapLayer* pLayer = map->CreateLayer(layer->name().toStdString());
    pLayer->SetVisible(visible);
}
/*
void AreaPolygonEditor::setVisible(int index, AreaPlanLayer* layer, bool)
{
    IMap* map = _sceneGraph->GetMap();
    IMapLayer* pLayer = map->CreateLayer(layer->name().toStdString());
    //pLayer->SetVisible(index, visible);
}
*/
