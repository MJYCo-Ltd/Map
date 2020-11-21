#ifndef CMAPNODEFACTORY_H
#define CMAPNODEFACTORY_H
#include <QObject>
#include <list>
#include <map>

class ISceneNode;
class ISceneGraph;

using namespace std;

class CMapNodeFactory:public QObject
{
    Q_OBJECT
public:
    CMapNodeFactory(ISceneGraph* pSceneGraph);
    ~CMapNodeFactory();

    /**
     * @brief 根据类型创建节点
     * @param 接口名称
     * @return
     */
    ISceneNode* CreateMapSceneNode(const string &sInterface);

    /**
     * @brief 删除不再使用的地图节点
     */
    void DeleteNoUseSceneNode();

    /**
     * @brief 初始化工厂
     */
    void InitFactory();

    /**
     * @brief 添加场景节点
     * @param pNode
     */
    void InsertNode(ISceneNode* pNode);

protected:

	/**
	 * @brief 初始化类型
	 */
    void InitType(const string& sInterface);

    /**
     * @brief 定时处理
     */
    void timerEvent(QTimerEvent *);
private:
    int   m_nTimerID=-1;
    ISceneGraph* m_pSceneGraph=nullptr;

    typedef ISceneNode* (*pCreateNodeFun)(ISceneGraph*,const string&);
    typedef bool (*pDeleteNodeFun)(ISceneNode*);
    typedef bool(*pQueryInterfaceFun)(string&);

    struct MapSceneFun
    {
        pCreateNodeFun pCrete=nullptr;
        pDeleteNodeFun pDelete=nullptr;
    };

    map<string,MapSceneFun> m_mapTypeFunc;
    map<ISceneNode*,string>m_mapDeleteFunc;

    map<string, string>        m_mapTypeDllName; /// 类型和dll的
    list<ISceneNode*> m_allCreateNode;
};

#endif // CMAPNODEFACTORY_H
