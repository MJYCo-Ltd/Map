#ifndef CSCENEIMAGE_H
#define CSCENEIMAGE_H

#include <Plot/SceneShape/IImage.h>
#include <Inner/ImplSceneShape.hpp>
class CSceneImage:public ImplSceneShape<IImage>
{
public:
    CONSTRUCTOR(CSceneImage,ImplSceneShape<IImage>)
protected:
    void ImageSizeChanged();
    void ImagePathChanged()SET_TRUE_SHAPE_UPDATE(m_bPathChanged)
    void ImageDataChanged()SET_TRUE_SHAPE_UPDATE(m_bImageDataChanged)

    /**
     * @brief 初始化节点
     */
    void CreateShape();
    void UpdateShape();

protected:
    bool m_bSizeChanged=false;
    bool m_bPathChanged=false;
    bool m_bImageDataChanged=false;
    osg::ref_ptr<osg::Vec2Array>  m_pTexCoordArray;
};

#endif // CSCENEIMAGE_H
