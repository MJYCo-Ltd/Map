#ifndef CTOOLSELECTOR_H
#define CTOOLSELECTOR_H
#include <map>
#include <list>
#include <NoQt.h>
#include <Tool/ITool.h>
#include "../IToolPub.h"

class IToolBase;
class CToolSelector:public ITool,public IToolPub
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

    /**
     * @brief 订阅消息
     */
    void SubPickMessage(PickMessage* pSub);
    void UnSubPickMessage(PickMessage* pSub);

    /**
     * @brief 点选消息
     */
    void PickID(unsigned int);
protected:
    void CreateTool(const std::string &sInterface);
    void InitType(const std::string &sInterface);

private:
    typedef IToolBase* (*pCreateToolFun)(ISceneGraph*,CToolSelector*,const std::string&);
    typedef bool(*pQueryInterfaceFun)(std::string&);

    std::list<PickMessage*> m_listPickMessage;
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
