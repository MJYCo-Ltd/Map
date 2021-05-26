#ifndef IMPL_OSG_MAP_GEOMETRY_H
#define IMPL_OSG_MAP_GEOMETRY_H
#include <osgEarth/DrapeableNode>
#include <osgEarth/Horizon>
#include <Inner/ImplMapSceneNode.hpp>
#include <Plot/SceneShape/IGeometry.h>

template <typename T>
class ImplMapSceneGeometry:public ImplMapSceneNode<T>,public DealPoint
{
public:
    CONSTRUCTOR(ImplMapSceneGeometry,ImplMapSceneNode<T>)

protected:

    void InitNode()
    {
        ImplMapSceneNode<T>::InitNode();

        /// 创建贴地节点
        m_pDrapeNode = new osgEarth::DrapeableNode;

        /// 增加地平面回调
        m_pCullCallBack = new osgEarth::HorizonCullCallback;
        m_pCullCallBack->setEnabled(true);
        m_pDrapeNode->addCullCallback(m_pCullCallBack);
        ImplMapSceneNode<T>::SetOsgNode(m_pDrapeNode.get());
    }

    void UpdateMapNode() override
    {
        m_pCullCallBack->setEnabled(ImplMapSceneNode<T>::s_pMapNode->isGeocentric());

        /// 如果有几何体
        if(nullptr != m_pGeometry)
        {
            m_pGeometry->NeedUpdate();
        }
    }

    /// 设置绘制几何体
    void SetGeometry(IGeometry* pGeometry)
    {
        m_pGeometry=pGeometry;
        AddNode(m_pDrapeNode.get(),pGeometry->AsOsgSceneNode()->GetOsgNode());
        m_pGeometry->SetDealPoint(this);
    }

    /**
     * @brief 状态更改
     */
    void UpdateTrerrain()
    {
        m_pDrapeNode->setDrapingEnabled(T::m_emType==T::CLOSE_TERRAIN);
//        if(T::RELATIVE_TERRAIN == T::m_emType)
//        {
//            auto pSate = m_pDrapeNode->getOrCreateStateSet();
//            pSate->setDefine("YTY_CLAMP");
//            auto m_pVirutlProgram = osgEarth::VirtualProgram::getOrCreate(pSate);
//            T::m_pSceneGraph->ResouceLoader()->LoadVirtualProgram(m_pVirutlProgram,"GLSL/Clamp.glsl");
//            m_pVirutlProgram->addBindAttribLocation("reletiveHight",osg::Drawable::ATTRIBUTE_6);
//        }
        m_pGeometry->NeedUpdate();
    }

    /**
     * @brief 坐标转换
     * @return
     */
    bool Conversion(const std::list<ScenePos>& listAllPos, std::vector<ScenePos>& vAllPos)
    {
        std::vector<osg::Vec3d> vIn;

        int nIndex(0);
        vIn.resize(listAllPos.size());
        for(auto one : listAllPos)
        {
            vIn.at(nIndex++).set(one.fX,one.fY,one.fZ);
        }

        /// 将经纬度信息转成地图的投影信息
        ImplMapSceneNode<T>::s_pWGS84->transform(vIn,ImplMapSceneNode<T>::s_pMapSRS.get());

        std::vector<osg::Vec3d> vOut;
        vOut.resize(vIn.size());
        for(int nIndex=vIn.size()-1;nIndex>-1;--nIndex)
        {
            ImplMapSceneNode<T>::s_pMapSRS.get()->transformToWorld(vIn.at(nIndex),vOut.at(nIndex));
        }

        vAllPos.resize(vOut.size());

        /// 如果是相对位置
        if(T::RELATIVE_TERRAIN == T::m_emType)
        {
            /// 根据高程计算
            double out_hamsl,dLength;

            osg::ref_ptr<osgEarth::Terrain> terrain;
            ImplMapSceneNode<T>::s_pTerrain.lock(terrain);

            nIndex=0;
            for(auto one : listAllPos)
            {
                if(terrain->getHeight(ImplMapSceneNode<T>::s_pWGS84.get(), one.fX, one.fY, &out_hamsl))
                {
                    dLength=vOut[nIndex].length();
                    vOut[nIndex] *= 1 + out_hamsl/dLength;
                }
                ++nIndex;
            }
        }

        nIndex=0;
        for(auto one : vOut)
        {
            vAllPos[nIndex].fX = one.x();
            vAllPos[nIndex].fY = one.y();
            vAllPos[nIndex].fZ = one.z();
            ++nIndex;
        }
        return(true);

    }
protected:
   IGeometry* m_pGeometry=nullptr;
   osg::observer_ptr<osgEarth::DrapeableNode> m_pDrapeNode;
   osg::ref_ptr<osgEarth::HorizonCullCallback> m_pCullCallBack;
};

#endif // IMPL_OSG_MAP_SHAPE_H
