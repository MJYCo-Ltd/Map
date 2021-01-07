#include <Inner/OsgExtern/MyMatrixTransform.h>

CMyMatrixTransform::CMyMatrixTransform()
{
}

void CMyMatrixTransform::SetMatrix(const Math::CMatrix &rMatrix)
{
    _matrix.set(rMatrix(0,0),rMatrix(0,1),rMatrix(0,2),0.,
                rMatrix(1,0),rMatrix(1,1),rMatrix(1,2),0.,
                rMatrix(2,0),rMatrix(2,1),rMatrix(2,2),0.,
                0.,0.,0.,1.);
}

bool CMyMatrixTransform::computeLocalToWorldMatrix(osg::Matrix &matrix, osg::NodeVisitor *nv) const
{
    if(_matrix.isIdentity())
    {
        return(true);
    }
    else
    {
        return(osg::MatrixTransform::computeLocalToWorldMatrix(matrix,nv));
    }
}

bool CMyMatrixTransform::computeWorldToLocalMatrix(osg::Matrix &matrix, osg::NodeVisitor *nv) const
{
    if(_matrix.isIdentity())
    {
        return(true);
    }
    else
    {
        return(osg::MatrixTransform::computeWorldToLocalMatrix(matrix,nv));
    }
}
