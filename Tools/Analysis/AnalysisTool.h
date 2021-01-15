#ifndef CANALYSISTOOL_H
#define CANALYSISTOOL_H
#include <string>
#include <NoQt.h>
#include <Tools/ITool.h>
class CAnalysisTool
{
public:
    CAnalysisTool();
};

extern "C"
{
    Q_DECL_EXPORT ITool* CreateTool(ISceneGraph* pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
};
#endif // CANALYSISTOOL_H
