#pragma once
#include "Process.h"
/*************************************************
* Copyright(C)
* File name:    ProcessConstructionJson
* Author:       魏晓亮
* Version:      1.0
* Date:         2018/09/11
* Description:  建造（模拟）过程。
* 				读取Json中的施工进度，模拟展示建造过程。
* History:
*************************************************/

class ProcessConstructionJson :	public Process
{
	Q_OBJECT
public:
	ProcessConstructionJson();
	~ProcessConstructionJson();

	virtual void start();
	bool init(QString jsonFilePath);					// 读取json文件来初始化施工进度（待补充）
	void release();
	virtual void execute(QDateTime dt);				// 对比模拟时间dt和表格中读取的时间，来展示对应的建造进度

private:
	QList<ProcessData>		_table;	// 进度数据表
	int						_curRow;		
};
