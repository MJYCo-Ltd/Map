#include <Inner/OsgExtern/JudgeGeometry.h>

void CJudgeGeometry::drawImplementation(osg::RenderInfo &renderInfo, const osg::Drawable *drawable) const
{
    drawable->drawImplementation(renderInfo);
    auto pLastProgram=renderInfo.getState()->getLastAppliedProgramObject();
    if(nullptr != pLastProgram)
    {

        auto pProgram = pLastProgram->getProgram();
        osg::notify(osg::WARN)<<"======================="<<m_sName<<"============================"<<std::endl;
        osg::notify(osg::WARN)<<pProgram->getName()<<"\n--------------------------------------------"<<std::endl;
        for(int i=0;i<pProgram->getNumShaders();++i)
        {
            osg::notify(osg::WARN)<<pProgram->getShader(i)->getName()<<"\n--------------------------------------------"<<std::endl;
            osg::notify(osg::WARN)<<pProgram->getShader(i)->getShaderSource()<<std::endl;
        }

        osg::notify(osg::WARN)<<"baseTexture:"<<renderInfo.getState()->getUniformMap().at("baseTexture").uniformVec[0].second<<std::endl;
    }
}
