#ifndef CSATELLITE3D_H
#define CSATELLITE3D_H
#include <vector>
#include <osg/MatrixTransform>
#include <Math/VecMat.h>

using namespace std;
/**
 * @brief 卫星的三维显示
 */

class CSatellite3D:public osg::MatrixTransform
{
    friend class SatelliteUpdateCallBack;
    friend class OribitUpdateCallBack;
public:
    CSatellite3D();

protected:
    bool m_bUpdateSatellite=false;
    bool m_bUpdateOribit=false;
    Math::CVector m_stNowPos;
    vector<CVector> m_vOribit;
};

#endif // CSATELLITE3D_H
