#include <fstream>
#include <sstream>
#include <algorithm>

#include <QLibrary>

#include <SceneGraph/ISceneGraph.h>
#include <ISceneCore.h>
#include <Map/IMapSceneNode.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IRender.h>
#include <Inner/IOsgSceneNode.h>
#include "MapNodeFactory.h"

CMapNodeFactory::CMapNodeFactory(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    InitFactory();
}

/// 析构模型工厂
CMapNodeFactory::~CMapNodeFactory()
{
    for(auto one : m_allCreateNode)
    {
        auto pGroup = dynamic_cast<IOsgSceneNode*>(one)->GetOsgNode();
        if(nullptr != pGroup)
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(pGroup));
        }
    }

    m_allCreateNode.clear();
}

/// 创建一个节点
IMapSceneNode *CMapNodeFactory::CreateMapSceneNode(const string& sInterface)
{
    auto findOne = m_mapTypeFunc.find(sInterface);

    if (m_mapTypeFunc.end() == findOne)
    {
        InitType(sInterface);
    }

    findOne = m_mapTypeFunc.find(sInterface);
    if(m_mapTypeFunc.end() != findOne)
    {
        IMapSceneNode* pNode = findOne->second(m_pSceneGraph,sInterface);
        if(nullptr != pNode)
        {
            dynamic_cast<IOsgSceneNode*>(pNode)->InitSceneNode();
            m_allCreateNode.push_back(pNode);
        }

        return(pNode);
    }
    else
    {
        return(nullptr);
    }
}

/// 删除地图节点
bool CMapNodeFactory::DeleteMapSceneNode(IMapSceneNode *pMapSceneNode)
{
    auto findResult = find(m_allCreateNode.begin(),m_allCreateNode.end(),pMapSceneNode);
    if(m_allCreateNode.end() != findResult)
    {
        m_allCreateNode.erase(findResult);
        delete pMapSceneNode;
        return(true);
    }
    return(false);
}

/// 初始化工厂
void CMapNodeFactory::InitFactory()
{
    string sFilePath = GetExePath();
#ifdef QT_NO_DEBUG
    sFilePath += "NodeFactory.dll";
#else
    sFilePath += "NodeFactoryd.dll";
#endif
    ifstream in(sFilePath, ios::in);
    string sInterfaceType;
    string sDllName;
    if(in.is_open())
    {
        while(!in.eof())
        {
            in>>sInterfaceType>>sDllName;
            m_mapTypeDllName[sInterfaceType] = sDllName;
        }
    }
}

/// 初始化类型
void CMapNodeFactory::InitType(const string &sInterface)
{
    QLibrary loadDll;
    auto findOne = m_mapTypeDllName.find(sInterface);
    if (m_mapTypeDllName.end() != findOne)
    {
        string sInterfaceNameList;
        loadDll.setFileName(findOne->second.c_str());
        auto pQueryFunc = reinterpret_cast<pQueryInterfaceFun>(loadDll.resolve("QueryInterface"));
        auto pFunc = reinterpret_cast<pCreateNodeFun>(loadDll.resolve("CreateNode"));
        if (nullptr != pFunc && nullptr != pQueryFunc)
        {
            if(pQueryFunc(sInterfaceNameList))
            {
                std::istringstream is(sInterfaceNameList);
                while(is)
                {
                    is>>sInterfaceNameList;
                    m_mapTypeFunc[sInterfaceNameList] = pFunc;
                }
            }
            else
            {
                m_mapTypeFunc[sInterface] = pFunc;
            }
        }
    }
}
