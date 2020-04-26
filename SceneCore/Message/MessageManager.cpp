#include "MessageManager.h"

CMessageManager::CMessageManager()
{

}

/// 添加事件监听者
bool CMessageManager::AddObser(IMessageObserver * pObserver)
{
    if(m_setObserver.contains(pObserver))
    {
        return(false);
    }
    else
    {
        m_setObserver.insert(pObserver);
        return(true);
    }
}

/// 移除事件监听者
void CMessageManager::RemoveObserver(IMessageObserver *pObserver)
{
    m_setObserver.remove(pObserver);
}

/// 发送消息
void CMessageManager::SendMessage(const char *cformat, ...)
{
//    va_start();
}
