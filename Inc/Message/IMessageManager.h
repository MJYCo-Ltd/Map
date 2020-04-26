#ifndef INTERFACE_MESSAGE_MANAGER_H
#define INTERFACE_MESSAGE_MANAGER_H

#include "IMessageObserver.h"

struct IMessageManager
{
    virtual ~IMessageManager(){}

    /**
     * @brief 增加事件监听者
     */
    virtual bool AddObser(IMessageObserver*)=0;

    /**
     * @brief 移除事件监听者
     */
    virtual void RemoveObserver(IMessageObserver*)=0;
};

#endif
