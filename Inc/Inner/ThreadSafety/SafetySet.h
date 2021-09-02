#ifndef SAFETYSET_HEADER_H
#define SAFETYSET_HEADER_H
#include <set>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
template<typename T>
class SafetySet
{
public:
    /**
     * @brief 放入数据
     * @param crT
     */
    void Add(const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        m_setData.insert(crT);
    }

    /**
     * @brief 是否
     * @param crT
     * @return
     */
    bool Contain(const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        return(m_setData.end() != m_setData.find(crT));
    }

    /**
     * @brief 获取所有的值，清空列表
     * @param rAll
     * @return
     */
    bool Take(std::set<T>& rAll)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        if(m_setData.empty())
        {
            return(false);
        }
        else
        {
            for(auto one : m_setData)
            {
                rAll.insert(one);
            }
            m_setData.clear();
        }
    }

    /**
     * @brief 清空所有的
     */
    void Clear()
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        m_setData.clear();
    }
private:
    std::set<T>                 m_setData;
    mutable OpenThreads::Mutex  m_setMutex;
};

#endif SAFETYSET_HEADER_H
