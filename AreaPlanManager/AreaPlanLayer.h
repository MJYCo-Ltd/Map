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
#pragma once
#include <QObject>
#include <QString>
#include <QIcon>
#include <QTimer>
#include <QTime>

class AreaPlanLayer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
	AreaPlanLayer();
	virtual ~AreaPlanLayer();

	QString name();
	void setName(QString);
	QIcon legend();
	void setLegend(QIcon);
	QColor color();
	void setColor(QColor);

	void load(QString jsonFilePath);
	void save(QString jsonFilePath);
signals:
    void nameChanged();
private:
	QString										_name;
	QIcon										_legend;			
    QColor										_color;
};
