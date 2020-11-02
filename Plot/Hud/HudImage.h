#ifndef CHUDIMAGE_H
#define CHUDIMAGE_H
#include <Plot/Hud/IHudImage.h>
#include <Inner/ImplHudNode.hpp>

class CHudImage:public ImplHudNode<IHudImage>
{
public:
    CONSTRUCTOR(CHudImage,ImplHudNode<IHudImage>)

protected:
    void ImageChanged();
protected:
    bool m_bImageChanged=false;
};

#endif // CHUDIMAGE_H
