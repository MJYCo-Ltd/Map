#ifndef AREAPOLYGON_H
#define AREAPOLYGON_H
/*************************************************
* Copyright(C)
* File name:    AreaPolygon
* Author:       魏晓亮
* Version:      1.0
* Date:         2021/01/05
* Description:  区域规划图的多边形区域
*				编辑多边形顶点，在三维地球上绘制图形
*               支持从Json读取和保存到Json
* History:
*************************************************/
#include <QJsonArray>
#include <QVector3D>
#include <QObject>
#include <QList>

class AreaPolygon : public QObject
{
    Q_OBJECT
public:
    explicit AreaPolygon(QObject *parent = nullptr);

    QJsonArray toJson();
    void fromJson(QJsonArray);

signals:

public slots:
    //addPoint(osg::Vec3d(x,y,z));

private:
    // YTY 保存多边形节点以获取或添加顶点
    QList<QVector3D>       _vertices;
};

#endif // AREAPOLYGON_H
