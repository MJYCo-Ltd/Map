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
    /// 立体模式
    enum StereoType
    {
        QUAD_BUFFER,
        ANAGLYPHIC,
        HORIZONTAL_SPLIT,
        VERTICAL_SPLIT,
        LEFT_EYE,
        RIGHT_EYE,
        HORIZONTAL_INTERLACE,
        VERTICAL_INTERLACE,
        CHECKERBOARD
    };

    /**
     * @brief 是否开启立体
     */
    virtual void OpenStereo(bool)=0;

    /**
     * @brief 设置立体模式
     * @param type
     */
    virtual void SetStereoMode(StereoType)=0;

    /**
     * @brief 设置跟踪的节点
     */
    virtual bool SetTrackNode(ISceneNode*)=0;
    virtual ISceneNode* GetTrackNode()const=0;

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    virtual IViewHud* GetHud()=0;

    /**
     * @brief 设置视点位置
     */
    virtual void SetViewPoint(const SceneViewPoint&)=0;
    virtual const SceneViewPoint& GetViewPoint()const=0;

    /**
     * @brief 设置视口大小
     */
    virtual void SetViewPort(const CameraViewPort&)=0;
    virtual const CameraViewPort& GetViewPort()const=0;

    /**
     * @brief 设置投影类型
     */
    virtual void SetProjectType(ProjectType)=0;
    virtual ProjectType GetProjectType()const=0;
protected:
    virtual ~IViewPort(){}
};

#endif
