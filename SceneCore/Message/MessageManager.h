#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H
#include "Message/IMessageManager.h"
#include <QSet>
/**
 * @brief 消息管理者实现类
 */
class CMessageManager:public IMessageManager
{
public:
    CMessageManager();

    /**
     * @brief 增加事件监听者
     */
    bool AddObser(IMessageObserver*);

    /**
     * @brief 移除事件监听者
     */
    void RemoveObserver(IMessageObserver*);

    /**
     * @brief 发送消息
     */
    void SendMessage(const char *, ...);
protected:
    QSet<IMessageObserver*> m_setObserver;
};

#endif // MESSAGEMANAGER_H
