#ifndef INTERFACE_VIEW_PORT_HEARDER_H
#define INTERFACE_VIEW_PORT_HEARDER_H
#include <SceneGraph/ViewType.h>
class IViewHud;
class ISceneNode;

/**
 * @brief 视口类
 */
class IViewPort
{
public:
    /**
     * @brief 设置跟踪的节点
     */
    virtual bool SetTrackNode(ISceneNode*)=0;

    /**
     * @brief 获取跟踪的节点
     * @return
     */
    virtual ISceneNode* GetTrackNode()=0;

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    virtual IViewHud* GetHud()=0;

    /**
     * @brief 设置视点位置
     */
    virtual void SetViewPoint(const SceneViewPoint&)=0;

    /**
     * @brief 获取视点位置
     * @return
     */
    virtual const SceneViewPoint& GetViewPoint()=0;

    /**
     * @brief 设置视口大小
     */
    virtual void SetViewPort(const CameraViewPort&)=0;

    /**
     * @brief 获取视口大小
     * @return 视口大小
     */
    virtual const CameraViewPort& GetViewPort()=0;

    /**
     * @brief 设置投影类型
     */
    virtual void SetProjectType(ProjectType)=0;

    /**
     * @brief 获取投影类型
     * @return 投影类型
     */
    virtual ProjectType GetProjectType()=0;
protected:
    virtual ~IViewPort(){}
};

#endif
