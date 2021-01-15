#ifndef CHUDIMAGE_H
#define CHUDIMAGE_H
#include <Plot/Hud/IHudImage.h>
#include <Inner/ImplHudNode.hpp>

class CHudImage:public ImplHudNode<IHudImage>
{
public:
    CONSTRUCTOR(CHudImage,ImplHudNode<IHudImage>)

protected:
    void ImageChanged()SET_TRUE_NODE_UPDATE(m_bImageChanged)
    void InitNode();
    void UpdateNode();
protected:
    bool m_bImageChanged=false;
    osg::observer_ptr<osgEarth::Controls::ImageControl> m_pImageControl;
};

#endif // CHUDIMAGE_H
