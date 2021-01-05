#include "AreaPolygon.h"
#include <QJsonObject>
#include <QJsonValue>

AreaPolygon::AreaPolygon(QObject *parent) : QObject(parent)
{

}

QJsonArray AreaPolygon::toJson()
{
    QJsonArray arr;
    // ...
    for (int i = 0; i < _vertices.count(); i++)
    {
        QJsonObject obj;
        obj.insert("lon", _vertices[i].x());
        QJsonObject y;
        obj.insert("lat", _vertices[i].y());
        QJsonObject z;
        obj.insert("alt", _vertices[i].z());
        arr.append(obj);
    }
    return arr;
}

void AreaPolygon::fromJson(QJsonArray arrayPoint)
{
    for (int j = 0; j < arrayPoint.size(); j++)
    {
        QJsonValue value = arrayPoint.at(j);
        if (value.isObject())
        {
            double x = value.toObject().value("lon").toDouble();
            double y = value.toObject().value("lat").toDouble();
            double z = value.toObject().value("alt").toDouble();
            // YTY 此处添加多边形顶点
            //addPoint(osg::Vec3d(x,y,z));
            _vertices.append(QVector3D(x,y,z));
        }
    }
}
