#ifndef CFLATABILITY_H
#define CFLATABILITY_H
#include <Ability/IFlatAbility.h>
#include "ImplNodeAbility.hpp"

class CFlatAbility:public ImplNodeAbility<IFlatAbility>
{
public:
    /**
     * @brief 初始化能力
     */
    void InitAbility();

    /**
     * @brief 移除能力
     */
    void RemoveAbility();

    /**
     * @brief 更新能力
     */
    void UpdateAbility();

    /**
     * @brief 添加洞
     */
    int AddHole(const FlatArea& flatArea);

    /**
     * @brief 移除洞
     */
    bool RemoveHole(int nIndex);
protected:
    std::map<int,FlatArea> m_mapId2Hole;
    osg::ref_ptr<osg::Uniform> m_pHoleMatrix;
    osg::ref_ptr<osg::Uniform> m_pHoleNum;
};

#endif // CFLATABILITY_H
