#ifndef CSATELLITESHOW_H
#define CSATELLITESHOW_H

/**
 * @brief 显示卫星的类
 */

#include <Map/Plot/ISatellite.h>
#include <Inner/QtOsgEarthMapSceneNode.h>
#include "SatelliteShow_Global.h"

class CSatellite2D;
class CSatellite3D;

class CSatelliteShow:public QtOsgEarthMapSceneNode<ISatellite>
{
public:
    CSatelliteShow(ISceneGraph*);

    /**
     * @brief 地图更新
     */
    void UpdateMapNode(osgEarth::MapNode*);

    /**
     * @brief 设置轨道数据
     */
    void SetJ2000Oribit(const vector<double> &, const vector<Math::CVector> &);

    /**
     * @brief 更新轨道数据
     */
    void SetGeoOribit(const vector<Math::CVector>&);

    /**
     * @brief 获取接口名称
     * @return
     */
    static const string& GetInterFaceName(){return(S_sInterFace);}
protected:

    osg::ref_ptr<CSatellite2D> m_p2D; /// 卫星的二维地图绘制类
    osg::ref_ptr<CSatellite3D> m_p3D; /// 卫星的三维地图绘制类
    static string S_sInterFace;
};

extern "C"
{
    PERSONINFOSHARED_EXPORT ISatellite* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    PERSONINFOSHARED_EXPORT bool QueryInterface(string& sInterfaceName);
}
#endif // CSATELLITESHOW_H
