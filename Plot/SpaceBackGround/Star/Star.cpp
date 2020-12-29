#include "Star.h"
static const double S_DINDEX2BV(4.0/127.);
static const double S_DBV2INDEX(127.0/4.0);

float IndexToBV(quint8 bV)
{
    return static_cast<float>(bV * S_DINDEX2BV - 0.50);
}

quint8 GetColorIndex(float bV)
{
    quint8 ret = static_cast<quint8> ((bV + 0.5) * S_DBV2INDEX);

    if (ret >= 128)
    {
        return 127;
    }
    else
    {
        return ret;
    }
}

void CBinStar1::getPosition(const IStarZoneData *z, float movementFactor, osg::Vec3f &pos) const
{
    pos = z->getCenter() + z->getFirstAxis() * (x0 + movementFactor * dx0)
            + z->getSecondAxis() * (x1 + movementFactor * dx1);
}

float CBinStar1::getBV() const
{
    return IndexToBV(bV);
}

void CBinStar2::getPosition(const IStarZoneData *z, float movementFactor, osg::Vec3f &pos) const
{
    pos = z->getCenter() + z->getFirstAxis() * (x0 + movementFactor * dx0)
            + z->getSecondAxis() * (x1 + movementFactor * dx1);
}

float CBinStar2::getBV() const
{
    return IndexToBV(bV);
}

void CBinStar3::getPosition(const IStarZoneData *z, float movementFactor, osg::Vec3f &pos) const
{
    Q_UNUSED(movementFactor);

    pos = z->getCenter() + z->getFirstAxis() * float(x0)
            + z->getSecondAxis() * float(x1);
}

float CBinStar3::getBV() const
{
    return IndexToBV(bV);
}
