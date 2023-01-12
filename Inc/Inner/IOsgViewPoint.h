#ifndef INTERFACE_OSG_VIEW_Point_H
#define INTERFACE_OSG_VIEW_Point_H

#include <osgViewer/View>
#include <Plot/Map/MapType.h>
class IOsgViewPoint
{
public:
    /// 设置视点状态
    enum ViewPointType
    {
        View_2DMap,  /// 二维查看
        View_3DMap,  /// 三维查看
        View_Osg,    /// 不使用地图的时候的操作器
        View_User,   /// 用户查看
        View_Node    /// 跟踪节点
    };

    virtual ~IOsgViewPoint(){}

    /**
     * @brief 地图类型切换
     * @param emType
     */
    virtual void ViewPointTypeChanged(ViewPointType emType){}

    /**
     * @brief 锁定视角
     */
    virtual void SetLockView(bool){};
	
    /**
     * @brief 获取osgView
     * @return
     */
    virtual osgViewer::View* GetOsgView()=0;
};
#endif
