#include "ProcessConstruction.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QFile>
#include <QDebug>
#include <QDate>
#include <QDir>
#include <QMap>

ProcessConstruction::ProcessConstruction()
{
	_curRow		= 0;
}

ProcessConstruction::~ProcessConstruction()
{
	release();
}

void ProcessConstruction::start()
{
	_curRow = 0;
	Process::start();
}

bool ProcessConstruction::init(QString dbFilePath, QString tableName)
{
    QString fp = QCoreApplication::applicationDirPath() + "/../Data/Scenario/Scenario.db";
	//打开数据库（获取数据库接口）
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(fp);
	if (!db.open())
	{
		qDebug() << "database is error";
		return false;
	}
	QSqlQuery query(db);
	QSqlQueryModel* querymodel = new QSqlQueryModel();
	querymodel->setQuery("SELECT * FROM " + tableName, db);
	while (querymodel->canFetchMore())
	{
		querymodel->fetchMore();
	}
	for (int i = 0; i < querymodel->rowCount(); i++)
	{
		ProcessData data;
		data.dt = QDateTime::fromString(querymodel->data(querymodel->index(i, 0), Qt::DisplayRole).toString(),
			"yyyy/MM/dd");																			//第1列：日期	
		data.items = querymodel->data(querymodel->index(i, 1), Qt::DisplayRole).toString();			//第2列：施工内容	
		data.exp = querymodel->data(querymodel->index(i, 2), Qt::DisplayRole).toDouble();			//第3列：费用合计（万元）	
		data.stuff = querymodel->data(querymodel->index(i, 3), Qt::DisplayRole).toInt();			//第4列：人员
		_table.append(data);
	}
	return true;
}

void ProcessConstruction::release()
{
	////关闭数据库  
	//database.close();
	//
	////删除数据库  
	//QFile::remove("database.db");	
}

void ProcessConstruction::execute(QDateTime dt)
{
	//qDebug() << "ProcessConstruction execute, date:" << dt.date();
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
		//map.insert("chart_work", QString::number(float(_curRow) / _table.count()));
		//map.insert("chart_fund", QString::number(strWork.toFloat() / _totalFund));
		//map.insert("chart_staff", QString::number(dt.date().day()) + "," + strStaff);
		emit stateChanged(map);
		//qDebug() << "ProcessConstruction, Date:" << dt.date() << ", Event:" << strEvent;
	}
	if(_curRow > _table.count())	setFinished(true);
}
