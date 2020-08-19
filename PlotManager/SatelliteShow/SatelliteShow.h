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
class COribit;

class CSatelliteShow:public QtOsgEarthMapSceneNode<ISatellite>
{
    friend class OribitUpdateCallBack;
public:
    CSatelliteShow(ISceneGraph*);

    /**
     * @brief 初始化节点
     */
    void InitSceneNode();

    /**
     * @brief 设置名称
     */
    void SetName(const string&);

    /**
     * @brief 设置卫星模型路径
     */
    void SetModelPath(const string&);

    /**
     * @brief 设置轨道颜色
     */
    void SetOribitColor(const SceneColor&);

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

    /**
     * @brief 更新时间
     */
    void UpdateData(double);
protected:
    /**
     * @brief 构建显示名字
     */
    void BuildName();

    double CalItNewton(double *dX, double dT, int nDim);
protected:
    string m_sName;
    string m_sModelPath;
    std::vector<Math::CVector> m_vOribit;   /// teme坐标系下的数据
    std::vector<double> m_vdMjd;               /// 坐标对应的时间
    osg::ref_ptr<osg::Node>    m_pSatellite; ///卫星模型
    COribit*            m_pOribit=nullptr;
    Math::CVector                           m_stNowPos; /// 当前卫星的位置


    double                       m_dStart = 0.0;
    double                       m_dEnd = 0.0;
    double                       m_dStep = 0.0;
    int  m_nIndex = -1;

    static string S_sInterFace;
};

extern "C"
{
    PERSONINFOSHARED_EXPORT ISatellite* CreateNode(ISceneGraph*pSceneGraph,const string& sInterfaceName);
    PERSONINFOSHARED_EXPORT bool QueryInterface(string& sInterfaceName);
}
#endif // CSATELLITESHOW_H
