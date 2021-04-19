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

    }

    for(auto one : renderInfo.getState()->getUniformMap())
    {
        osg::notify(osg::WARN)<<one.first<<":"<<std::endl;
        osg::notify(osg::WARN)<<"    UniformVec { ";
        for(auto itr = one.second.uniformVec.begin();
            itr != one.second.uniformVec.end();
            ++itr)
        {
            if (itr!=one.second.uniformVec.begin()) osg::notify(osg::WARN)<<", ";
            osg::notify(osg::WARN)<<"("<<itr->first<<", "<<itr->second<<")";
        }
        osg::notify(osg::WARN)<<" }"<<std::endl;
    }
}
