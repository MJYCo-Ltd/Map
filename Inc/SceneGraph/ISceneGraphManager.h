#ifndef INTERFACE_SCENE_MANAGER_HEARDER_H
#define INTERFACE_SCENE_MANAGER_HEARDER_H
#include <string>
#include "ISceneGraph.h"

using namespace std;
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
     * @param 场景路径
     * @attention 如果路径中有中文,需要使用utf-8编码的路径,
     *            否则,可能不识别。
     * @return 创建好的场景图
     */
    virtual ISceneGraph* CreateSceneGraph(SceneGraphType,const string&)=0;

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
