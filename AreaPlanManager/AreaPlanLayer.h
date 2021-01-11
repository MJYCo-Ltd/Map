#pragma once
/*************************************************
* Copyright(C)
* File name:    AreaPlanLayer
* Author:       魏晓亮
* Version:      1.0
* Date:         2018/06/15
* Description:  区域规划图的图层
*				包含图层名、图例、区域多边形的顶点数据、显隐控制
* History:
*************************************************/
#include "AreaPlanManager_global.h"
#include <QObject>
#include <QString>
#include <QIcon>
#include <QTimer>
#include <QTime>

class AreaPolygon;
class AREAPLANMANAGER_EXPORT AreaPlanLayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString legend READ legend WRITE setLegend NOTIFY legendChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    AreaPlanLayer(QObject* parent = nullptr);
	virtual ~AreaPlanLayer();

	QString name();
	void setName(QString);
    QString legend();
    void setLegend(QString);
    //QIcon legend();
    //void setLegend(QIcon);
	QColor color();
	void setColor(QColor);

    bool isVisible();
    void setVisible(bool);

	void load(QString jsonFilePath);
	void save(QString jsonFilePath);

    void addAreaPolygon(AreaPolygon*);
    void removeAreaPlanPolygon(AreaPolygon*);

signals:
    void nameChanged();
    void legendChanged();
    void colorChanged();

private:
	QString										_name;
    QString										_legend;
    QColor										_color;
    QList<AreaPolygon*>                         _areaList;
};
