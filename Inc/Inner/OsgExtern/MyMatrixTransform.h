#ifndef CMYMATRIXTRANSFORM_H
#define CMYMATRIXTRANSFORM_H
#include <osg/MatrixTransform>
#include <Inner/OsgExtern/OsgExtern_Global.h>
class OSGEXTERNSHARED_EXPORT CMyMatrixTransform:public osg::MatrixTransform
{
public:
    CMyMatrixTransform();

    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const;

    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor*nv) const;
};

#endif // CMYMATRIXTRANSFORM_H
