#ifndef H_MY_POSITION_ATTITUDE_TRANSFORM_HEADER_H
#define H_MY_POSITION_ATTITUDE_TRANSFORM_HEADER_H
#include <osg/PositionAttitudeTransform>

#include "OsgExtern_Global.h"

/**
 * @brief 将显示节点从场景中移除
 */
class OSGEXTERNSHARED_EXPORT CMyPositionAttitudeTransform:public osg::PositionAttitudeTransform
{
public:
    CMyPositionAttitudeTransform();

    bool computeLocalToWorldMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const;
    bool computeWorldToLocalMatrix(osg::Matrix& matrix,osg::NodeVisitor*) const;

protected:
    ~CMyPositionAttitudeTransform(){}

    /**
     * @brief 提前判断，减少矩阵运算
     * @return
     */
    bool IsTransform() const;
    bool IsScale()const;
    bool IsPovi()const;
};

#endif // MAP_GLOBAL_H
