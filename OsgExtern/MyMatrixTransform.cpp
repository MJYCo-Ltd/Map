#include <Inner/OsgExtern/MyMatrixTransform.h>

CMyMatrixTransform::CMyMatrixTransform()
{

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
