#ifndef INCLUDE_MAPNODE_CULL_BACK_H
#define INCLUDE_MAPNODE_CULL_BACK_H
#include <osg/Callback>
#include <osgViewer/View>
#include <Inner/OsgExtern/OsgExtern_Global.h>
#include <SceneGraph/IViewPort.h>
#include <SceneGraph/ISceneGraph.h>
#include <SceneGraph/IWindow.h>
#include <Math/Vector.h>

class OSGEXTERNSHARED_EXPORT YtyUserData:public osg::Referenced
{
public:
    YtyUserData(){}

    void SetValue(const double& dX,const double& dY, const double& dZ);

    const double& GetDx()const{return(m_dX);}
    const double& GetDy()const{return(m_dY);}
    const double& GetDz()const{return(m_dZ);}
protected:
    ~YtyUserData(){}
protected:
    double m_dX;
    double m_dY;
    double m_dZ;
};

class OSGEXTERNSHARED_EXPORT CMapNodeCullBack:public osg::Callback,public IViewPortMessageObserver
{
public:
    CMapNodeCullBack(ISceneGraph*pSceneGraph);

    /**
     * @brief 重写父类方法
     */
    bool run(osg::Object* object, osg::Object* data);

    /**
     * @brief 视点更新
     */
    void EypePos(const ScenePos&);

    /**
     * @brief 设置鼠标位置
     */
    void SetMousePos(int nX, int nY);

    /**
     * @brief 获取眼睛位置
     * @return
     */
    const Math::CVector& GetEyePos()const;
protected:
    ~CMapNodeCullBack();
protected:
    ISceneGraph* m_pSceneGraph;
    osg::observer_ptr<osgViewer::View> m_pView;
    osg::ref_ptr<YtyUserData> m_pUserData;
    osg::ref_ptr<osgUtil::LineSegmentIntersector> m_pPicker;
    osg::Vec3d m_stStartVertex;
    osg::Vec3d m_stEndVertex;
    Math::CVector m_vEyePos;
    bool m_bInstelld=false;
    double m_dEyeLength;
    int    m_nX=0;
    int    m_nY=0;
};

#endif
