#include <fstream>
#include <algorithm>

#include <QLibrary>

#include <SceneGraph/ISceneGraph.h>
#include <ISceneCore.h>
#include <Map/IMapSceneNode.h>
#include <Inner/OsgExtern/OsgExtern.h>
#include <Inner/IRender.h>
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
        auto pGroup = one->GetOsgNode()->asGroup();
        if(nullptr != pGroup)
        {
            m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new RemoveFromeScene(pGroup));
        }
    }

    m_allCreateNode.clear();
}

/// 创建一个节点
IMapSceneNode *CMapNodeFactory::CreateMapSceneNode(unsigned int unType)
{
    auto findOne = m_mapTypeFunc.find(unType);

	if (m_mapTypeFunc.end() == findOne)
	{
		InitType(unType);
	}

	findOne = m_mapTypeFunc.find(unType);
    if(m_mapTypeFunc.end() != findOne)
    {
        IMapSceneNode* pNode = findOne->second(m_pSceneGraph);
        if(nullptr != pNode)
        {
            pNode->InitSceneNode();
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
#ifdef _DEBUG
    sFilePath += "\\NodeFactoryd.dll";
#else
    sFilePath += "\\NodeFactory.dll";
#endif
    ifstream in(sFilePath, ios::in);
    unsigned int uType;
    string sDllName;
    if(in.is_open())
    {
        while(!in.eof())
        {
            in>>uType>>sDllName;
			m_mapTypeDllName[uType] = sDllName;
        }
    }
}

/// 初始化类型
void CMapNodeFactory::InitType(unsigned int unType)
{
	QLibrary loadDll;
	auto findOne = m_mapTypeDllName.find(unType);
	if (m_mapTypeDllName.end() != findOne)
	{
		loadDll.setFileName(findOne->second.c_str());
		auto pFunc = reinterpret_cast<pCreateNodeFun>(loadDll.resolve("CreateNode"));
		if (nullptr != pFunc)
		{
			m_mapTypeFunc[unType] = pFunc;
		}
	}
}
