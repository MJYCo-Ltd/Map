#ifndef INTERFACE_TOOL_BASE_HEADER_H
#define INTERFACE_TOOL_BASE_HEADER_H

class ISceneGraph;
class CToolSelector;
/**
 * @brief 工具类基类
 */
class IToolBase
{
public:
    IToolBase(ISceneGraph* pSceneGraph,CToolSelector*pToolSelector):
        m_pSceneGraph(pSceneGraph),m_pToolSelector(pToolSelector){}

    /**
     * @brief 初始化工具
     */
    virtual bool InitTool()=0;

    /**
     * @brief 释放工具
     */
    virtual void ReleaseTool()=0;

    virtual ~IToolBase(){}
protected:
    ISceneGraph* m_pSceneGraph;
    CToolSelector* m_pToolSelector;
};

#endif//INTERFACE_TOOL_HEADER_H
