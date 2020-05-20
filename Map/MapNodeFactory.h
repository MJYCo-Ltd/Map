#ifndef CMAPNODEFACTORY_H
#define CMAPNODEFACTORY_H

#include <list>
#include <map>

class IMapSceneNode;
class ISceneGraph;

using namespace std;

class CMapNodeFactory
{
public:
    CMapNodeFactory(ISceneGraph* pSceneGraph);
    ~CMapNodeFactory();

    /**
     * @brief 根据类型创建节点
     * @param unType
     * @return
     */
    IMapSceneNode* CreateMapSceneNode(unsigned int unType);

    /**
     * @brief 删除节点
     * @param pMapSceneNode
     * @return
     */
    bool DeleteMapSceneNode(IMapSceneNode *pMapSceneNode);

protected:
    /**
     * @brief 初始化工厂
     */
    void InitFactory();

	/**
	 * @brief 初始化类型
	 */
	void InitType(unsigned int unType);
private:
    ISceneGraph* m_pSceneGraph;

    typedef IMapSceneNode* (*pCreateNodeFun)(ISceneGraph*);
    map<unsigned int,pCreateNodeFun> m_mapTypeFunc;
	map<unsigned int, string>        m_mapTypeDllName; /// 类型和dll的
    list<IMapSceneNode*> m_allCreateNode;
};

#endif // CMAPNODEFACTORY_H
