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
    int AddHole(const ScenePos& leftUp,const ScenePos& rightDown);

    /**
     * @brief 移除洞
     */
    bool RemoveHole(int nIndex);
protected:
    std::map<int,osg::ref_ptr<osg::Uniform>> m_mapId2Hole;
};

#endif // CFLATABILITY_H
