#ifndef CPLOTMANAGER_H
#define CPLOTMANAGER_H
#include<map>

#include <osg/ref_ptr>
#include <osg/observer_ptr>
#include <NoQt.h>
#include <Plot/IPlot.h>

class CPlotLayer;
class ISceneGraph;
class CMapNodeFactory;


class CPlotManager:public IPlot
{
    friend class ChangeMapNode;

public:
    CPlotManager(ISceneGraph* pSceneGraph);
    ~CPlotManager();


    /**
     * @brief 根据不同的类型创建组节点
     */
    ISceneGroup* CreateSceneGroup(SCENE_GROUP_TYPE emType);

    /**
     * @brief 根据接口类型创建节点
     * @param csInterfaceName
     * @return
     */
    ISceneNode * CreateSceneNode(const std::string& csInterfaceName);

    /**
     * @brief LoadSceneNode
     * @param sPath
     * @return
     */
    ISceneNode* LoadSceneNode(const std::string& sPath,bool bRef);

    /**
     * @brief 根据模型ID获取模型
     * @param unPickID
     * @return
     */
    ISceneNode* FindNodeByID(unsigned int unPickID);

    /**
     * @brief 强制删除不再使用的地图节点
     */
    void FreeNoUseSceneNode();

    /**
     * @brief
     */
    void RegisterPlotType();
private:
    CMapNodeFactory*     m_pCreateFactory{};/// 创建节点
};

extern "C"
{
    Q_DECL_EXPORT IPlot* CreatePlot(ISceneGraph* pSceneGraph);
};
#endif // CPLOTMANAGER_H
