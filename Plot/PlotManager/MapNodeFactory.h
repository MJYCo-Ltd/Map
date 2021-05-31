#ifndef CMAPNODEFACTORY_H
#define CMAPNODEFACTORY_H
#include <QObject>
#include <list>
#include <map>

class ISceneNode;
class ISceneGraph;

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
    ISceneNode* CreateSceneNode(const std::string &sInterface);

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

    /**
     * @brief 根据模型ID获取模型
     * @param unPickID
     * @return
     */
    ISceneNode* FindNodeByID(unsigned int unPickID);

protected:

	/**
	 * @brief 初始化类型
	 */
    void InitType(const std::string& sInterface);

    /**
     * @brief 定时处理
     */
    void timerEvent(QTimerEvent *);
private:
    int   m_nTimerID{-1};
    ISceneGraph* m_pSceneGraph{};

    typedef ISceneNode* (*pCreateNodeFun)(ISceneGraph*,const std::string&);
    typedef bool (*pDeleteNodeFun)(ISceneNode*);
    typedef bool(*pQueryInterfaceFun)(std::string&);

    struct MapSceneFun
    {
        pCreateNodeFun pCrete{};
        pDeleteNodeFun pDelete{};
    };

    std::map<std::string,MapSceneFun> m_mapTypeFunc;

    std::map<std::string, std::string> m_mapTypeDllName; /// 类型和dll的
    std::list<ISceneNode*> m_allCreateNode;
};

#endif // CMAPNODEFACTORY_H
