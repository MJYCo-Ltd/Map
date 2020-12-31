#include "ProcessConstructionJson.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QDir>
#include <QMap>

ProcessConstructionJson::ProcessConstructionJson()
{
	_curRow		= 0;
}

ProcessConstructionJson::~ProcessConstructionJson()
{
	release();
}

void ProcessConstructionJson::start()
{
	_curRow = 0;
	Process::start();
}

bool ProcessConstructionJson::init(QString jsonFilePath)
{
	// 以只读方式打开主目录下的1.json文件，若该文件不存在则会自动创建
	QFile file(jsonFilePath);
	if (!file.open(QIODevice::ReadOnly)) {
		qDebug() << "File open error";
	}
	else {
		qDebug() << "File open!";
	}
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &jsonError);	//转化为JSON文档
	if (!document.isNull() && (jsonError.error == QJsonParseError::NoError))		//解析未发生错误
	{
		if (document.isObject())
		{
			QJsonObject object = document.object();

			// 第一项
			if (object.contains("Company"))
			{
				QJsonValue value = object.value("Company");
				if (value.isString())
				{
					QString strName = value.toString();
					qDebug() << strName;
				}
			}

			ProcessData data;
			//第1列：日期	
			//第2列：施工内容	
			//第3列：费用合计（万元）	
			//第4列：人员
			_table.append(data);
		}
	}
	return true;
}

void ProcessConstructionJson::release()
{
}

void ProcessConstructionJson::execute(QDateTime dt)
{
	if (isFinished())
	{
		return;
	}

	for (int r = _curRow; r < _table.count(); r++)	
	{
		_curRow = r;
		ProcessData data = _table[r];
		if (dt < data.dt)	// 日期时间比dt晚的一律不执行			
			break;

		// 事件
		QString strEvent = data.items;
		// 人员
		//QString strStaff = QString::number(data.stuff);
		// 发送状态变化信号
		QMap<QString, QString> map;
		map.insert("event", strEvent);
		emit stateChanged(map);
	}
	if(_curRow > _table.count())	setFinished(true);
}
