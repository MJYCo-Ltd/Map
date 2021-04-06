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
        View_2D,  /// 二维查看
        View_3D,  /// 三维查看
        View_User,/// 用户查看
        View_Node /// 跟踪节点
    };

    virtual ~IOsgViewPoint(){}

    /**
     * @brief 地图类型切换
     * @param emType
     */
    virtual void ViewPointTypeChanged(ViewPointType emType){}
	
    /**
     * @brief 获取osgView
     * @return
     */
    virtual osgViewer::View* GetOsgView()=0;
};
#endif
