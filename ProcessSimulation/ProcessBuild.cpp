#include "ProcessBuild.h"
#include "CommandBuildComponent.h"
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


void ProcessBuild::goTo(QDateTime dt)
{
    //qDebug() << "== ProcessBuild::goTo ==========";
    //qDebug() << "dt ：" << dt;
    //qDebug() << "ProcessBuild::startDateTime ：" << startDateTime();
    //qDebug() << "ProcessBuild::startDateTime ：" << endDateTime();
    //qDebug() << "ProcessBuild::_curRow ：" << _curRow;
    //qDebug() << "ProcessBuild::currentDateTime ：" << currentDateTime();
    //qDebug() << "================================";
    if (dt < startDateTime() || dt > endDateTime()) // 日期时间超出范围
	{
		return;
	}
    if (dt == currentDateTime())                // 日期时间不变，无需跳转
        return;
    else if (dt > currentDateTime())            // 目标时间比当前日期时间晚，前进
    {
        for (int r = _curRow; r < _data.count(); r++)
        {
            _curRow = r;
            if (dt < currentDateTime())
                break;
            (_data[_curRow])->execute();

            qDebug() << "-------------------------------";
            qDebug() << "日期： " << _data[_curRow]->dateTime() ;
            qDebug() << "构件："<< _data[_curRow]->name() << ", 花费：" << _data[_curRow]->expense() << ",人员：" << _data[_curRow]->headcount();
            qDebug() << "-------------------------------";
        }
    }
    else if (dt < currentDateTime())            // 目标时间比当前日期时间晚早，撤回
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

void ProcessBuild::setStartDateTime(const QDateTime dt)
{
    _startDateTime = dt;
}

void ProcessBuild::setEndDateTime(const QDateTime dt)
{
    _endDateTime = dt;
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
    // 第一条数据，直接插入即可
    if (_data.count() == 0)
    {
        _data.append(cmd);
        // 更新时间范围
        setStartDateTime(cmd->dateTime());
        setEndDateTime(cmd->dateTime());
        return;
    }
    else
    {
        // 更新时间范围
        if (startDateTime() > cmd->dateTime())
            setStartDateTime(cmd->dateTime());
        if (endDateTime() < cmd->dateTime())
            setEndDateTime(cmd->dateTime());
        qDebug() << "==========";
        qDebug() << "cmd->dateTime() :" << cmd->dateTime();
        qDebug() << "startDateTime:" << startDateTime();
        qDebug() << "endDateTime:" << endDateTime();
        for (int i = 0; i < _data.count(); i++)
        {
            if (cmd == _data[i])    // 不重复添加命令
                return;
            if (cmd->dateTime() < _data[i]->dateTime()) // 比当前日期时间早则插入
            {
                _data.insert(i, cmd);
                return;
            }
        }
        // 比所有项日期都晚，加在末尾
        _data.append(cmd);
    }
}

void ProcessBuild::addComponents(QList<CommandBuildComponent*> list)
{
    for(int i = 0; i < list.count(); i++)
    {
        addComponent(list[i]);
    }
}
