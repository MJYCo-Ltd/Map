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
};

#endif // CSCENEIMAGE_H
