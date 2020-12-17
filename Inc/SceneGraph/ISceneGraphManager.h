#ifndef INTERFACE_SCENE_MANAGER_HEARDER_H
#define INTERFACE_SCENE_MANAGER_HEARDER_H
#include <string>
#include <SceneGraph/ISceneGraph.h>

/**
 * @brief 场景类
 */
class ISceneGraphManager
{
public:
    virtual ~ISceneGraphManager(){}

    /**
     * @brief 创建场景图
     * @param 场景类型
     * @return 创建好的场景图
     */
    virtual ISceneGraph* CreateSceneGraph(SceneGraphType)=0;

    /**
     * @brief 根据窗口查找关联的SceneGraph
     * @return
     */
    virtual ISceneGraph* FindSceneGraph(IWindow*)=0;

    /**
     * @brief 根据quickitem 查找关联的SceneGraph
     * @return
     */
    virtual ISceneGraph* FindSceneGraph(QQuickItem*)=0;

    /**
     * @brief 删除场景图
     * @return
     */
    virtual bool DeleteSceneGraph(ISceneGraph*&)=0;
};

#endif
