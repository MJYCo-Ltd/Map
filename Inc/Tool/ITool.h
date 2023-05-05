#ifndef INTERFACE_TOOL_HEADER_H
#define INTERFACE_TOOL_HEADER_H
#include <vector>
#include <string>

#include <SceneGraph/SceneType.h>
class ISceneGraph;
struct PickMessage
{
    virtual ~PickMessage(){}
    /**
     * @brief  鼠标选中目标信息
     * @param1 0 表示没有选中目标 非 0 表示选中目标ID
     * @param2 1 表述鼠标左键按下 2 表示鼠标右键双击
     */
    virtual void PickID(unsigned int,unsigned int){}

    virtual void ToolChanged(const char* csOldTool,const char* csNewTool){}
};

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
     * @brief 选择/取消工具
     */
    virtual bool SelecteTool(const std::string& sToolID)=0;
    virtual void UnSelecteTool()=0;
    virtual const std::string& CurrentTool()=0;

    /**
     * @brief 注册消息
     */
    virtual void SubPickMessage(PickMessage* pSub)=0;
    virtual void UnSubPickMessage(PickMessage* pSub)=0;
protected:
    virtual ~ITool(){}
protected:
    ISceneGraph* m_pSceneGraph{};
};

#endif//INTERFACE_TOOL_HEADER_H
