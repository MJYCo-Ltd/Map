#ifndef MYNOTIFY_H
#define MYNOTIFY_H

#include <osg/Notify>

/**
 * @brief 用于接收osg和osgEarth打印的字符串
 */
class CMyNotify:public osg::NotifyHandler
{
public:
    CMyNotify(const std::string& sLogPath, const char *pFileName);

    /**
     * @brief 重写notify函数
     * @param message  将要打印的信息
     */
    void notify(osg::NotifySeverity, const char *message);
protected:
    ~CMyNotify();
private:
    FILE* m_pFile{};
    bool  m_bInit{false};
};

#endif // MYNOTIFY_H
