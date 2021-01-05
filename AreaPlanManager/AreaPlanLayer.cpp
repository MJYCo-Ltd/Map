#include "AreaPlanLayer.h"
#include "AreaPolygon.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

AreaPlanLayer::AreaPlanLayer()
{
}

AreaPlanLayer::~AreaPlanLayer()
{
}

QString AreaPlanLayer::name()
{
	return _name;
}

void AreaPlanLayer::setName(QString name)
{
	_name = name;
}

QIcon AreaPlanLayer::legend()
{
	return _legend;
}

void AreaPlanLayer::setLegend(QIcon legend)
{
	_legend = legend;
}

QColor AreaPlanLayer::color()
{
	return _color;
}

void AreaPlanLayer::setColor(QColor color)
{
	_color = color;
}

void AreaPlanLayer::load(QString jsonFilePath)
{
	QFile file(jsonFilePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "File open error";
	}
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonError);
	if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))
	{
        if ( ! document.isArray())
		{
            return;
        }
        int debug_point_not_enough = 0;
        QJsonArray arrayArea = document.array();
        // 遍历所有多边形区域
        for (int i = 0; i < arrayArea.size(); i++)
        {
            QJsonValue value = arrayArea.at(i);
            if (value.isArray())
            {
                QJsonArray arrayPoint = value.toArray();
                // 不足3个顶点，无效多边形数据
                if (arrayPoint.size() < 3)
                {
                    debug_point_not_enough++;
                    qDebug() << "file:" << jsonFilePath << ", " << debug_point_not_enough << ":point not enough!";
                    continue;
                }
                AreaPolygon* area = new AreaPolygon;
                area->fromJson(arrayPoint);
                _areaList.append(area);
            }
        }
	}
}

void AreaPlanLayer::save(QString jsonFilePath)
{
	QFile file(jsonFilePath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
	{
		qDebug() << "file error";
		return;
	}
	QTextStream in(&file);        
    QJsonArray areaArray;
    for(int i = 0; i < _areaList.count(); i++)
    {
        QJsonValue value(_areaList[i]->toJson());
        areaArray.append(value);
    }

	QJsonDocument document;
	document.setArray(areaArray);

	in << document.toJson();
	file.close();
}

