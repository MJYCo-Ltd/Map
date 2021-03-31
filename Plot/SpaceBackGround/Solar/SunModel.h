#ifndef SUNMODEL_H
#define SUNMODEL_H

#include <osg/Camera>
#include <osg/MatrixTransform>
#include <Math/Vector.h>

class CSunModel:public osg::MatrixTransform
{
public:
    CSunModel();

    /**
     * @brief 更新行星位置
     * @param rPos
     */
    void UpdatePostion(const Math::CVector& rPos);

    void SetMatrix(const osg::Matrix& rMatrix);
private:
    bool              m_bNeedUpdate;   /// 是否需要更新
    osg::Vec3         m_rECIPostion;   /// 太阳的位置
    osg::ref_ptr<osg::Camera> m_pTrans;
};

#endif // SUNMODEL_H
