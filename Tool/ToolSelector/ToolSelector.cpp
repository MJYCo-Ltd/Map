#include <QLibrary>
#include <osg/Notify>
#include <fstream>
#include <sstream>
#include <ISceneCore.h>

#include "../IToolBase.h"
#include "ToolSelector.h"

ITool* CreateToolSelector(ISceneGraph* pSceneGraph)
{
    return(new CToolSelector(pSceneGraph));
}

/// 注册工具
void CToolSelector::RegisterTool()
{
    static std::string sPrefix = "MapPlugin/Tool/";

    std::string sFilePath = GetExePath();
    sFilePath += sPrefix + "ToolFactory.dll";
    std::ifstream in(sFilePath, std::ios::in);
    std::string sInterfaceType;
    std::string sDllName;

    sFilePath = GetExePath();
    if(in.is_open())
    {
        while(!in.eof())
        {
            in>>sInterfaceType>>sDllName;
            sDllName += s_sSuffix;
            m_mapTypeDllName[sInterfaceType] = sFilePath + sPrefix + sDllName;
        }
    }
}

/// 获取所有的工具
std::vector<std::string> CToolSelector::AllTool() const
{
    std::vector<std::string> vAllToolID;
    for(auto one:m_mapTypeDllName)
    {
        vAllToolID.push_back(one.first);
    }

    return(vAllToolID);
}

/// 选择工具
bool CToolSelector::SelecteTool(const std::string &sToolID)
{
    if(m_sCurrentToolID == sToolID)
    {
        return(false);
    }
    else
    {
        CreateTool(sToolID);
        if(nullptr == m_pCurrentTool)
        {
            return(false);
        }
    }

    m_sCurrentToolID = sToolID;
    return(true);
}

/// 取消工具
void CToolSelector::UnSelecteTool()
{
    if(nullptr != m_pCurrentTool)
    {
        m_pCurrentTool->ReleaseTool();
        delete m_pCurrentTool;
        m_pCurrentTool = nullptr;
    }
}

void CToolSelector::SubPickMessage(PickMessage* pSub)
{
    m_listPickMessage.push_back(pSub);
}

void CToolSelector::UnSubPickMessage(PickMessage* pSub)
{
    auto pFindOne = std::find(m_listPickMessage.begin(),m_listPickMessage.end(),pSub);
    if(m_listPickMessage.end() != pFindOne)
    {
        m_listPickMessage.erase(pFindOne);
    }
}

void CToolSelector::PickID(unsigned int unID)
{
    for(auto one : m_listPickMessage)
    {
        one->PickID(unID);
    }
}

/// 创建工具
void CToolSelector::CreateTool(const std::string &sInterface)
{
    /// 没有找到接口函数，则初始化一下
    auto findOne = m_mapTypeFunc.find(sInterface);
    if (m_mapTypeFunc.end() == findOne)
    {
        InitType(sInterface);
    }

    findOne = m_mapTypeFunc.find(sInterface);
    if(m_mapTypeFunc.end() != findOne)
    {
        if(nullptr != m_pCurrentTool)
        {
            delete m_pCurrentTool;
        }

        m_pCurrentTool = findOne->second(m_pSceneGraph,this,sInterface);
        if(nullptr != m_pCurrentTool)
        {
            m_pCurrentTool->InitTool();
        }
    }
}

/// 初始化类型
void CToolSelector::InitType(const std::string &sInterface)
{
    QLibrary loadDll;
    auto findOne = m_mapTypeDllName.find(sInterface);
    if (m_mapTypeDllName.end() != findOne)
    {
        std::string sInterfaceNameList;
        loadDll.setFileName(findOne->second.c_str());
        auto pQueryFunc = reinterpret_cast<pQueryInterfaceFun>(loadDll.resolve("QueryInterface"));
        if(nullptr == pQueryFunc)
        {
            osg::notify(osg::WARN)<<loadDll.errorString().toStdString();
        }

        auto pCreateFun = reinterpret_cast<pCreateToolFun>(loadDll.resolve("CreateTool"));

        if (nullptr != pCreateFun && nullptr != pQueryFunc)
        {
            if(pQueryFunc(sInterfaceNameList))
            {
                std::istringstream is(sInterfaceNameList);
                std::string sType;
                while(is)
                {
                    is>>sType;
                    m_mapTypeFunc[sType] = pCreateFun;
                }
            }
            else
            {
                m_mapTypeFunc[sInterface] = pCreateFun;
            }
        }
    }
}
