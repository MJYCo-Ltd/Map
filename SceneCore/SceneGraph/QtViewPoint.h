#ifndef QT_VIEWPOINT_H
#define QT_VIEWPOINT_H

#include <osgViewer/View>
#include <osgGA/TrackballManipulator>
#include <osgGA/NodeTrackerManipulator>

#include "SceneGraph/IViewPort.h"
#include "Map/IMap.h"
#include "Inner/IOsgViewPoint.h"

class IRender;
class IOsgSceneNode;
class QtViewPointUpdateCallback;
class CMyEarthManipulator;

/// 设置视点状态
enum ViewPointType
{
    View_2D,
    View_3D,
    View_Node
};

class QtViewPoint :public IViewPort,public IMapMessageObserver,public IOsgViewPoint
{
public:
    explicit QtViewPoint(IRender* pRender,ProjectType emProject=Perspective);
    ~QtViewPoint();

    /**
     * @brief 地图类型切换
     * @param emType
     */
    void MapTypeChanged(MapType emType);

    /**
     * @brief 设置跟踪的节点
     */
    void SetTrackNode(ISceneNode* pTrackNode);

    /**
     * @brief 获取跟踪的节点
     * @return
     */
    ISceneNode* GetTrackNode();

    /**
     * @brief 获取视口的屏显控制类
     * @return
     */
    IViewHud* GetHud();

    /**
     * @brief 设置视点位置
     */
    void SetViewPoint(const SceneViewPoint&rViewPoint);

    /**
     * @brief 获取视点位置
     * @return
     */
    const SceneViewPoint& GetViewPoint();

    /**
     * @brief 设置窗口大小
     * @param rViewPort
     */
    void SetViewPort(const CameraViewPort& rViewPort);

    /**
     * @brief 设置窗口大小
     * @return
     */
    const CameraViewPort& GetViewPort();

    /**
     * @brief 设置投影类型
     */
    void SetProjectType(ProjectType emProject);

    /**
     * @brief 获取投影类型
     * @return 当前的投影类型
     */
    ProjectType GetProjectType();

    /**
     * @brief 获取osgView
     * @return
     */
    osgViewer::View* GetOsgView();
protected:
    osg::ref_ptr<osgViewer::View>                  m_pView;
    osg::ref_ptr<QtViewPointUpdateCallback>        m_pCameraUpdate;
    osg::ref_ptr<CMyEarthManipulator>              m_p2DEarthManipulator; /// 地图操作器
    osg::ref_ptr<CMyEarthManipulator>              m_p3DEarthManipulator; /// 地图操作器
    osg::ref_ptr<osgGA::CameraManipulator>         m_pSelfManipulator;  /// 没有地图的操作器
    osg::ref_ptr<osgGA::NodeTrackerManipulator>    m_pTrackManipulator; /// 跟踪操作器
    bool                                           m_bIsMap=false;      /// 是否是地图
    ViewPointType                                  m_emType;

    IViewHud*      m_pHud=nullptr;                                       /// 屏显
    IRender*       m_pRender;
    IOsgSceneNode* m_pTrackNode=nullptr;
    SceneViewPoint m_rViewPoint;
    CameraViewPort m_rViewPort;
    ProjectType    m_emProjectType;
};

#endif // QTVIEWPOINT_H
