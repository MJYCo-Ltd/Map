#ifndef IMPL_OSG_MAP_GEOMETRY_H
#define IMPL_OSG_MAP_GEOMETRY_H
#include <osgEarth/DrapeableNode>
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
        ImplMapSceneNode<T>::SetMapSceneNode(m_pDrapeNode.get());
    }

    void UpdateMapNode() override
    {
        ImplMapSceneNode<T>::UpdateMapNode();

        /// 如果有几何体
        if(nullptr != m_pGeometry)
        {
            m_pGeometry->NeedUpdate();
        }
    }

    /**
     * @brief 设置绘制几何体
     * @param 指定的绘制几何体
     */
    void SetGeometry(IGeometry* pGeometry)
    {
        m_pGeometry=pGeometry;
        AddNode(m_pDrapeNode.get(),pGeometry->AsOsgSceneNode()->GetOsgNode());
        m_pGeometry->SetDealPoint(this);
    }

    /**
     * @brief 瓦片数据更新
     * @param rTileKey
     */
    void TileDataChanged(const osgEarth::TileKey& rTileKey)
    {
        if(s_bIs3D && T::RELATIVE_TERRAIN==T::m_emType && nullptr != m_pGeometry)
        {
            const std::list<ScenePos> & rAllPos = m_pGeometry->BetterGetMulPos();
            for(const ScenePos& one : rAllPos)
            {
                if(rTileKey.getExtent().contains(one.dX,one.dY,s_pWGS84))
                {
                    m_pGeometry->NeedUpdate();
                    break;
                }
            }
        }
    }

    /**
     * @brief 状态更改
     */
    void UpdateTrerrain()
    {
        /// 根据高程类型选择是否开启贴地
        if(s_bIs3D)
        {
            m_pDrapeNode->setDrapingEnabled(T::CLOSE_TERRAIN == T::m_emType);
        }
        else
        {
            m_pDrapeNode->setDrapingEnabled(false);
        }
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
            vIn.at(nIndex++).set(one.dX,one.dY,one.dZ);
        }

        vAllPos.resize(vIn.size());
        if(s_bIs3D)
        {
            std::vector<osg::Vec3d> vOut;
            vOut.resize(vIn.size());
            for(int nIndex=vIn.size()-1;nIndex>-1;--nIndex)
            {
                ImplMapSceneNode<T>::s_pMapSRS->transformToWorld(vIn.at(nIndex),vOut.at(nIndex));
            }

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
                    if(terrain->getHeight(ImplMapSceneNode<T>::s_pWGS84.get(), one.dX, one.dY, &out_hamsl))
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
                vAllPos[nIndex].dX = one.x();
                vAllPos[nIndex].dY = one.y();
                vAllPos[nIndex].dZ = one.z();
                ++nIndex;
            }
            return(true);
        }
        else
        {
            /// 将经纬度信息转成地图的投影信息
            ImplMapSceneNode<T>::s_pWGS84->transform(vIn,ImplMapSceneNode<T>::s_pMapSRS.get());
            nIndex=0;
            for(auto one : vIn)
            {
                vAllPos[nIndex].dX = one.x();
                vAllPos[nIndex].dY = one.y();
                vAllPos[nIndex].dZ = one.z();
                ++nIndex;
            }
            return(true);
        }
    }
protected:
   IGeometry* m_pGeometry{};
   osg::observer_ptr<osgEarth::DrapeableNode> m_pDrapeNode;
};

#endif // IMPL_OSG_MAP_SHAPE_H
