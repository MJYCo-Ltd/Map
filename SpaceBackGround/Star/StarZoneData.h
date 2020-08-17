#ifndef Star_ZONEDATA_H
#define Star_ZONEDATA_H

#include <osg/Vec3f>
#include <assert.h>
#include "StarZoneDataBase.h"

template<typename T>
class CStarZoneData:public IStarZoneData
{
public:
    /**
     * Query the count of stars in this zone.
     */
    virtual unsigned int getStarsCount() const
    {
        return m_unStarCount;
    }

    /**
     * Query the center vector of this zone.
     */
    virtual const osg::Vec3f &getCenter() const
    {
        return center;
    }

    /**
     * Query the first axis vector of this zone.
     */
    virtual const osg::Vec3f &getFirstAxis() const
    {
        return axis0;
    }

    /**
     * Query the second axis vector of this zone.
     */
    virtual const osg::Vec3f &getSecondAxis() const
    {
        return axis1;
    }

    virtual const osg::Vec3f &getCorner(int idx)
    {
        assert(idx >= 0 && idx < 3);
        return corner[idx];
    }

    /**
     * Query the normalized position of the star in J2000 coordinate system.
     * @param pos A vector to hold the position.
     */
    virtual void getStarPosition(unsigned int idx, float movementFactor,
            osg::Vec3f &pos) const
    {
        if (idx > m_unStarCount)
        {
            return;
        }

        m_pStars[idx].getPosition(this, movementFactor, pos);
    }

    /**
     * Query the visual magnitude of the star.
     * @return The visual magnitude of the star.
     */
    virtual int getStarMag(unsigned int idx) const
    {
        if (idx > m_unStarCount)
        {
            return -1;
        }

        return m_pStars[idx].getMag();
    }

    /**
     * Query the BV magnitude value of the star.
     * @return The BV magnitude.
     */
    virtual float getStarBV(unsigned int idx) const
    {
        if (idx > m_unStarCount)
        {
            return -1;
        }

        return m_pStars[idx].getBV();
    }

    virtual unsigned char getStarBVIndex(unsigned int idx) const
    {
        if (idx > m_unStarCount)
        {
            return 0;
        }

        return m_pStars[idx].getBVIndex();
    }

    /**
     * Query the star's HIP number
     */
    virtual int getStarHip(unsigned int idx) const
    {
        if (idx > m_unStarCount)
        {
            return -1;
        }

        return m_pStars[idx].getHip();
    }

    void setupZone(const osg::Vec3f &c0, const osg::Vec3f &c1,
            const osg::Vec3f &c2, double star_position_scale)
    {
        static const osg::Vec3f north(0, 0, 1);
        center = c0 + c1 + c2;
        center.normalize();
        axis0 = north ^ center;
        axis0.normalize();
        axis1 = center ^ axis0;

        axis0 *= star_position_scale;
        axis1 *= star_position_scale;
    }

public:
    /// Center of the zone.
    osg::Vec3f center;
    /// Axis
    osg::Vec3f axis0, axis1;
    /// Corners
    osg::Vec3f corner[3];

    unsigned int m_unStarCount;   /// 星星的个数
    T* m_pStars;                             /// 星星的地址
};

#endif // Star_ZONEDATA_H
