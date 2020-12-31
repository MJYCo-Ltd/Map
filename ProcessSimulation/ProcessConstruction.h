#pragma once
#include "Process.h"
/*************************************************
* Copyright(C)
* File name:    ProcessConstruction
* Author:       魏晓亮
* Version:      1.0
* Date:         2018/07/28
* Description:  建造（模拟）过程。
* 				读取表中的施工进度，模拟展示建造过程。
* History:
*************************************************/

class ProcessConstruction :	public Process
{
	Q_OBJECT
public:
	ProcessConstruction();
	~ProcessConstruction();

	virtual void start();
	bool init(QString dbFilePath, QString tableName);	// 读取指定表格来初始化施工进度，表格读取失败时返回false
	void release();
	virtual void execute(QDateTime dt);				// 对比模拟时间dt和表格中读取的时间，来展示对应的建造进度

private:
	QList<ProcessData>		_table;	// 进度数据表
	int						_curRow;		
};
