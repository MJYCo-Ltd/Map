#ifndef IMPL_SCENE_SHAPE_H
#define IMPL_SCENE_SHAPE_H
#include <osg/Geometry>
#include <Inner/ImplSceneNode.hpp>

/**
 *  实现ISceneNode所有的接口
 */
template <typename T>
class ImplSceneShape:public ImplSceneNode<T>
{
public:
    CONSTRUCTOR(ImplSceneShape,ImplSceneNode<T>)

    /**
     * @brief 初始化节点
     */
    void InitNode()
    {
        ImplSceneNode<T>::m_unStateSet = BLEND_STATE|POLYGON_OFFSET_STATE;

        ImplSceneNode<T>::InitNode();
        /// 创建显示体
        m_pGeometry = new osg::Geometry;
        m_pGeometry->setDataVariance(osg::Object::DYNAMIC);

        ImplSceneNode<T>::SetOsgNode(m_pGeometry.get());

        /// 创建颜色
        m_pColorArray = new osg::Vec4Array;
        m_pColorArray->push_back(osg::Vec4(T::m_stColor.fR,T::m_stColor.fG,T::m_stColor.fB,T::m_stColor.fA));
        m_pGeometry->setColorArray(m_pColorArray,osg::Array::BIND_OVERALL);

        ///创建顶点
        m_pVertexArray = new osg::Vec3Array;
        m_pGeometry->setVertexArray(m_pVertexArray);

        ///创建法线
        m_pNormals = new osg::Vec3Array(osg::Array::BIND_PER_VERTEX);
        m_pGeometry->setNormalArray(m_pNormals);

        /// 构建绘制的形状
        CreateShape();
        m_pGeometry->setNodeMask(~PICK_MASK);
    }

protected:
    virtual void CreateShape()=0;
    virtual void UpdateShape()=0;

    /// 形状更改
    void ShapeChanged()SET_TRUE_NODE_UPDATE(m_bShapeChanged)
    void ColorChanged()SET_TRUE_NODE_UPDATE(m_bColorChanged)

    /// 颜色修改
    virtual void UpdateColor()
    {
        m_pColorArray->at(0).set(T::m_stColor.fR,T::m_stColor.fG,T::m_stColor.fB,T::m_stColor.fA);
    }

    /// 帧调用
    void FrameCall()
    {
        if(m_bColorChanged)
        {
            UpdateColor();
            m_pColorArray->dirty();
            m_bColorChanged = false;
        }

        if(m_bShapeChanged)
        {
            UpdateShape();
            m_pVertexArray->dirty();
            m_pGeometry->dirtyBound();
            m_bShapeChanged=false;
        }

        ImplSceneNode<T>::FrameCall();
    }
protected:
    osg::observer_ptr<osg::Geometry> m_pGeometry;
    osg::ref_ptr<osg::Vec3Array>     m_pVertexArray;
    osg::ref_ptr<osg::Vec4Array>     m_pColorArray;
    osg::ref_ptr<osg::Vec3Array>     m_pNormals;
    bool       m_bColorChanged{false};
    bool       m_bShapeChanged{false};
};

#endif // IMPL_SCENE_SHAPE_H
