#ifndef INCLUDE_JUDGE_GEOMETRY_H
#define INCLUDE_JUDGE_GEOMETRY_H

#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <osg/Drawable>

class OSGEXTERNSHARED_EXPORT CJudgeGeometry:public osg::Drawable::DrawCallback
{
public:
    CJudgeGeometry(std::string sName):m_sName(sName){}

    /**
     * @brief 重写父类的函数
     */
    void drawImplementation(osg::RenderInfo& renderInfo,const osg::Drawable* drawable) const;
private:
    std::string m_sName;
};

#endif
