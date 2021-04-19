#ifndef INTERFACE_ELLIPSOID_SENSOR_HEARDER_H
#define INTERFACE_ELLIPSOID_SENSOR_HEARDER_H
#include <string>
#include <Plot/SceneShape/ISensor.h>

/**
 * @brief 场景图片类
 */
class IEllipsoidSensor:public ISensor
{
public:
    enum DRAW_TYPE
    {
        FULL_PART, /// 整个椭球
        SUB_PART,  /// 一部分椭球
        SCONE_PART,/// 椭球的方锥部分
        CONE_PART  /// 椭球的圆锥部分
    };
    CONSTRUCTOR(IEllipsoidSensor,ISensor)

    /**
     * @brief 设置赤道半径 [m]
     */
    void SetEquator(double dEquator)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dEquator,m_dEquator,ShapeChanged)
    double Equator()const{return(m_dEquator);}

    /**
     * @brief 设置极半径 [m]
     */
    void SetPolar(double dPolar)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dPolar,m_dPolar,ShapeChanged)
    double Polar()const{return(m_dPolar);}

    /**
     * @brief 设置椭球的纹理
     */
    void SetImage(const std::string& sImage)JUDGE_EQUAL_CALL_FUNCTION(sImage,m_sImagePath,ShapeChanged)
    const std::string& Image()const{return(m_sImagePath);}

    /**
     * @brief 设置绘制类型
     */
    void SetDrawType(DRAW_TYPE type)JUDGE_EQUAL_CALL_FUNCTION(type,m_emDrawType,ShapeChanged)
    DRAW_TYPE DrawType()const{return(m_emDrawType);}

    /**
     * @brief 设置水平探测范围[deg][-180,180]
     */
    void SetAzimuth(double dMin,double dMax)
    {
        if((dMax-dMin)>0
                && dMin>=-180
                && dMax<=180
                && (fabs(dMin-m_dMinAzim)>DBL_EPSILON || fabs(dMax-m_dMaxAzim)>DBL_EPSILON))
        {
            m_dMinAzim=dMin;
            m_dMaxAzim=dMax;
            ShapeChanged();
        }
    }
    double MinAzimuth()const{return(m_dMinAzim);}
    double MaxAzimuth()const{return(m_dMaxAzim);}

    /**
     * @brief 设置垂直探测范围 [deg][-90,90]
     */
    void SetElevation(double dMin,double dMax)
    {
        if((dMax-dMin)>0
                && dMin>=-90
                && dMax<=90
                && (fabs(dMin-m_dMinElev)>DBL_EPSILON || fabs(dMax-m_dMaxElev)>DBL_EPSILON))
        {
            m_dMinElev=dMin;
            m_dMaxElev=dMax;
            ShapeChanged();
        }
    }
    double MinElevation()const{return(m_dMinAzim);}
    double MaxElevation()const{return(m_dMaxAzim);}

    /**
     * @brief 设置圆锥半角[deg][0~90]
     */
    void SetConeAngle(double dAngle)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dAngle,m_dAngle,ShapeChanged)
    double ConeAngle()const{return(m_dAngle);}

    /**
     * @brief 设置方锥半角[deg][0~90]
     */
    void SetSConeHAngle(double dHAngle)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dHAngle,m_dHAngle,ShapeChanged)
    double SConeHAngle()const{return(m_dHAngle);}

    /**
     * @brief 设置方锥半角[deg][0~90]
     */
    void SetSConeVAngle(double dVAngle)JUDGE_POSITIVE_DOUBLE_CALL_FUNCTION(dVAngle,m_dVAngle,ShapeChanged)
    double SConeVAngle()const{return(m_dVAngle);}

    /**
     * @brief 设置椭球纬度上划分的个数
     * @attention 经度上为纬度上划分的个数的2倍
     */
    void SetLatSegMents(unsigned int nSgements)JUDGE_EQUAL_CALL_FUNCTION(nSgements,m_unSegments,ShapeChanged)
    unsigned int LatSegmentsconst()const{return(m_unSegments);}

protected:
    virtual ~IEllipsoidSensor(){}
protected:
    DRAW_TYPE m_emDrawType=FULL_PART;
    unsigned int m_unSegments=18;
    double m_dEquator=1.; /// 赤道半径
    double m_dPolar=1.;   /// 极半径
    double m_dMinAzim=-1.;
    double m_dMaxAzim=1.;
    double m_dMinElev=-1.0;
    double m_dMaxElev=1.;
    double m_dHAngle=1.;
    double m_dVAngle=1.;
    double m_dAngle=1.;
    std::string m_sImagePath;
};

#endif//INTERFACE_ELLIPSOID_SENSOR_HEARDER_H
