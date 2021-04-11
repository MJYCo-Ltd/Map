#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <osg/Drawable>

class OSGEXTERNSHARED_EXPORT CJudgeGeometry:public osg::Drawable::DrawCallback
{
    CJudgeGeometry(std::string sName):m_sName(sName){}
    std::string m_sName;
    void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const;
};
