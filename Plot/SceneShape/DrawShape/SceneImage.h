#ifndef CSCENEIMAGE_H
#define CSCENEIMAGE_H

#include <Plot/SceneShape/IImage.h>
#include <Inner/ImplSceneNode.hpp>
class CSceneImage:public ImplSceneNode<IImage>
{
public:
    CONSTRUCTOR(CSceneImage,ImplSceneNode<IImage>)
protected:
    void ImageSizeChanged();
    void ImagePathChanged()SET_TRUE_NODE_UPDATE(m_bPathChanged)
    void ImageDataChanged()SET_TRUE_NODE_UPDATE(m_bImageDataChanged)

    void InitNode();
    /**
     * @brief 初始化节点
     */
    void ColorChanged() SET_TRUE_NODE_UPDATE(m_bColorChanged)
    void ShapeChanged(){}

    /**
     * @brief 重写父类
     */
    void FrameCall();

protected:
    bool m_bSizeChanged{false};
    bool m_bPathChanged{false};
    bool m_bImageDataChanged{false};
    bool m_bColorChanged{false};
    osg::observer_ptr<osg::Geometry> m_pDrawNode;
};

#endif // CSCENEIMAGE_H
