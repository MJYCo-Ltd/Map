#include "AreaPolygonLoader.h"
#include "AreaPolygon.h"
#include <QJsonObject>
#include <QJsonValue>

AreaPolygonLoader* AreaPolygonLoader::_self = nullptr;

AreaPolygonLoader::AreaPolygonLoader() : QObject()
{

}

QJsonArray AreaPolygonLoader::toJson(AreaPolygon* ap)
{
    if (ap == nullptr)
        return QJsonArray();
    QList<QVector3D> vertices = ap->vertices();
    QJsonArray arr;
    // ...
    for (int i = 0; i < vertices.count(); i++)
    {
        QJsonObject obj;
        obj.insert("lon", vertices[i].x());
        QJsonObject y;
        obj.insert("lat", vertices[i].y());
        QJsonObject z;
        obj.insert("alt", vertices[i].z());
        arr.append(obj);
    }
    return arr;
}

AreaPolygon* AreaPolygonLoader::fromJson(QJsonArray arrayPoint)
{
    QList<QVector3D> vertices;
    for (int j = 0; j < arrayPoint.size(); j++)
    {
        QJsonValue value = arrayPoint.at(j);
        if (value.isObject())
        {
            double x = value.toObject().value("lon").toDouble();
            double y = value.toObject().value("lat").toDouble();
            double z = value.toObject().value("alt").toDouble();            
            vertices.append(QVector3D(x,y,z));
        }
    }
    AreaPolygon* ap = new AreaPolygon;
    ap->setVertices(vertices);
    return ap;
}
