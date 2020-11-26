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
    void ImagePathChanged();

    /**
     * @brief 初始化节点
     */
    void CreateShape();
    void UpdateShape();

protected:
    bool m_bSizeChanged=false;
    bool m_bPathChanged=false;
    osg::ref_ptr<osg::Vec2Array>  m_pTexCoordArray;
};

#endif // CSCENEIMAGE_H
