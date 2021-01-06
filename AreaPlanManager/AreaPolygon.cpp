#include "AreaPolygon.h"
#include "AreaPolygonEditor.h"
#include <Plot/Map/IMapPolygon.h>

AreaPolygon::AreaPolygon(QObject *parent) : QObject(parent)
{
    _iMapPolygon = nullptr;
}
/*
AreaPolygon::AreaPolygon(const AreaPolygon& other)
{
    setPoints(other.points());
}

void AreaPolygon::operator=(const AreaPolygon& other)
{
    setPoints(other.points());
}*/

//bool AreaPolygon::isNull()
//{
//    return (_vertices.count() > 2);
//}

const QList<QVector3D> AreaPolygon::vertices() const
{
    return _vertices;
}

void AreaPolygon::setVertices(const QList<QVector3D> list)
{
    _vertices = list;
}

void AreaPolygon::setIMapPolygon(IMapPolygon* p)
{
    _iMapPolygon = p;
}

IMapPolygon* AreaPolygon::getIMapPolygon()
{
    return _iMapPolygon;
}
/*
void AreaPolygon::create()
{
    AreaPolygonEditor::getInstance()->createPolygon(this);
}

void AreaPolygon::release()
{
    AreaPolygonEditor::getInstance()->deletePolygon(this);
}
*/
