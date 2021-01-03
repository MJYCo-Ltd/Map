#pragma once
/*************************************************
* Copyright(C)
* File name:    Process
* Author:       魏晓亮
* Version:      1.1
* Date:         2021/1/1
* Description:  模拟过程接口
* 				子类化并实现goTo等接口来完成想要模拟的过程
*               模拟时间在startDateTime和endDateTime之间goTo将被调用
* History:      2018/07/28 v1.0
*************************************************/
#include <QDateTime>

class Process
{
public:
    virtual ~Process(){}

    // 跳转到该时间的状态
    virtual void goTo(QDateTime dt) = 0;

    // 获取开始时间
    virtual const QDateTime startDateTime() = 0;

    // 获取结束时间
    virtual const QDateTime endDateTime() = 0;
};

