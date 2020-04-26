#include <QFile>
#include <QDir>
#include <QDateTime>

#include "MyNotify.h"

CMyNotify::CMyNotify()
{
    m_pFile = new QFile(QDir::currentPath()+QDate::currentDate().toString("/yyyyMMdd.log"));
    m_pFile->open(QIODevice::WriteOnly|QIODevice::Text);
}

void CMyNotify::notify(osg::NotifySeverity, const char *message)
{
    m_pFile->write(message);
}

CMyNotify::~CMyNotify()
{
    m_pFile->flush();
    m_pFile->close();
    delete m_pFile;
}
