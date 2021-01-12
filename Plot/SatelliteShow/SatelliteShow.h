#ifndef CSATELLITESHOW_H
#define CSATELLITESHOW_H

#include <NoQt.h>
/**
 * @brief 显示卫星的类
 */

#include <Plot/SatelliteShow/ISatellite.h>

#include <Inner/ImplSceneGroup.hpp>

class ILine;

class CSatelliteShow:public ImplSceneGroup<ISatellite>
{
public:
    CONSTRUCTOR(CSatelliteShow,ImplSceneGroup<ISatellite>)

    /**
     * @brief 初始化节点
     */
    void InitNode();

    /**
     * @brief 设置轨道数据
     */
    void SetJ2000Oribit(const std::vector<double> &, const std::vector<Math::CVector> &);

    /**
     * @brief 更新轨道数据
     */
    void SetECFOribit(const std::vector<Math::CVector>&);

    /**
     * @brief 添加传感器
     * @param pSensor
     */
    void AddSensor(ISensor*pSensor);

    /// <summary>
    /// 获取卫星位置
    /// </summary>
    /// <returns></returns>
    ScenePos GetSatellitePos();

    /// <summary>
    /// 获取卫星初始姿态
    /// </summary>
    /// <returns></returns>
    Math::CMatrix GetSatelliteInitAtt();
protected:
    void ModelChanged();
    void NameChanged();
    void NowTimeChanged();
    void OribitColorChanged();
    /**
     * @brief 根据时间计算卫星轨道数据
     */
    double CalItNewton(double *dX, double dT, int nDim);
    double CalItNewtonEcf(double*,double,int);

protected:
    std::set<ISensor*>              m_vSensor;
    ISceneAttitudeGroup*       m_pSatellite=nullptr;
    ISceneModel*               m_pModel=nullptr;
    ILine*                     m_pOribit=nullptr;
    std::vector<Math::CVector> m_vOribit;    /// J2000坐标系下的数据
    std::vector<Math::CVector> m_vEcfOribit; /// 地固系下的轨道数据
    std::vector<double>        m_vdMjd;      /// 坐标对应的时间
    Math::CVector              m_stNowPos; /// 当前卫星的位置(j2000)
    ScenePos                   m_satelliteWgs84Pos;  // 当前卫星的位置(wgs84)
    Math::CMatrix              m_satelliteInitAtt;      //卫星初始姿态


    double                     m_dStart = 0.0;
    double                     m_dEnd = 0.0;
    double                     m_dStep = 0.0;
    int                        m_nIndex = -1;
};

extern "C"
{
    Q_DECL_EXPORT ISatellite* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // CSATELLITESHOW_H
