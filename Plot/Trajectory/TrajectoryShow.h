#ifndef TRAJECTORYSHOW_H
#define TRAJECTORYSHOW_H
#include "Math/Vector.h"
#include <osg/Geometry>

class CTrajectoryShow
{
public:
    CTrajectoryShow();

    /**
     * @brief 设置轨道数据
     * @param vTrajectory
     */
    void SetTrajectory(const std::vector<Math::CVector>& vTrajectory);

    osg::Node* GetNode();
private:
    std::vector<Math::CVector> m_vAllData;
    osg::ref_ptr<osg::Group>   m_pRoot;
    osg::ref_ptr<osg::Geometry> m_Oribit;
};

#endif // TRAJECTORYSHOW_H
