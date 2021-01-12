#ifndef AREAPOLYGON_H
#define AREAPOLYGON_H
/*************************************************
* Copyright(C)
* File name:    AreaPolygon
* Author:       魏晓亮
* Version:      1.0
* Date:         2021/01/05
* Description:  区域规划图的多边形区域，包含顶点数据
* History:
*************************************************/
#include <QJsonArray>
#include <QVector3D>
#include <QObject>
#include <QList>

class IMapPolygon;
class AreaPolygon : public QObject
{
    Q_OBJECT
public:
    explicit AreaPolygon(QObject *parent = nullptr);
    //AreaPolygon(const AreaPolygon&);
    //void operator=(const AreaPolygon&);
    //bool isNull();
    const QList<QVector3D> vertices() const;
    void setVertices(const QList<QVector3D>);
    // 多边形场景图形节点
    void setIMapPolygon(IMapPolygon*);
    IMapPolygon* getIMapPolygon();
    // 创建场景图形
    //void create();
    // 删除场景图形
    //void release();

signals:

public slots:

private:

private:
    // 保存多边形的场景图形节点
    IMapPolygon*           _iMapPolygon;
    QList<QVector3D>       _vertices;
};

#endif // AREAPOLYGON_H
