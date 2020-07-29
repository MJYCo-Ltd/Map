#ifndef INTERFACE_OSG_SCENE_NODE_H
#define INTERFACE_OSG_SCENE_NODE_H

namespace osg
{
    class Group;
}

class IOsgSceneNode
{
public:
    virtual ~IOsgSceneNode(){}
    /**
     * @brief 获取显示节点
     * @return
     */
    virtual osg::Group* GetOsgNode()=0;

    /**
     * @brief 初始化
     */
    virtual void InitSceneNode()=0;
};
#endif
