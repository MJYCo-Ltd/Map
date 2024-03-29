#include <fstream>
#include <sstream>
#include <algorithm>
#include <osg/Notify>
#include <QLibrary>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/ISceneNode.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IRender.h>
#include <Inner/ILoadResource.h>
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
    for(const CanDeleteNode &one : m_allCreateNode)
    {
        auto pGroup =one.pSceneNode->AsOsgSceneNode()->GetRealNode();
        if(nullptr != pGroup)
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(pGroup));
        }
    }

    DeleteNoUseSceneNode();

    m_allCreateNode.clear();
}

/// 创建一个节点
ISceneNode *CMapNodeFactory::CreateSceneNode(const std::string& sInterface)
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
            CanDeleteNode tmpDelete;
            tmpDelete.pSceneNode = pNode;
            pNode->AsOsgSceneNode()->Init();
            m_allCreateNode.push_back(tmpDelete);
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
        if(one->nTimes > 3)
        {
            delete one->pSceneNode->AsOsgSceneNode();
            one = m_allCreateNode.erase(one);
        }
        else
        {
            if(one->pSceneNode->AsOsgSceneNode()->CanDelete())
            {
                ++(one->nTimes);
            }
            else
            {
                one->nTimes = 0;
            }
            ++one;
        }
    }
}

/// 初始化工厂
void CMapNodeFactory::InitFactory()
{
    static std::string sPrefix = "MapPlugin/Plot/";

    std::string sFilePath = GetExePath();
    sFilePath += sPrefix + "NodeFactory.dll";
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

    m_nTimerID = startTimer(60000);
}

/// 插入场景节点
void CMapNodeFactory::InsertNode(ISceneNode *pNode)
{
    CanDeleteNode tmpDelete;
    tmpDelete.pSceneNode = pNode;
    m_allCreateNode.push_back(tmpDelete);
}

///根据模型ID获取模型
ISceneNode *CMapNodeFactory::FindNodeByID(unsigned int unPickID)
{
    /// 遍历所有创建的节点
    for(const CanDeleteNode& one : m_allCreateNode)
    {
        if(one.pSceneNode->CanPick())
        {
            if(unPickID == one.pSceneNode->PickID())
            {
                return(one.pSceneNode);
            }
        }
    }

    return(nullptr);
}

/// 初始化类型
void CMapNodeFactory::InitType(const std::string &sInterface)
{
    auto findOne = m_mapTypeDllName.find(sInterface);
    if (m_mapTypeDllName.end() != findOne)
    {
        std::string sInterfaceNameList;

        auto pQueryFunc = reinterpret_cast<pQueryInterfaceFun>(QLibrary::resolve(findOne->second.c_str(),"QueryInterface"));
        auto pCreateFun = reinterpret_cast<pCreateNodeFun>(QLibrary::resolve(findOne->second.c_str(),"CreateNode"));

        if (nullptr != pCreateFun && nullptr != pQueryFunc)
        {
            if(pQueryFunc(sInterfaceNameList))
            {
                std::istringstream is(sInterfaceNameList);
                std::string sType;
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
        else
        {
            osg::notify(osg::WARN)<<"加载"<<findOne->second<<" 失败"<<std::endl;
        }
    }
}

#include <QTimerEvent>
void CMapNodeFactory::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_nTimerID)
    {
        DeleteNoUseSceneNode();
        m_pSceneGraph->ResouceLoader()->ClearNoUse();
    }
}
