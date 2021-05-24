#include <QtCore/qsystemdetection.h>
#include <ISceneCore.h>
#include "MyNotify.h"

CMyNotify::CMyNotify(const std::string &sLogPath,const char* pFileName)
{
    std::string sFilePath = sLogPath;
#ifdef Q_OS_WIN
    const char* pFindSubfix = strrchr(pFileName,'.');
    sFilePath.append(pFileName,pFindSubfix-pFileName);
#else
    sFilePath += pFileName;
#endif
    sFilePath += ".log";
    m_pFile = fopen(sFilePath.c_str(),"w");

    m_bInit = m_pFile != nullptr;
}

void CMyNotify::notify(osg::NotifySeverity, const char *message)
{
    if(m_bInit)
    {
        fwrite(message,strlen(message),1,m_pFile);
    }
}

CMyNotify::~CMyNotify()
{
    if(m_bInit)
    {
        fflush(m_pFile);
        fclose(m_pFile);
        delete m_pFile;
    }
}
