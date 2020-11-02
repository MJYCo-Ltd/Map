#ifndef CMYMATRIXTRANSFORM_H
#define CMYMATRIXTRANSFORM_H
#include <osg/MatrixTransform>
#include <Math/Matrix.h>
#include <Inner/OsgExtern/OsgExtern_Global.h>
class OSGEXTERNSHARED_EXPORT CMyMatrixTransform:public osg::MatrixTransform
{
public:
    CMyMatrixTransform();

    void SetMatrix(const Math::CMatrix& rMatrix);

    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const;

    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor*nv) const;
};

#endif // CMYMATRIXTRANSFORM_H
