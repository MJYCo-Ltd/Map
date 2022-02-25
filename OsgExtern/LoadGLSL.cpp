#include <Inner/OsgExtern/LoadGLSL.h>

CLoadGLSL::CLoadGLSL(const std::string &sGlslFileName, const std::string &sGlslConfigFileName)
{
    LoadGlsl(sGlslFileName);
    LoadConfig(sGlslConfigFileName);
}

void CLoadGLSL::LoadGLSL(osg::State *pSateSet)
{
}

osg::StateSet *CLoadGLSL::ChangeFieldValue(const std::string &sFieldName, const Field &fieldValue)
{

    return(nullptr);
}

void CLoadGLSL::LoadGlsl(const std::string &sGlslFileName)
{
}

void CLoadGLSL::LoadConfig(const std::string &sConfigFileName)
{
    m_pDefaultStateset;
}
