#include <algorithm>
#include <SceneGraph/ISceneGroup.h>
#include "SceneGraphManager.h"
#include "QtSceneGraph.h"

CSceneGraphManager::CSceneGraphManager()
{
}

/// 释放所有的
CSceneGraphManager::~CSceneGraphManager()
{
    for(auto one : m_vSceneGraph)
    {
        delete one;
    }
}

/// 初始化
ISceneGraph *CSceneGraphManager::CreateSceneGraph(ISceneGraph::SceneGraphType typeScene)
{
    auto pSceneGraph = new QtSceneGraph(typeScene);
    pSceneGraph->InitSceneGraph();
    m_vSceneGraph.push_back(pSceneGraph);

    return(pSceneGraph);
}

/// 获取绑定的场景图
ISceneGraph *CSceneGraphManager::FindSceneGraph(IWindow *pWindow)
{
    for(auto one : m_vSceneGraph)
    {
        one->ContainWindow(pWindow);
        return(one);
    }

    return(nullptr);
}

ISceneGraph *CSceneGraphManager::FindSceneGraph(QQuickItem *pItem)
{
    for(auto one : m_vSceneGraph)
    {
        if(one->IsQuickItem(pItem))
        {
            return(one);
        }
    }

    return(nullptr);
}

/// 删除场景
bool CSceneGraphManager::DeleteSceneGraph(ISceneGraph *& rForDelete)
{
    auto itor = find(m_vSceneGraph.begin(),m_vSceneGraph.end(),rForDelete);

    if(itor != m_vSceneGraph.end())
    {
        delete dynamic_cast<QtSceneGraph*>(rForDelete);
        rForDelete = nullptr;
        m_vSceneGraph.erase(itor);
        return(true);
    }

    return(false);
}
