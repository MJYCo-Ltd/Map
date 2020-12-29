#ifndef SCENCECORE_HEADER_H
#define SCENCECORE_HEADER_H

#include <ISceneCore.h>

class CSceneGraphManager;
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

protected:
    void InitSystem();
    void InitLog();

protected:
    CSceneGraphManager*  m_pSceneGraphManger;
};

ISceneCore* GetMapCore();
bool         CheckPC();
#endif // SCENCECORE_H
