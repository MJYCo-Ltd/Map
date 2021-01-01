#include "ProcessBuild.h"
#include "CommandBuildComponent.h"
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QMap>

ProcessBuild::ProcessBuild()
{
    _curRow         = 0;
    _startDateTime  = QDateTime::currentDateTime();
    _endDateTime    = QDateTime::currentDateTime();
}

ProcessBuild::~ProcessBuild()
{
    for (int i = 0; i < _data.count(); i++)
    {
        if (_data[i])
        {
            delete _data[i];
        }
    }
    _data.clear();
}
/*
bool init(QString dbFilePath, QString tableName)
{
    //QString dbFilePath = QCoreApplication::applicationDirPath() + "/../Data/Scenario/Scenario.db";
	//打开数据库（获取数据库接口）
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbFilePath);
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

	////关闭数据库  
	//database.close();
	//
	////删除数据库  
	//QFile::remove("database.db");	
}
*/
void ProcessBuild::goTo(QDateTime dt)
{
    if (dt < startDateTime() || dt > endDateTime()) // 日期时间超出范围
	{
		return;
	}
    if (dt == currentDateTime())                // 日期时间不变，无需跳转
        return;
    else if (dt < currentDateTime())            // 当前日期时间比目标时间晚，前进
    {
        for (int r = _curRow; r < _data.count(); r++)
        {
            _curRow = r;
            if (dt < currentDateTime())
                break;
            (_data[_curRow])->execute();
            qDebug() << "ProcessBuild::goTo " << _data[_curRow]->dateTime() << "," << _data[_curRow]->name();
        }
    }
    else if (dt > currentDateTime())            // 当前日期时间比目标时间早，撤回
    {
        for (int r = _curRow; r >= 0; r--)
        {
            _curRow = r;
            if (dt > currentDateTime())
                break;
            (_data[_curRow])->undo();
            qDebug() << "ProcessBuild::goTo," << _data[_curRow]->dateTime() << "," << _data[_curRow]->name();
        }
    }
}

const QDateTime ProcessBuild::startDateTime()
{
    return _startDateTime;
}

const QDateTime ProcessBuild::endDateTime()
{
    return _endDateTime;
}

const QDateTime ProcessBuild::currentDateTime()
{
    if(_curRow < 0 || _curRow >= _data.count())
    {
        // qDebug() << "ProcessBuild::currentDateTime, _curRow : out of range!"
        return QDateTime();
    }
    return (_data[_curRow])->dateTime();
}

void ProcessBuild::addComponent(CommandBuildComponent* cmd)
{
    for (int i = 0; i < _data.count() - 1; i++)
    {
        if (cmd == _data[i])    // 不重复添加命令
            return;
        if (cmd->dateTime() >= _data[i]->dateTime()
                && cmd->dateTime() < _data[i+1]->dateTime())
        {
            _data.insert(i+1, cmd);
            return;
        }
    }
    _data.append(cmd);
}
