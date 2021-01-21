#ifndef INTERFACE_TOOL_BASE_HEADER_H
#define INTERFACE_TOOL_BASE_HEADER_H

#include <SceneGraph/SceneType.h>

class ISceneGraph;
/**
 * @brief 工具类基类
 */
class IToolBase
{
public:
    CONSTRUCTOR(IToolBase,m_pSceneGraph)

    /**
     *初始化工具
     */
    virtual void InitTool()=0;
    virtual void ReleaseTool()=0;

    virtual ~IToolBase(){}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif//INTERFACE_TOOL_HEADER_H
