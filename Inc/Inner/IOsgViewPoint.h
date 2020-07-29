#ifndef INTERFACE_OSG_VIEW_Point_H
#define INTERFACE_OSG_VIEW_Point_H

namespace osgViewer
{
    class View;
}

class IOsgViewPoint
{
public:
    virtual ~IOsgViewPoint(){}
	
    /**
     * @brief 获取osgView
     * @return
     */
    virtual osgViewer::View* GetOsgView()=0;
};
#endif
