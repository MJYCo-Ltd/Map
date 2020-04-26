#ifndef SCENCECORE_HEADER_H
#define SCENCECORE_HEADER_H

#include "ISceneCore.h"

class CSceneGraphManager;
class CMessageManager;
/**
 * @brief 整个场景的核心库
 */
class CSceneCore:public ISceneCore
{
public:
    CSceneCore();
    ~CSceneCore();

    /**
     * @brief 获取窗口管理者
     * @return
     */
    ISceneGraphManager* GetSceneGraphManager();

    /**
     * @brief 获取消息管理者
     * @return
     */
    IMessageManager* GetMessageManager();

protected:
    void InitSystem();
    void InitLog();

protected:
    CSceneGraphManager*  m_pSceneGraphManger;
    CMessageManager*     m_pMessageManager;
};

ISceneCore* GetMapCore();
bool         CheckPC();
#endif // SCENCECORE_H
