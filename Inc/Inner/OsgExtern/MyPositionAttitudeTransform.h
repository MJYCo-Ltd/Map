#ifndef H_MY_POSITION_ATTITUDE_TRANSFORM_HEADER_H
#define H_MY_POSITION_ATTITUDE_TRANSFORM_HEADER_H
#include <osg/PositionAttitudeTransform>

#include <Inner/OsgExtern/OsgExtern_Global.h>

/**
 * @brief 自定义平移旋转节点
 */
class OSGEXTERNSHARED_EXPORT CMyPositionAttitudeTransform:public osg::PositionAttitudeTransform
{
public:
    CMyPositionAttitudeTransform();

    /**
     * @brief设置旋转矩阵
     */
    void SetRotateMatrix(const osg::Matrix&);
    const osg::Matrix& RotateMatrix()const{return(m_matRotate);};

    /**
     * @brief 重写父类方法
     * @param matrix
     * @return
     */
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
private:
    osg::Matrix  m_matRotate;
};

#endif // MAP_GLOBAL_H
