#ifndef INTERFACE_TOOL_HEADER_H
#define INTERFACE_TOOL_HEADER_H
#include <vector>
#include <string>

#include <SceneGraph/SceneType.h>

/**
 * @brief 工具类
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

    /**
     * @brief 获取所有的工具
     */
    virtual std::vector<std::string> AllTool()const=0;

    /**
     * @brief 选择工具
     */
    virtual bool SelecteTool(const std::string& sToolID)=0;
    virtual void UnSelecteTool()=0;
    virtual const std::string& CurrentTool()=0;

protected:
    virtual ~ITool(){}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif//INTERFACE_TOOL_HEADER_H
