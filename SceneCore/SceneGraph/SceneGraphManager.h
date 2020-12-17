#ifndef CSCENEGRAPHMANAGER_H
#define CSCENEGRAPHMANAGER_H
#include <list>
#include "SceneGraph/ISceneGraphManager.h"

class QtSceneGraph;
class CSceneGraphManager:public ISceneGraphManager
{
public:
    CSceneGraphManager();
    ~CSceneGraphManager();

    /**
     * @brief 创建一个场景图
     * @return
     */
    ISceneGraph* CreateSceneGraph(SceneGraphType typeScene);

    /**
     * @brief 查找绑定的场景图
     * @param pWindow
     * @return
     */
    ISceneGraph* FindSceneGraph(IWindow* pWindow);

    /**
     * @brief 查找绑定的场景图
     * @param pItem
     * @return
     */
    ISceneGraph* FindSceneGraph(QQuickItem* pItem);

    /**
     * @brief 删除一个
     * @return
     */
    bool DeleteSceneGraph(ISceneGraph*& rForDelete);
private:
    std::list<QtSceneGraph*> m_vSceneGraph; /// 保存所有创建的地图
};

#endif // CSCENEGRAPHMANAGER_H
