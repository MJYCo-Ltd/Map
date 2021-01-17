#ifndef INTERFACE_TOOL_HEADER_H
#define INTERFACE_TOOL_HEADER_H

#include <SceneGraph/ISceneGroup.h>

/**
 * @brief 标绘管理类
 */
class ITool
{
    friend class ISceneGraph;
public:

    CONSTRUCTOR(ITool,m_pSceneGraph)

    /**
     * @brief 注册标绘类型
     */
    virtual void RegisterTool()=0;

protected:
    virtual ~ITool(){}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif//INTERFACE_TOOL_HEADER_H
