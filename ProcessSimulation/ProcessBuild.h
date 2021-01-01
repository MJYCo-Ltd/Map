#pragma once
/*************************************************
* Copyright(C)
* File name:    ProcessBuild
* Author:       魏晓亮
* Version:      1.1
* Date:         2021/1/1
* Description:  建造（模拟）过程。
* 				读取表中的施工进度，模拟展示建造过程。
* History:      2018/07/28 v1.0
*************************************************/
#include "Process.h"

class CommandBuildComponent;
class ProcessBuild : public Process
{
public:
    ProcessBuild();
    ~ProcessBuild();

    virtual void goTo(QDateTime dt);
    virtual const QDateTime startDateTime();
    virtual const QDateTime endDateTime();
    virtual const QDateTime currentDateTime();

private:
    void addDataItem(CommandBuildComponent*);
private:
    QList<CommandBuildComponent*>   _data;              // 进度数据（按日期时间排序）
    int                             _curRow;            // 模拟进度
    QDateTime                       _startDateTime;
    QDateTime                       _endDateTime;
};
