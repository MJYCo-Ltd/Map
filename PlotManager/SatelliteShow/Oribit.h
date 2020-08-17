#ifndef CORIBIT_H
#define CORIBIT_H

#include <Map/Plot/IOribit.h>

#include <Inner/QtOsgSceneNode.h>
class COribit:public QtOsgSceneNode<IOribit>
{
    friend class OribitUpdateCallBack;
public:
    COribit(ISceneGraph*);

    /**
     * @brief 设置轨道数据
     */
    void SetJ2000Oribit(const vector<Math::CVector> &);

    /**
     * @brief 更新轨道数据
     */
    void SetGeoOribit(const vector<Math::CVector>&);

    /**
     * @brief 设置轨道颜色
     */
    void SetOribitColor(const SceneColor&);

    void InitSceneNode();
private:
    bool m_bUpdate=false;
    SceneColor m_rColor;
    vector<Math::CVector> m_vOribit;
    osg::ref_ptr<osg::Geometry> m_pOribit;     /// 绘制卫星轨道的节点
};

#endif // CORIBIT_H
