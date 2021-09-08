#ifndef SAFETY_LIST_HEADER_H
#define SAFETY_LIST_HEADER_H

#include <list>
#include <vector>
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
     * @brief 根据指定位置增加数据
     * @param nIndex
     * @param crT
     */
    void Add(int nIndex, const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        if(nIndex <=0)
        {
            m_listData.push_front(crT);
        }
        else if(nIndex >= m_listData.size())
        {
            m_listData.push_back(crT);
        }
        else
        {
            auto pIter = m_listData.begin();
            for(int i=0; i<nIndex;++i,++pIter){}

            m_listData.insert(pIter,crT);
        }
    }

    /**
     * @brief 移除指定位置的点
     * @param nIndex
     */
    void RemoveIndex(int nIndex)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);

        auto iter = m_listData.begin();
        for(int i=0;i<nIndex;++i) ++iter;
        m_listData.erase(iter);
    }

    /**
     * @brief 获取数组大小
     * @return
     */
    int Size() const
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        return(m_listData.size());
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

    void Get(int nIndex,T& rT) const
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        auto iter = m_listData.begin();
        for(int i=0;i<nIndex;++i) ++iter;
        rT = *iter;
    }

    /**
     * @brief 更新所有的数据
     * @param vAll
     */
    void Update(const std::vector<T>& vAll)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        m_listData.clear();
        for(auto one : vAll)
        {
            m_listData.push_back(one);
        }
    }

    void Update(int nIndex,const T& crT)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m_listMutex);
        auto iter = m_listData.begin();
        for(int i=0;i<nIndex;++i) ++iter;
        *iter=crT;
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
