#include "AreaPlanLayer.h"
#include "AreaPolygon.h"
#include "AreaPolygonLoader.h"
#include "AreaPolygonEditor.h"
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QFile>

AreaPlanLayer::AreaPlanLayer(QObject* parent):QObject(parent)
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

QString AreaPlanLayer::legend()
{
    return _legend;
}

void AreaPlanLayer::setLegend(QString legend)
{
    _legend = legend;
}
/*
QIcon AreaPlanLayer::legend()
{
	return _legend;
}

void AreaPlanLayer::setLegend(QIcon legend)
{
	_legend = legend;
}*/

QColor AreaPlanLayer::color()
{
	return _color;
}

void AreaPlanLayer::setColor(QColor color)
{
	_color = color;
}

bool AreaPlanLayer::isVisible()
{
    return AreaPolygonEditor::getInstance()->isVisible(this);
}

void AreaPlanLayer::setVisible(bool visible)
{
    AreaPolygonEditor::getInstance()->setVisible(this, visible);
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
                AreaPolygon* ap = AreaPolygonLoader::getInstance()->fromJson(arrayPoint);
                addAreaPolygon(ap);
            }
        }
	}
}

void AreaPlanLayer::save(QString jsonFilePath)
{
    qDebug() << "AreaPlanLayer::save file:" << jsonFilePath;
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
        QJsonArray jsonArray = AreaPolygonLoader::getInstance()->toJson(_areaList[i]);
        QJsonValue value(jsonArray);
        areaArray.append(value);
    }

	QJsonDocument document;
	document.setArray(areaArray);

	in << document.toJson();
	file.close();
}

void AreaPlanLayer::addAreaPolygon(AreaPolygon* ap)
{
    AreaPolygonEditor::getInstance()->createPolygon(ap, this);
    _areaList.append(ap);

    emit areaListChanged();
}

void AreaPlanLayer::removeAreaPlanPolygon(AreaPolygon* ap)
{
    AreaPolygonEditor::getInstance()->deletePolygon(ap, this);
    _areaList.removeOne(ap);
}

QList<AreaPolygon*>  AreaPlanLayer::areaList()
{
    return _areaList;
}
