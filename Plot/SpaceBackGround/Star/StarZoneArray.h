#ifndef STAR_ZONE_ARRAY_H
#define STAR_ZONE_ARRAY_H
/***************************************************************
  *
  * 每一个星表的Bin/Cat文件对应一个ZoneArray
  *
  **************************************************************/
#include <QFile>
#include <QtEndian>
#include <VersionMathCommon.h>
#include "StarZoneArrayBase.h"
#include "StarZoneData.h"

template<typename T>
class StarZoneArray: public IStarZoneArray
{
public:
    StarZoneArray() :m_fStarPositionScale(0.0),m_pZonesOffset(nullptr)
    {
    }

    virtual ~StarZoneArray()
    {
        delete [] m_pZonesOffset;

        m_pFile->close();
        delete m_pFile;
    }

    /**
     * Query the minimum mag value.
     */
    virtual int getMagMin() const
    {
        return m_nMagMin;
    }

    /**
     * Query the mag range value.
     */
    virtual int getMagRange() const
    {
        return m_nMagRange;
    }

    /**
     * Query the mag step.
     */
    virtual int getMagStep() const
    {
        return m_nMagSteps;
    }

    void scaleAxis()
    {
        m_fStarPositionScale /= T::MaxPosVal;
    }

    virtual void initTriangle(const osg::Vec3f &c0, const osg::Vec3f &c1,
            const osg::Vec3f &c2)
    {
        static const osg::Vec3f north(0, 0, 1);
        osg::Vec3f center = c0 + c1 + c2;
        center.normalize();
        osg::Vec3f axis0 = north ^ center;
        axis0.normalize();
        osg::Vec3f axis1 = center ^ axis0;

        // initialize star_position_scale:
        float mu0, mu1, f, h;
        mu0 = (c0 - center) * axis0;
        mu1 = (c0 - center) * axis1;
        f = 1.0 / sqrt(1.0 - mu0 * mu0 - mu1 * mu1);
        h = fabs(mu0) * f;

        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

        h = fabs(mu1) * f;
        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

        mu0 = (c1 - center) * axis0;
        mu1 = (c1 - center) * axis1;
        f = 1.0 / sqrt(1.0 - mu0 * mu0 - mu1 * mu1);
        h = fabs(mu0) * f;
        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

        h = fabs(mu1) * f;
        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

        mu0 = (c2 - center) * axis0;
        mu1 = (c2 - center) * axis1;
        f = 1.0 / sqrt(1.0 - mu0 * mu0 - mu1 * mu1);
        h = fabs(mu0) * f;
        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

        h = fabs(mu1) * f;
        if (m_fStarPositionScale < h)
        {
            m_fStarPositionScale = h;
        }

    }

    /// 获取星区的个数
    static int nrOfZones(int level)
    {
        return (20 << (level << 1));
    }

    static bool initTriangleFunc(int lev, unsigned int /*index*/,
            const osg::Vec3f &c0, const osg::Vec3f &c1, const osg::Vec3f &c2,
            void *context)
    {
        StarZoneArray<T> *zones = reinterpret_cast<StarZoneArray<T>*>(context);
        if (lev == zones->m_nLevel)
        {
            zones->initTriangle(c0, c1, c2);
        }
        return true;
    }

    /**
     * @brief 读取文件
     * @param type
     * @param level
     * @param pMapFile
     * @param dataSource
     * @return
     */
    bool read(int type, int level, QFile *pMapFile,
            void *dataSource)
    {
        m_pFile = pMapFile;
        this->m_nStarType = type;
        this->m_nLevel = level;


        m_unZonesCount = nrOfZones(level);

        int *data_head = reinterpret_cast<int *>(dataSource);
        m_nMagMin = qFromLittleEndian(*data_head++);
        m_nMagRange = qFromLittleEndian(*data_head++);
        m_nMagSteps = qFromLittleEndian(*data_head++);

        m_pZones = (unsigned int *)data_head;
        m_pZonesOffset = new unsigned int[m_unZonesCount];

        m_pStars = (T *) (m_pZones + m_unZonesCount);
        m_pZonesOffset[0] = 0;


        for (unsigned int i = 1; i < m_unZonesCount; i++)
        {
            m_pZonesOffset[i] = m_pZonesOffset[i - 1] + qFromLittleEndian(m_pZones[i - 1]);
        }

        return true;
    }

    /**
     * Query count of zones in this array.
     */
    virtual unsigned int getZonesCount() const
    {
        return m_unZonesCount;
    }

    /**
     * Query the specified zone in this array.
     * @param idx Zero based index of the zone wanted in this array.
     */
    virtual IStarZoneData* getZone(unsigned int idx, const osg::Vec3f &c0,
            const osg::Vec3f &c1, const osg::Vec3f &c2) const
    {
        assert(idx < m_unZonesCount);
        CStarZoneData<T> *zone = new CStarZoneData<T>();
        zone->m_unStarCount = *(m_pZones + idx);
        zone->m_pStars = m_pStars + m_pZonesOffset[idx];
        zone->setupZone(c0, c1, c2, m_fStarPositionScale);
        return zone;
    }

    /**
     * Convert the original mag data to float one.
     */
    virtual float convertMag(int mag)
    {
        return 0.001 * m_nMagMin + mag * (0.001 * m_nMagRange) / m_nMagSteps;
    }

    /**
     * Query the level of this array.
     */
    virtual int getLevel() const
    {
        return m_nLevel;
    }

    virtual float getMovementFactor(float JD) const
    {
        return DAS2R * 0.0001 * ((JD - DJ00) / DJY)
                / m_fStarPositionScale;
    }

    virtual int getType() const
    {
        return m_nStarType;
    }

private:
    /// 需要划分的星区级别 级别越高划分的 区域越多 0 级划分
    /// 20个 每增加一级 个数乘以 4
    int m_nLevel;                            /// 恒星区域个数
    int m_nStarType;                       /// 恒星数据类型
    int m_nMagMin;                       /// 最小的视星等信息
    int m_nMagRange;                   /// 视星等的范围
    int m_nMagSteps;                    /// 视星等的步长
    unsigned int m_unZonesCount; /// 一个文件中星区的个数
    float m_fStarPositionScale;        /// 恒星位置缩放系数

    unsigned int * m_pZones;          /// 指向恒星区域的首地址
    unsigned int * m_pZonesOffset; /// 每一个恒星区域的偏移量
    T * m_pStars;                             /// 指向恒星数据的首地址

    /**
     * @brief 星历文件
     */
    QFile* m_pFile;
};

#endif // STAR_ZONE_ARRAY_H
