#ifndef SAFETY_LIST_HEADER_H
#define SAFETY_LIST_HEADER_H

#include <list>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

template<typename T>
class CSafetyList
{
public:
    /**
     * @brief 放入数据
     * @param crT
     */
    void Add(const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        m_listData.push_back(crT);
    }

    /**
     * @brief 获取所有的值，清空列表
     * @param rAll
     * @return
     */
    bool Take(std::list<T>& rAll)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        if(m_listData.empty())
        {
            return(false);
        }
        else
        {
            rAll.splice(rAll.end(), m_listData);
            m_listData.clear();
        }
    }

    /**
     * @brief 清空所有的
     */
    void Clear()
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        m_listData.clear();
    }
private:
    std::list<T>                m_listData;
    mutable OpenThreads::Mutex  m_listMutex;
};

#endif //SAFETY_LIST_HEADER_H
