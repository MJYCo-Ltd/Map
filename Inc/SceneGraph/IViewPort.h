#ifndef INTERFACE_VIEW_PORT_HEARDER_H
#define INTERFACE_VIEW_PORT_HEARDER_H
#include "ISceneNode.h"

const int C_WINDOW_HEIGHT(100);
const int C_WINDOW_WIDTH(100);

/// 视点位置
struct SceneViewPoint
{
    ScenePos stPos;
    float fAzimuth=0;
    float fElev=0;

    bool operator == (const SceneViewPoint& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(this->stPos == rOther.stPos &&
           fabs(rOther.fElev - fElev) < 1e-6 &&
           fabs(rOther.fAzimuth - fAzimuth) < 1e-6)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    bool operator !=(const SceneViewPoint& rOther) const
    {
        return(!(this->operator==(rOther)));
    }
};

/// 视口位置
struct CameraViewPort
{
    int nX=0;      /// 视口在屏幕X轴的位置
    int nY=0;      /// 视口在屏幕Y轴的位置
    int nWidth=0;  /// 视口的宽度
    int nHeight=0; /// 视口的高度

    /// 判断两个是否相等
    bool operator == (const CameraViewPort& rOther) const
    {
        if(&rOther == this)
        {
            return(true);
        }

        if(rOther.nX == nX &&
           rOther.nY == nY &&
           rOther.nWidth == nWidth &&
           rOther.nHeight == nHeight)
        {
            return(true);
        }
        else
        {
            return(false);
        }
    }

    /// 判断两者是否不相等
    bool operator != (const CameraViewPort& rOther) const
    {
        return(!this->operator==(rOther));
    }
};

/// 视口类型
enum ProjectType
{
    Perspective, /// 透视投影
    Ortho        /// 正视投影
};

/**
 * @brief 视口类
 */
class IViewPort
{
public:
    virtual ~IViewPort(){}

    /**
     * @brief 设置跟踪的节点
     */
    virtual void SetTrackNode(ISceneNode*)=0;

    /**
     * @brief 获取跟踪的节点
     * @return
     */
    virtual ISceneNode* GetTrackNode()=0;

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
};

#endif
