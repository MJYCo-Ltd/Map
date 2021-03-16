#ifndef SPACEENV_H
#define SPACEENV_H

#include <Map/SpaceEnv/ISpaceBackGround.h>
#include <Inner/ImplSceneNode.hpp>
#include <NoQt.h>
#include "SkyNode.h"

class CSpaceBackGround:public ImplSceneNode<ISpaceBackGround>
{
public:
    CONSTRUCTOR(CSpaceBackGround,ImplSceneNode<ISpaceBackGround>)
    ~CSpaceBackGround();
    /**
     * @brief 设置显示最大星等 默认为 6 最大星等为 20
     * @param nMax
     */
    void SetMaxVisibleMagnitude(int nMax);

    /**
     * @brief 设置银河是否可见
     * @param bShow
     */
    void SetMilkywayVisible(bool bShow);

    /**
     * @brief 设置星星名称是否可见
     * @param bShow
     */
    void SetStarNamesVisible(bool bShow);

    /**
     * @brief 设置边界是否可见
     * @param bShow
     */
    void SetBoundariesVisible(bool bShow);

    /**
     * @brief 设置星座连线是否可见
     * @param bShow
     */
    void SetConstellationLinesVisible(bool bShow);

    /**
     * @brief 设置星座名称是否可见
     * @param bShow
     */
    void SetConstellationNamesVisible(bool bShow);

    /**
     * @brief 设置行星名称是否可见
     * @param bShow
     */
    void SetPlanetsNamesVisible(bool bShow);

    /**
     * @brief UpdateDate
     */
    void DateChanged()SET_TRUE_NODE_UPDATE(m_bDateChanged)
    void MatrixChanged()SET_TRUE_NODE_UPDATE(m_bMatrixChanged)

    void ShowAtmosphere(bool);

    /**
     * @brief 初始化节点
     */
    void InitNode();
protected:
    void UpdateNode();
protected:
    osg::observer_ptr<CSkyNode> m_pSkyNode;
    bool  m_bDateChanged=false;
    bool  m_bMatrixChanged=false;
};


extern "C"
{
    Q_DECL_EXPORT ISceneNode* CreateNode(ISceneGraph*pSceneGraph,const std::string& sInterfaceName);
    Q_DECL_EXPORT bool QueryInterface(std::string& sInterfaceName);
}
#endif // SPACEENV_H
