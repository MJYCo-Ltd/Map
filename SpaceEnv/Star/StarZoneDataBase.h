#ifndef I_Star_ZoneBase_H
#define I_Star_ZoneBase_H

#include <osg/Referenced>
#include <osg/Vec3f>


/** @interface 星空区域基类
 *  将星空按照 Zone进行划分
 */
struct IStarZoneData: public osg::Referenced
{
    /**
     * 获取该Zone中星星的数量
     */
    virtual unsigned int getStarsCount() const = 0;

    /**
     * 获取该Zone的中心位置
     */
    virtual const osg::Vec3f &getCenter() const = 0;

    /**
     * Query the first axis vector of this zone.
     */
    virtual const osg::Vec3f &getFirstAxis() const = 0;

    /**
     * Query the second axis vector of this zone.
     */
    virtual const osg::Vec3f &getSecondAxis() const = 0;

    /**
     * Query the normalized position of the star in J2000 coordinate system.
     * @param pos A vector to hold the position.
     */
    virtual void getStarPosition(unsigned int idx, float movementFactor,
            osg::Vec3f &pos) const = 0;

    /**
     * 根据索引查询恒星的亮度
     * @return The visual magnitude of the star.
     */
    virtual int getStarMag(unsigned int idx) const = 0;

    /**
     * 根据索引查找恒星的颜色
     * @return The BV magnitude.
     */
    virtual float getStarBV(unsigned int idx) const = 0;

    /**
     * @brief 根据索引查找恒星的颜色索引
     * @param idx
     * @return
     */
    virtual unsigned char getStarBVIndex(unsigned int idx) const=0;

    /**
     * 根据索引查找恒星的依巴谷编号
     */
    virtual int getStarHip(unsigned int idx) const = 0;
};

#endif // I_Star_ZoneBase_H
