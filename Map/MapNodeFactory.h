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
     * @param 接口名称
     * @return
     */
    IMapSceneNode* CreateMapSceneNode(const string &sInterface);

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
    void InitType(const string& sInterface);
private:
    ISceneGraph* m_pSceneGraph=nullptr;

    typedef IMapSceneNode* (*pCreateNodeFun)(ISceneGraph*,const string&);
    typedef bool(*pQueryInterfaceFun)(string&);

    map<string,pCreateNodeFun> m_mapTypeFunc;

    map<string, string>        m_mapTypeDllName; /// 类型和dll的
    list<IMapSceneNode*> m_allCreateNode;
};

#endif // CMAPNODEFACTORY_H
