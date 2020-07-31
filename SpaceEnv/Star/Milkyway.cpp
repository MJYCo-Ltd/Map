#include <osg/Texture2D>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>

#include "Milkyway.h"

CMilkyway::CMilkyway(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    makeMilkyway();
}

osg::Node *CMilkyway::getNode()
{
    return(m_pMilkyway.get());
}

CMilkyway::~CMilkyway()
{
}

/// 构建银河
void CMilkyway::makeMilkyway()
{
    osg::ref_ptr<osg::TessellationHints> pHints = new osg::TessellationHints;

    /// 纹理坐标
    pHints->setCreateTextureCoords(true);
    pHints->setCreateFrontFace(false);
    pHints->setCreateBackFace(true);
    pHints->setCreateNormals(false);
    pHints->setCreateTop(false);
    pHints->setCreateBody(false);
    pHints->setCreateBottom(false);

    osg::ref_ptr<osg::Sphere> pShape = new osg::Sphere(osg::Vec3(),1.0999f);
    m_pMilkyway = new osg::ShapeDrawable(pShape,pHints);

    m_pMilkyway->setColor(osg::Vec4f(1.0,1.0,1.0,1.0));

    osg::StateSet *state = m_pMilkyway->getOrCreateStateSet();

    /// 添加纹理
    osg::Texture2D *txt = m_pSceneGraph->ResouceLoader()->LoadTexture("SpaceResource/pixmaps/milkyway.png");

    state->setTextureAttributeAndModes(0, txt);
}
