#pragma once
/*************************************************
* Copyright(C)
* File name:    Process
* Author:       魏晓亮
* Version:      1.0
* Date:         2018/07/28
* Description:  （模拟）过程抽象类
* 				子类化并实现execute来完成想要模拟的过程
* History:
*************************************************/
#include <QObject>
#include <QDateTime>
#include <QVariant>

struct ProcessData		// 进度数据
{
	QDateTime dt;		// 日期	
	QString items;		// 施工内容	
	double exp;			// 费用合计（万元）	
	int stuff;			// 人员
};

class Process : public QObject
{
	Q_OBJECT
public:
	Process();
	virtual void start();
	virtual void execute(QDateTime dt) = 0;
	virtual bool isFinished();
protected:
	virtual void setFinished(bool finished);
signals:
	void stateChanged(const QMap<QString, QString> &);
private:
	bool		_finished;
};
/*
class ProcessDebug : public Process
{
public:
	virtual void execute(QDateTime dt);
};*/

