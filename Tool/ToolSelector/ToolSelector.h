#ifndef CTOOLSELECTOR_H
#define CTOOLSELECTOR_H
#include <map>
#include <NoQt.h>
#include <Tool/ITool.h>

class IToolBase;
class CToolSelector:public ITool
{
public:
    CONSTRUCTOR(CToolSelector,ITool)

    /**
     * @brief 注册工具
     */
    void RegisterTool();

    /**
     * @brief 获取所有的工具
     * @return
     */
    std::vector<std::string> AllTool()const;

    /**
     * @brief 选择工具和取消工具
     */
    bool SelecteTool(const std::string& sToolID);
    void UnSelecteTool();
    const std::string& CurrentTool(){return(m_sCurrentToolID);}
protected:
    void CreateTool(const std::string &sInterface);
    void InitType(const std::string &sInterface);

private:
    typedef IToolBase* (*pCreateToolFun)(ISceneGraph*,const std::string&);
    typedef bool(*pQueryInterfaceFun)(std::string&);

    std::map<std::string,std::string> m_mapTypeDllName;
    std::map<std::string,pCreateToolFun> m_mapTypeFunc;
    std::string  m_sCurrentToolID;
    IToolBase *  m_pCurrentTool=nullptr;
};

extern "C"
{
    Q_DECL_EXPORT ITool* CreateToolSelector(ISceneGraph* pSceneGraph);
};
#endif // CTOOLSELECTOR_H
