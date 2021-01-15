#ifndef INTERFACE_PLOT_HEADER_H
#define INTERFACE_PLOT_HEADER_H

#include <string>
#include <SceneGraph/ISceneGroup.h>

enum SCENE_GROUP_TYPE
{
    ATTITUDE_GROUP, /// 调整位置、姿态
    FLASH_GROUP,    /// 闪烁
    LOD_GROUP,      /// 分级显示
    SCALE_GROUP,    /// 自动缩放
    STANDARD_GROUP  /// 只是一个容器
};

/**
 * @brief 标绘管理类
 */
class IPlot
{
    friend class ISceneGraph;
public:

    CONSTRUCTOR(IPlot,m_pSceneGraph)

    /**
     * @brief 注册标绘类型
     */
    virtual void RegisterPlotType()=0;

    /**
     * @brief
     * @return
     */
    virtual ISceneGroup* CreateSceneGroup(SCENE_GROUP_TYPE)=0;

    /**
     * @brief 根据类型创建节点
     * @return
     */
    virtual ISceneNode* CreateSceneNode(const std::string&)=0;

    /**
     * @brief 根据路径加载模型
     * @return
     */
    virtual ISceneNode* LoadSceneNode(const std::string&)=0;

    /**
     * @brief 强制删除不再使用的地图节点
     */
    virtual void FreeNoUseSceneNode()=0;
protected:
    virtual ~IPlot(){}
protected:
    ISceneGraph* m_pSceneGraph;
};

#endif//INTERFACE_PLOT_HEADER_H
