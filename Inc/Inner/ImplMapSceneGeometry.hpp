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
        T::m_bOpenPreCut=false;
        ImplMapSceneNode<T>::InitNode();

        /// 创建贴地节点
        m_pDrapeNode = new osgEarth::DrapeableNode;
        if(ImplMapSceneNode<T>::s_pMapNode.valid())
        {
            m_pDrapeNode->setMapNode(ImplMapSceneNode<T>::s_pMapNode.get());
        }

        /// 增加地平面回调
//        m_pCullCallBack = new osgEarth::HorizonCullCallback;
//        m_pCullCallBack->setEnabled(true);
//        m_pDrapeNode->addCullCallback(m_pCullCallBack);
        ImplMapSceneNode<T>::SetOsgNode(m_pDrapeNode.get());
    }

    void UpdateMapNode() override
    {
        if(m_pDrapeNode.valid())
        {
            m_pDrapeNode->setMapNode(ImplMapSceneNode<T>::s_pMapNode.get());
        }


//        m_pCullCallBack->setEnabled(ImplMapSceneNode<T>::s_pMapNode->isGeocentric());

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
        /// 不再贴地了
        if(CLOSE_TERRAIN != T::m_emType)
        {
            T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(new CReplaceNode(m_pDrapeNode.get(),
                                                                                      m_pGeometry->AsOsgSceneNode()->GetOsgNode()));
            ImplMapSceneNode<T>::SetOsgNode(m_pGeometry->AsOsgSceneNode()->GetOsgNode());
            m_pGeometry->NeedUpdate();
        }
        else
        {
            if(!m_pDrapeNode.valid())
            {
                m_pDrapeNode = new osgEarth::DrapeableNode;
                m_pDrapeNode->setMapNode(ImplMapSceneNode<T>::s_pMapNode.get());
                T::m_pSceneGraph->SceneGraphRender()->AddUpdateOperation(
                            new CReplaceNode(m_pGeometry->AsOsgSceneNode()->GetOsgNode(),m_pDrapeNode.get()));
                AddNode(m_pDrapeNode.get(),m_pGeometry->AsOsgSceneNode()->GetOsgNode());
            }
            ImplMapSceneNode<T>::SetOsgNode(m_pDrapeNode.get());
        }
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

        ImplMapSceneNode<T>::s_pWGS84->transform(vIn,ImplMapSceneNode<T>::s_pMapSRS.get());

        std::vector<osg::Vec3d> vOut;
        vOut.resize(vIn.size());
        for(int nIndex=vIn.size()-1;nIndex>-1;--nIndex)
        {
            ImplMapSceneNode<T>::s_pMapSRS.get()->transformToWorld(vIn.at(nIndex),vOut.at(nIndex));
        }

        vAllPos.resize(vOut.size());

        nIndex=0;
        for(auto one : vOut)
        {
            vAllPos[nIndex].fX = one.x();
            vAllPos[nIndex].fY = one.y();
            vAllPos[nIndex].fZ = one.z();
            ++nIndex;
        }

        /// 如果是相对位置
        if(T::RELATIVE_TERRAIN == T::m_emType)
        {
            double out_hamsl;

            nIndex=0;
            for(auto one : listAllPos)
            {
                if(ImplMapSceneNode<T>::s_pTerrain->getHeight(ImplMapSceneNode<T>::s_pWGS84.get(), one.fX, one.fY, &out_hamsl))
                {
                    vAllPos[nIndex].fZ +=out_hamsl;
                }
                ++nIndex;
            }

            return(0 != nIndex);
        }
        return(true);

    }
protected:
   IGeometry* m_pGeometry=nullptr;
   osg::observer_ptr<osgEarth::DrapeableNode> m_pDrapeNode;
   osg::ref_ptr<osgEarth::HorizonCullCallback> m_pCullCallBack;
};

#endif // IMPL_OSG_MAP_SHAPE_H
