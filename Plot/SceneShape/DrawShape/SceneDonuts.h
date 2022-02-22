#ifndef SCENEDONUTS_H
#define SCENEDONUTS_H

#include <Inner/ImplSceneSensor.hpp>
#include <Plot/SceneShape/ISceneDonut.h>

class CSnGeoMathCommon
{
public:

    static CSnGeoMathCommon* GetInstance();
    /**
     * @brief 获取sin值
     * @return
     */
    const double* GetSin();

    /**
     * @brief 获取cos值
     * @return
     */
    const double* GetCos();

    bool CaLineInterEllipse(const osg::Vec3d& vPt,const osg::Vec3d& vDir,osg::Vec3d& vInterPt );
private:
    CSnGeoMathCommon();
    ~CSnGeoMathCommon();

    /**
     * @brief 初始化数据
     */
    void Init();

private:
    double* m_pSin;    /// sin值
    double* m_pCos;  /// cos值
    bool       m_bInit;    /// 是否初始化

    double   m_dA;
    double   m_dB;
};
/**
 * @brief 本类的主要作用是将设定的网格
 *             化成 TRIANGLE_STRIP 的顶点索引
 */
class CGrid2TriangleStrip
{
public:
    /**
     * @brief 默认构造函数
     * @param unRow   行数 (左右 方向为行)
     * @param unLine    列数 (上下 方向为列)
     * @param bCloseure 行的最后是否闭合
     */
    CGrid2TriangleStrip(unsigned int unRow=0, unsigned int unLine=0,bool bCloseure=false);

    /**
     * @brief 设置行的收尾是否闭合
     * @param bClosure
     */
    void SetClosure(bool bClosure);
    bool GetClosure(){return(m_bClosure);}

    /**
     * @brief 设置行列
     * @param nRow   行数
     * @param nLine   列数
     */
    void SetRowAndLine(unsigned int unRow, unsigned int unLine);

    /**
     * @brief 获取索引数组
     * @return
     */
    const osg::MixinVector<osg::VectorGLuint>& GetIndex(){return(m_vIndex);}

private:
    /**
     * @brief 生成索引
     */
    void GenerateIndex();
private:
    unsigned int m_unRowCount;                  /// 行数
    unsigned int m_unLineCount;                  /// 列数
    bool              m_bClosure;                        /// 设置收尾是否闭合
    osg::MixinVector<osg::VectorGLuint> m_vIndex;  /// 所有的顶点索引的集合
};



class CSceneDonuts:public ImplSceneSensor<ISceneDonut>
{
public:
    CONSTRUCTOR(CSceneDonuts,ImplSceneSensor<ISceneDonut>)
protected:
    void CreateShape();
    void UpdateShape();
private:
    bool    m_bBuild;/// 是否构建完成
    osg::Vec4 m_vColor={1.0,0,0,1.0};
};

#endif // SCENEDONUTS_H
