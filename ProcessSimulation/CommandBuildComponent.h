#pragma once
/*************************************************
* Copyright(C)
* File name:    CommandBuildComponent
* Author:       魏晓亮
* Version:      1.1
* Date:         2020/12/31
* Description:  建造构件命令
* 				用于施工过程模拟
*               调用模型接口实现execute(建造)undo（拆除）命令
* History:      2018/07/28 v1.0
*************************************************/
#include "ProcessSimulation_global.h"
#include <QObject>
#include <QDateTime>
#include <QString>

// 命令接口
class Command{
public:
    virtual ~Command(){}
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// 项目（模型构件）施工命令
class PROCESSSIMULATION_EXPORT CommandBuildComponent :public QObject, public Command
{
    Q_OBJECT
public:
    CommandBuildComponent();
    ~CommandBuildComponent();

    void execute();     // 建造，调用模型接口显示项目（模型构件）
    void undo();        // 拆除，调用模型接口隐藏项目（模型构件）

    void setExpense(double);
    double expense();               // 花费
    void setHeadcount(double);
    int headcount();                // 人员
    void setName(QString);
    QString name();                 // 构件名称（KEY：通过此项查询并控制模型构件显隐）
    void setDateTime(QDateTime);
    QDateTime dateTime();           // 日期

private:
    double      _expense;
    int         _headcount;
    QString     _name;
    QDateTime   _datetime;
};
