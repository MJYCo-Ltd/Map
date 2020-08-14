#ifndef CSATELLITE3D_H
#define CSATELLITE3D_H
#include <vector>
#include <osgText/Text>
#include <osg/MatrixTransform>
#include <Math/VecMat.h>
#include <SceneType.h>
class ISceneGraph;
using namespace std;
/**
 * @brief 卫星的三维显示
 */

class CSatellite3D:public osg::Group
{
    friend class SatelliteUpdateCallBack;
    friend class OribitUpdateCallBack;
public:
    CSatellite3D(ISceneGraph*);

    /**
     * @brief 设置名称
     */
    void SetSatName(const string&);

    /**
     * @brief 设置卫星模型路径
     */
    void SetModelPath(const string&);

    /**
     * @brief 设置轨道颜色
     */
    void SetOribitColor(const SceneColor&);

    /**
     * @brief 设置缩放系数
     */
    void SetScalBit(double);

    /**
     * @brief 构建名称
     */
    void BuildName();

protected:
    bool m_bUpdateSatellite=false;
    bool m_bUpdateOribit=false;
    ISceneGraph* m_pSceneGraph=nullptr;
    osg::ref_ptr<osgText::Text> m_pName;
    Math::CVector m_stNowPos;
    vector<CVector> m_vOribit;
};

#endif // CSATELLITE3D_H
