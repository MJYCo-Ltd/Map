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
    std::vector<FlatArea> m_vInsertHole;
};

#endif // CFLATABILITY_H
