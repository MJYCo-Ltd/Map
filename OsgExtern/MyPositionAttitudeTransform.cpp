#include "Inner/OsgExtern/MyPositionAttitudeTransform.h"

using namespace osg;
CMyPositionAttitudeTransform::CMyPositionAttitudeTransform()
{
}

/// 设置旋转矩阵
void CMyPositionAttitudeTransform::SetRotateMatrix(const Matrix & rMatrix)
{
    m_matRotate = rMatrix;
}

bool CMyPositionAttitudeTransform::computeLocalToWorldMatrix(Matrix& matrix,NodeVisitor*) const
{
    if (_referenceFrame==RELATIVE_RF)
    {
        if(IsTransform()) matrix.preMultTranslate(_position);
        if(!m_matRotate.isIdentity()) matrix.preMult(m_matRotate);
        if(IsScale()) matrix.preMultScale(_scale);
        if(IsPovi()) matrix.preMultTranslate(-_pivotPoint);
    }
    else // absolute
    {
        if(!m_matRotate.isIdentity()) matrix.set(m_matRotate);
        if(IsTransform()) matrix.postMultTranslate(_position);
        if(IsScale()) matrix.preMultScale(_scale);
        if(IsPovi()) matrix.preMultTranslate(-_pivotPoint);
    }
    return true;
}


bool CMyPositionAttitudeTransform::computeWorldToLocalMatrix(Matrix& matrix,NodeVisitor*) const
{
    if (_scale.x() == 0.0 || _scale.y() == 0.0 || _scale.z() == 0.0)
        return false;

    if (_referenceFrame==RELATIVE_RF)
    {
        if(IsTransform()) matrix.postMultTranslate(-_position);
        if(!m_matRotate.isIdentity()) matrix.postMult(m_matRotate);
        if(IsScale()) matrix.postMultScale(Vec3d(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z()));
        if(IsPovi()) matrix.postMultTranslate(_pivotPoint);
    }
    else // absolute
    {
        if(!m_matRotate.isIdentity()) matrix.set(m_matRotate);
        if(IsTransform()) matrix.preMultTranslate(-_position);
        if(IsScale()) matrix.postMultScale(Vec3d(1.0/_scale.x(), 1.0/_scale.y(), 1.0/_scale.z()));
        if(IsPovi()) matrix.postMultTranslate(_pivotPoint);
    }
    return true;
}

/// 是否移动
bool CMyPositionAttitudeTransform::IsTransform() const
{
    if(_position[0]==0.0 && _position[1] == 0.0 && _position[2] == 0.0)
    {
        return(false);
    }

    return(true);
}

bool CMyPositionAttitudeTransform::IsScale() const
{
    if(_scale[0]==1.0 && _scale[1] == 1.0 && _scale[2] == 1.0)
    {
        return(false);
    }

    return(true);
}

bool CMyPositionAttitudeTransform::IsPovi() const
{
    if(_pivotPoint[0]==0.0 && _pivotPoint[1] == 0.0 && _pivotPoint[2] == 0.0)
    {
        return(false);
    }

    return(true);
}
