#include <VersionMathCommon.h>
#include <ISceneCore.h>
#include "RadarSensor.h"

void CRadarSensor::SetRadarData(const std::string &sFilePath, bool bIsRef)
{
    std::string appPath;
    if(bIsRef)
    {
        appPath = GetDataPath() + sFilePath;
    }
    else
    {
        appPath = sFilePath;
    }

    FILE* pFile = fopen(appPath.c_str(),"rb");
    if(NULL != pFile)
    {

        /// 将文件指针定位到文件结尾
        fseek(pFile,0,SEEK_END);

        /// 获取文件大小
        long lFileSize = ftell(pFile);

        /// 将文件指针重置到文件开始
        fseek(pFile,0,SEEK_SET);

        /// 开辟缓存大小
        char* pBuffer = new char[lFileSize+1]();

        /// 一次性读取所有的数据
        if(1 == fread(pBuffer,lFileSize,1,pFile))
        {
            DealBuffer(pBuffer);
        }
        fclose(pFile);

        /// 释放空间
        delete []pBuffer;
    }
}

void CRadarSensor::CreateShape()
{
}

void CRadarSensor::UpdateShape()
{
}

void CRadarSensor::DealBuffer(const char *pBuffer)
{
    Flank vFlank;
    FlankPoint ptFlank;

    double dAngle,dDist;
    /// 获取一个侧边上的数据
    while(pBuffer && 2 == sscanf(pBuffer,"%lf %lf",&dAngle,&dDist))
    {
        /// 将角度转换成弧度
        dAngle *= DD2R;
        ptFlank.dX = dDist * cos(dAngle);
        ptFlank.dZ = dDist * sin(dAngle);

        if(dDist > m_dMaxDis)
        {
            m_dMaxDis = dDist;
        }

        /// 将数据放到边上
        vFlank.push_back(ptFlank);
        pBuffer = strchr(pBuffer,'\n');
        if(0 == pBuffer)
        {
            break;
        }
        ++pBuffer;
    }
}

void CRadarSensor::CreateGeomery(double dRad, const Flank &vFlank)
{
    unsigned int nCount = 20;
    int nFlankCount = vFlank.size();

    /// 计算每个角度的步长
    double dStep{},dIndex{},dDist{},dAngle{};

    /// 如果大于2PI，则绘制整个圆周
    if(dRad >= 2* osg::PI)
    {
        dStep = osg::PI /(nCount>>1);
    }
    /// 如果小于2PI，则只绘制部分
    else
    {
        dStep = dRad / nCount;
    }

    /// 重新设置顶点数量
    m_pVertexArray->resize(nCount * nFlankCount + 1);
    m_pColorArray->push_back(osg::Vec4(0.5,0.5,0.5,0.5));

    /// 侧边的顶点索引数组
    osg::VectorGLuint vBesideIndex;
    vBesideIndex.resize(2*(nFlankCount-1)*nCount);

    float fRed;
    osg::Vec3 ptTemp;
    /// 遍历 设置位置
    for(unsigned int nIndex = 0; nIndex < nCount; ++nIndex,dIndex+= dStep)
    {
        dAngle = cos(dIndex);
        dDist    = sin(dIndex);
        /// 将值放到 数组中
        for(int nInnerIndex = 0; nInnerIndex < nFlankCount; ++nInnerIndex)
        {
            ptTemp.x() = vFlank[nInnerIndex].dX * dAngle;
            ptTemp.y() = vFlank[nInnerIndex].dX * dDist;
            ptTemp.z() = vFlank[nInnerIndex].dZ;
            m_pVertexArray->at(nIndex + nInnerIndex * nCount + 1) = ptTemp;


            fRed = std::max(0.5,ptTemp.length()/m_dMaxDis);
            m_pColorArray->push_back(osg::Vec4(fRed,fRed,fRed,0.5));

            /// 构建顶点数组
            if(nInnerIndex + 1 != nFlankCount)
            {
                vBesideIndex[2*(nIndex + nInnerIndex*nCount)] = nInnerIndex*nCount + nIndex + 1;
                vBesideIndex[2*(nIndex + nInnerIndex*nCount) + 1] = (nInnerIndex + 1)*nCount + nIndex + 1;
            }
        }
    }
}
