#ifndef SAFETY_LIST_HEADER_H
#define SAFETY_LIST_HEADER_H

#include <list>
#include <vector>
#include <unordered_set>
#include <OpenThreads/Mutex>
#include <OpenThreads/ScopedLock>

template<typename T>
class CSafetyUniqueList
{
public:
    /**
     * @brief 放入数据
     * @param crT
     */
    void Add(const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        if(m_setUnique.end() == m_setUnique.find(crT))
        {
            m_setUnique.insert(crT);
            m_listData.push_back(crT);
        }
    }

    /**
     * @brief 获取所有的值
     * @param rAll
     * @return
     */
    void Get(std::vector<T>& rAll) const
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        rAll.reserve(m_listData.size());

        for(auto one : m_listData)
        {
            rAll.push_back(one);
        }
    }

    /**
     * @brief 清空所有的
     */
    void Clear()
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        m_setUnique.clear();
        m_listData.clear();
    }
private:
    std::list<T>                m_listData;
    std::unordered_set<T>       m_setUnique;
    mutable OpenThreads::Mutex  m_listMutex;
};

#endif //SAFETY_LIST_HEADER_H
