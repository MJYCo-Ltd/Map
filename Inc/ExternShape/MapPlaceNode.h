#ifndef C_PLACENODEINFO_H
#define C_PLACENODEINFO_H

#include <ExternShape/ExternShape_Global.h>
#include <string>
class ISceneGraph;
class IMapLocation;
class ILabel;
class IImage;
class ISceneScreenGroup;
struct ScenePos;
struct SceneColor;
struct SceneImageSize;

class OSGEXTERNSHARED_EXPORT CPlaceNode
{
public:
    CPlaceNode(ISceneGraph*  pSceneGraph);

    /**
     * @brief 设置地雷的位置
     * @param fLon
     * @param fLat
     */
    void SetPos(float fLon, float fLat);
    const ScenePos GetPos();

    /**
     * @brief 设置显示的文字
     * @param sTextInfo
     */
    void SetText(const std::string& sTextInfo);

    /**
     * @brief 设置图标颜色
     */
    void SetColor(const SceneColor&);

    /**
     * @brief 设置图片路径
     * @param sPath
     */
    void SetImagePath(const std::string& sPath);

    /**
     * @brief 设置图片大小
     * @param stSize
     */
    void SetImageSize(const SceneImageSize& stSize);

    /**
     * @brief 获取位置节点
     * @return
     */
    IMapLocation* GetLocationNode(){return(m_pLocation);}

    /**
     * @brief 获取Label标签
     * @return
     */
    ILabel* GetLabel(){return(m_pLabel);}

    /**
     * @brief 获取图片
     * @return
     */
    IImage* GetImage(){return(m_pImage);}
protected:
    /**
      * @brief 初始化节点
      */
    void InitNode();

    void TypeChanged();
protected:
    ISceneGraph*  m_pSceneGraph{};
    IMapLocation* m_pLocation{};
    ISceneScreenGroup* m_pSceneScreenGroup{};
    ILabel*       m_pLabel{};
    IImage*       m_pImage{};
};
#endif // C_PLACENODEINFO_H
