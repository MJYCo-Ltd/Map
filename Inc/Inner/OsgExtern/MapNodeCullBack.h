#ifndef INCLUDE_MAPNODE_CULL_BACK_H
#define INCLUDE_MAPNODE_CULL_BACK_H

#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <osg/Callback>
#include <osgEarth/AutoClipPlaneHandler>

class OSGEXTERNSHARED_EXPORT CMapNodeCullBack:public osg::Callback
{
public:
    CMapNodeCullBack();

    /**
     * @brief 重写父类方法
     */
    bool run(osg::Object* object, osg::Object* data);
protected:
    ~CMapNodeCullBack(){}
protected:
    bool m_bInstelld=false;
    osg::ref_ptr<osgEarth::Util::AutoClipPlaneCullCallback> m_pCullBack;
};

#endif
