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
//        int nType;
//        float fType;
        osg::notify(osg::WARN)<<one.first<<":"<<std::endl;
        osg::notify(osg::WARN)<<"    UniformVec { ";
        for(auto itr = one.second.uniformVec.begin();
            itr != one.second.uniformVec.end();
            ++itr)
        {
            if (itr!=one.second.uniformVec.begin()) osg::notify(osg::WARN)<<", ";
            osg::notify(osg::WARN)<<"("<<itr->first<<':'
//                                  <<(itr->first->asUniform()->get(nType)?nType:
//                                     itr->first->asUniform()->get(fType)?fType:0)
                                  <<'\t'<<itr->second<<")";
        }
        osg::notify(osg::WARN)<<" }"<<std::endl;
    }

    osg::notify(osg::WARN)<<drawable->getStateSet()<<std::endl;
    osg::notify(osg::WARN)<<drawable->getStateSet()->getTextureAttribute(0,osg::StateAttribute::TEXTURE)<<std::endl;

//    osg::notify(osg::WARN)<<"Model"<<std::endl;
//    for(auto one : renderInfo.getState()->getModeMap())
//    {
//        osg::notify(osg::WARN)<<one.first<<":"<<std::endl;
//        osg::notify(osg::WARN)<<"valid:"<<one.second.valid<<std::endl;
//        osg::notify(osg::WARN)<<"changed:"<<one.second.changed<<std::endl;
//        osg::notify(osg::WARN)<<"last_applied_value:"<<one.second.last_applied_value<<std::endl;
//        osg::notify(osg::WARN)<<"global_default_value:"<<one.second.global_default_value<<std::endl;

//        for(auto itr = one.second.valueVec.begin();
//            itr != one.second.valueVec.end();++itr)
//        {
//            osg::notify(osg::WARN)<<*itr<<std::endl;
//        }
//    }

    osg::notify(osg::WARN)<<"TextureAttribute"<<std::endl;
    for(auto one : renderInfo.getState()->getTextureAttributeMapList())
    {
        for(auto oneTexture:one)
        {
            osg::notify(osg::WARN)<<oneTexture.first.second<<":"<<std::endl;
            osg::notify(osg::WARN)<<"last_applied_shadercomponent:"<<oneTexture.second.last_applied_shadercomponent<<std::endl;
            osg::notify(osg::WARN)<<"changed:"<<oneTexture.second.changed<<std::endl;
            osg::notify(osg::WARN)<<"last_applied_attribute:"<<oneTexture.second.last_applied_attribute<<std::endl;
            osg::notify(osg::WARN)<<"global_default_attribute:"<<oneTexture.second.global_default_attribute<<std::endl;

            for(auto itr = oneTexture.second.attributeVec.begin();
                itr != oneTexture.second.attributeVec.end();++itr)
            {
                osg::notify(osg::WARN)<<itr->first->getType()<<'\t'<<itr->first<<':'<<itr->second<<std::endl;
            }
        }
    }
}
