#ifndef SUNMODEL_H
#define SUNMODEL_H

#include <osg/MatrixTransform>

class CSunModel:public osg::MatrixTransform
{
    friend class SunGeometry;
    friend class SunUpdateCallback;
public:
    CSunModel();

    /**
     * @brief 更新行星位置
     * @param rPos
     */
    void UpdatePostion(const osg::Vec3& rPos);
    const osg::Vec3& GetUpdatePostion(){return(m_rECIPostion);}
private:
    bool              m_bNeedUpdate;   /// 是否需要更新
    osg::Vec3         m_rECIPostion;   /// 太阳的位置
};

#endif // SUNMODEL_H
