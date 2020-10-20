#include <fstream>
#include <sstream>
#include <algorithm>
#include <osg/Notify>
#include <QLibrary>

#include <SceneGraph/ISceneGraph.h>
#include <ISceneCore.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IRender.h>
#include <Inner/IOsgSceneNode.h>
#include "MapNodeFactory.h"

CMapNodeFactory::CMapNodeFactory(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
}

/// 析构模型工厂
CMapNodeFactory::~CMapNodeFactory()
{
    killTimer(m_nTimerID);
    for(auto one : m_allCreateNode)
    {
        auto pGroup =one->AsOsgSceneNode()->GetOsgNode();
        if(nullptr != pGroup)
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(pGroup));
        }
    }

    DeleteNoUseSceneNode();

    m_allCreateNode.clear();
}

/// 创建一个节点
ISceneNode *CMapNodeFactory::CreateMapSceneNode(const string& sInterface)
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
        ISceneNode* pNode = findOne->second.pCrete(m_pSceneGraph,sInterface);
        if(nullptr != pNode)
        {
            m_mapDeleteFunc[pNode] = findOne->first;

            pNode->AsOsgSceneNode()->Init();
            m_allCreateNode.push_back(pNode);
        }

        return(pNode);
    }
    else
    {
        return(nullptr);
    }
}

/// 删除没用的节点
void CMapNodeFactory::DeleteNoUseSceneNode()
{
    for(auto one=m_allCreateNode.begin();one != m_allCreateNode.end();)
    {
        auto pMapSceneNode = *one;
        if(pMapSceneNode->AsOsgSceneNode()->CanDelete())
        {
            delete pMapSceneNode->AsOsgSceneNode();
            one = m_allCreateNode.erase(one);
        }
        else
        {
            ++one;
        }
    }
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

    m_nTimerID = startTimer(60000);
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
        if(nullptr == pQueryFunc)
        {
            osg::notify(osg::NOTICE)<<loadDll.errorString().toStdString();
        }

        auto pCreateFun = reinterpret_cast<pCreateNodeFun>(loadDll.resolve("CreateNode"));

        if (nullptr != pCreateFun && nullptr != pQueryFunc)
        {
            if(pQueryFunc(sInterfaceNameList))
            {
                std::istringstream is(sInterfaceNameList);
                string sType;
                while(is)
                {
                    is>>sType;
                    m_mapTypeFunc[sType].pCrete = pCreateFun;
                }
            }
            else
            {
                m_mapTypeFunc[sInterface].pCrete = pCreateFun;
            }
        }
    }
}

#include <QTimerEvent>
void CMapNodeFactory::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID)
    {
        DeleteNoUseSceneNode();
    }
}