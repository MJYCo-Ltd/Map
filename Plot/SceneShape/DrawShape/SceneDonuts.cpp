#include "SceneDonuts.h"
#include <VersionMathCommon.h>

CSnGeoMathCommon *CSnGeoMathCommon::GetInstance()
{
    static CSnGeoMathCommon s_mathCommon;

    /// 如果没有初始化 则初始化
    if(!s_mathCommon.m_bInit)
    {
        s_mathCommon.Init();
    }

    return(&s_mathCommon);
}

/// 获取sin值
const double *CSnGeoMathCommon::GetSin()
{
    return(m_pSin);
}

/// 获取cos值
const double* CSnGeoMathCommon::GetCos()
{
    return(m_pCos);
}

/// 计算椭球交点
bool CSnGeoMathCommon::CaLineInterEllipse(const osg::Vec3d& vPt,const osg::Vec3d& vDir,osg::Vec3d& vInterPt)
{
    osg::Vec3d vec = vDir;
    vec.normalize();

    static double a2 = m_dA * m_dA;
    static double b2 = m_dB * m_dB;
    double dx = vec.x();
    double dy = vec.y();
    double dz = vec.z();
    double ex = vPt.x();
    double ey = vPt.y();
    double ez = vPt.z();

    /// 构造一元二次方程的参数
    double A = b2 * dx * dx + b2 * dy * dy + a2 * dz * dz;
    double B = 2 * (b2 * dx * ex + b2 * dy * ey + a2 * dz * ez);
    double C = b2 * ex * ex + b2 * ey * ey + a2 * ez * ez - a2 * b2;

    /// 解一元二次方程
    double delta = B * B - 4 * A * C;
    if(delta < 0)
        return false;

    double deltaS = sqrt(delta);
    double t1 = (-B + deltaS)/(2 * A);
    double t2 = (-B - deltaS)/(2 * A);

    if(t1 < 0 && t2 < 0)
        return false;

    /// 取离vPt近的位置
    double t = fabs(t1) < fabs(t2) ? t1:t2;

    /// 根据直线方程计算交点位置
    vInterPt.x() = t * dx + ex;
    vInterPt.y() = t * dy + ey;
    vInterPt.z() = t * dz + ez;
    return true;

}

CSnGeoMathCommon::CSnGeoMathCommon():
    m_bInit(false)
{
    m_pSin = new double[360];
    m_pCos = new double[360];
}

/// 析构数据空间
CSnGeoMathCommon::~CSnGeoMathCommon()
{
    delete []m_pSin;
    delete []m_pCos;
}

/// 初始化
void CSnGeoMathCommon::Init()
{
    for(int i=0; i<360; ++i)
    {
        m_pSin[i] = sin(i*DD2R);
        m_pCos[i] = cos(i*DD2R);
    }

    m_dA = osg::WGS_84_RADIUS_EQUATOR;
    m_dB = osg::WGS_84_RADIUS_POLAR;
    m_bInit = true;
}

CGrid2TriangleStrip::CGrid2TriangleStrip(unsigned int unRow, unsigned int unLine, bool bCloseure):
    m_unRowCount(unRow),
    m_unLineCount(unLine),
    m_bClosure(bCloseure)
{
    GenerateIndex();
}

/// 设置行数和列数
void CGrid2TriangleStrip::SetRowAndLine(unsigned int unRow, unsigned int unLine)
{
    /// 如果行列数相同则退出
    if(unRow == m_unRowCount && unLine == m_unLineCount)
    {
        return;
    }

    m_unRowCount = unRow;
    m_unLineCount = unLine;

    /// 生成索引数组
    GenerateIndex();
}

/// 设置行尾部是否闭合
void CGrid2TriangleStrip::SetClosure(bool bClosure)
{
    /// 如果相同则退出
    if(bClosure == m_bClosure)
    {
        return;
    }

    m_bClosure = bClosure;

    /// 如果状态改该则重新生成索引
    GenerateIndex();
}

#include <iostream>
/// 生成顶点索引数组
void CGrid2TriangleStrip::GenerateIndex()
{
    m_vIndex.clear();

    /// 根据收尾是否相接进行数据的产生
    osg::VectorGLuint tmpVIndex;
    if(m_bClosure)
    {
        tmpVIndex.resize(2*(m_unLineCount+1));
    }
    else
    {
        tmpVIndex.resize(2*m_unLineCount);
    }

    int nBase,nIndexBase;

    /// 开始构建索引数组
    for(int nIndexRow = 0; nIndexRow < m_unRowCount; ++nIndexRow)
    {
        if(m_unRowCount -1 != nIndexRow)
        {
            /// 如果闭合
            if(m_bClosure)
            {
                nIndexBase = 2*nIndexRow * (m_unLineCount + 1);
            }
            else
            {
                nIndexBase = 2*nIndexRow * m_unLineCount;
            }

            nBase = (m_unRowCount - 1 - nIndexRow) * m_unLineCount;
            /// 循环生成
            for(int nIndexLine = 0; nIndexLine < m_unLineCount; ++nIndexLine)
            {
                tmpVIndex[nIndexLine*2] = nBase + nIndexLine;
                tmpVIndex[1 + nIndexLine*2] = nBase + nIndexLine - m_unLineCount;
            }

            /// 如果闭合则多增加两个
            if(m_bClosure)
            {
                tmpVIndex[m_unLineCount*2] = nBase;
                tmpVIndex[m_unLineCount*2 + 1] = nBase - m_unLineCount;
            }

            m_vIndex.push_back(tmpVIndex);
        }
    }

}


void CSceneDonuts::CreateShape()
{
    m_pVertexArray->resizeArray(25*25);
    UpdateShape();
    CGrid2TriangleStrip grid(25,25,true);
    const osg::MixinVector<osg::VectorGLuint>& rIndex = grid.GetIndex();

    for(int nIndex=0; nIndex<rIndex.size(); ++nIndex)
    {
        m_pGeometry->addPrimitiveSet(new osg::DrawElementsUInt(GL_TRIANGLE_STRIP,rIndex[nIndex].size(),&rIndex[nIndex].front()));
    }
    m_bBuild = true;

    //m_pColorArray->push_back(m_vColor);
}

void CSceneDonuts::UpdateShape()
{
    double dR = (m_dRMax - m_dRMin) / 2.0;
    CGrid2TriangleStrip grid(25,25,true);

    const osg::MixinVector<osg::VectorGLuint>& rIndex = grid.GetIndex();
   // m_pGeometry->setVertexArray(m_pVertexArray);

    osg::Vec3 vLocal;
    for(int i=0,indexi=0; i<=360; i +=15,++indexi)
    {
        osg::Matrix tmpMatrix = osg::Matrix::rotate(osg::DegreesToRadians((float)i),osg::Z_AXIS);
        for(int j=0,indexj=0; j<=360; j +=15,++indexj)
        {
            vLocal.z() = CSnGeoMathCommon::GetInstance()->GetCos()[j] * dR;
            vLocal.x() = m_dRMin+dR + CSnGeoMathCommon::GetInstance()->GetSin()[j] * dR;
            vLocal.y() = 0;

            vLocal = tmpMatrix.preMult(vLocal);
            m_pVertexArray->at(indexj*25 + indexi) = vLocal;
            //pVetexArray->push_back(vLocal);
        }
    }
}
