#ifndef CPLOTMANAGER_H
#define CPLOTMANAGER_H
#include<map>
using namespace std;

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


    ISceneGroup* CreateSceneGroup(SCENE_GROUP_TYPE emType);
    /**
     * @brief 根据接口类型创建节点
     * @param csInterfaceName
     * @return
     */
    ISceneNode * CreateSceneNode(const string& csInterfaceName);

    /**
     * @brief LoadSceneNode
     * @param sPath
     * @return
     */
    ISceneNode* LoadSceneNode(const std::string& sPath);

    /**
     * @brief FreeNoUseMapSceneNode
     */
    void FreeNoUseSceneNode();

    /**
     * @brief
     */
    void RegisterPlotType();
private:
    CMapNodeFactory*                   m_pCreateFactory=nullptr;/// 创建节点
};

extern "C"
{
    Q_DECL_EXPORT IPlot* CreatePlot(ISceneGraph* pSceneGraph);
};
#endif // CPLOTMANAGER_H