#ifndef SAFETYSET_HEADER_H
#define SAFETYSET_HEADER_H
#include <set>
#include <vector>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>
template<typename T>
class CSafetySet
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
     * @brief 移除数据
     * @param crT
     */
    void Remove(const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);

        m_setData.erase(crT);
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
    bool Take(std::vector<T>& rAll)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        if(m_setData.empty())
        {
            return(false);
        }
        else
        {
            rAll.reserve(m_setData.size());
            for(auto one : m_setData)
            {
                rAll.push_back(one);
            }
            m_setData.clear();
        }
        return(true);
    }

    /**
     * @brief 获取所有数据
     * @param rAll
     * @return
     */
    void GetAll(std::vector<T>& rAll)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_setMutex);
        if(m_setData.empty())
        {
            return;
        }

        rAll.reserve(m_setData.size());
        for(auto one : m_setData)
        {
            rAll.push_back(one);
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
