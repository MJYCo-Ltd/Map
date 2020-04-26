#ifndef STAR_ZONE_ARRAY_BASE_H
#define STAR_ZONE_ARRAY_BASE_H
#include <vector>
#include <osg/ref_ptr>

#include "StarZoneDataBase.h"

/** @interface IStarZoneArray
 *   包含多个星区数据.
 */
struct IStarZoneArray: public osg::Referenced
{
    /**
     * Query count of zones in this array.
     */
    virtual unsigned int getZonesCount() const = 0;

    /**
     * Query the specified zone in this array.
     * @param idx Zero based index of the zone wanted in this array.
     */
    virtual IStarZoneData *getZone(unsigned int idx, const osg::Vec3f &c0,
            const osg::Vec3f &c1, const osg::Vec3f &c2) const = 0;

    /**
     * Query the level of this array.
     */
    virtual int getLevel() const = 0;

    /**
     * Convert the original mag data to float one.
     */
    virtual float convertMag(int mag) = 0;

    /**
     * Query the minimum mag value.
     */
    virtual int getMagMin() const = 0;

    /**
     * Query the mag range value.
     */
    virtual int getMagRange() const = 0;

    /**
     * Query the mag step.
     */
    virtual int getMagStep() const = 0;

    /**
     * Query the movement factor.
     */
    virtual float getMovementFactor(float JD) const = 0;

    /// Query the data type
    virtual int getType() const = 0;

    virtual void initTriangle(const osg::Vec3f &c0, const osg::Vec3f &c1,
            const osg::Vec3f &c2) = 0;
    virtual void scaleAxis() = 0;
};

typedef std::vector<osg::ref_ptr<IStarZoneArray> > ZoneArrayVector;

#endif // STAR_ZONE_ARRAY_BASE_H
