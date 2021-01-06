#ifndef AREAPOLYGONLOADER_H
#define AREAPOLYGONLOADER_H
/*************************************************
* Copyright(C)
* File name:    AreaPolygonLoader
* Author:       魏晓亮
* Version:      1.0
* Date:         2021/01/05
* Description:  区域规划图的Json读写
* History:
*************************************************/
#include <QObject>

class AreaPolygon;
class AreaPolygonLoader : public QObject
{
    Q_OBJECT
private:
    explicit AreaPolygonLoader();
    static AreaPolygonLoader* _self;
public:
    static AreaPolygonLoader* getInstance()
    {
        if(_self == nullptr)
            _self = new AreaPolygonLoader();
        return _self;
    }

    QJsonArray toJson(AreaPolygon*);
    AreaPolygon* fromJson(QJsonArray);
signals:

public slots:
};

#endif // AREAPOLYGONLOADER_H
