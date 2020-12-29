#ifndef SA_STAR_H
#define SA_STAR_H

/***
 *  该文件用于描述依巴谷星表中恒星信息
 * ***********************/
#include <QtCore>
#include "StarZoneDataBase.h"

const qint32   UNVALID_HIP(-1);
const quint32 FILE_MAGIC(0x835f040a);
const quint32 FILE_MAGIC_OTHER_ENDIAN(0x0a045f83);
const quint32 FILE_MAGIC_NATIVE(0x835f040b);
const quint32 MAX_MAJOR_FILE_VERSION(0);

float IndexToBV(quint8 bV);
quint8 GetColorIndex(float bV);

class CCatStar1
{
public:
    enum {MaxPosVal=0x7FFFFFFF};

    void getPosition(const IStarZoneData *z,float movementFactor, osg::Vec3f& pos) const
    {
        pos = z->getCenter() + z->getFirstAxis() * (getX0() + movementFactor * getDx0())
                + z->getSecondAxis() * (getX1() + movementFactor * getDx1());
    }
    inline quint8 getBVIndex() const {return d.uint8[12];}
    inline int getMag() const {return d.uint8[13];}
    inline int getSpInt() const {return d.uint16[7];}
    inline int getX0() const { return qFromLittleEndian(d.int32[1]);}
    inline int getX1() const { return qFromLittleEndian(d.int32[2]);}
    inline int getDx0() const {return qFromLittleEndian(d.int32[4]);}
    inline int getDx1() const {return qFromLittleEndian(d.int32[5]);}
    inline int getPlx() const {return qFromLittleEndian(d.int32[6]);}

    /**
     * @brief 查询恒星在依巴谷星表中的编号
     * @return
     */
    inline int getHip() const
    {
        quint32 v = d.uint8[0] | d.uint8[1] << 8 | d.uint8[2] << 16;
        return ((qint32)v) << 8 >> 8;
    }

    inline int getComponentIds() const
    {
        return d.uint8[3];
    }

    float getBV(void) const {return IndexToBV(getBVIndex());}

private:
    /// 存储结构示意图 28 byte
    /*
              _______________
    0    hip |               |
    1        |               |
    2        |_______________|
    3   cIds |_______________|
    4     x0 |               |
    5        |               |
    6        |               |
    7        |_______________|
    8     x1 |               |
    9        |               |
    10       |               |
    11       |_______________|
    12    bV |_______________|
    13   mag |_______________|
    14 spInt |               |
    15       |_______________|
    16   dx0 |               |
    17       |               |
    18       |               |
    19       |_______________|
    20   dx1 |               |
    21       |               |
    22       |               |
    23       |_______________|
    24   plx |               |
    25       |               |
    26       |               |
    27       |_______________|

    */

    // componentIds         8
    // hip                  24
    //
    // qint32 x0            32
    // qint32 x1            32
    //
    // unsigned char bV     8
    // unsigned char mag    8
    // Uint16 spInt         16
    //
    // qint32 dx0,dx1,plx   32

    // Use an union so we can access the data as different types without
    // aliasing issues.
    union {
        quint8  uint8[28];
        quint16 uint16[14];
        qint32  int32[7];
    } d;
};

class CCatStar2
{
public:

    enum {MaxPosVal=((1<<19)-1)};

    inline int getX0() const
    {
        quint32 v = d[0] | d[1] << 8 | (d[2] & 0xF) << 16;
        return ((qint32)v) << 12 >> 12;
    }

    inline int getX1() const
    {
        quint32 v = d[2] >> 4 | d[3] << 4 | d[4] << 12;
        return ((qint32)v) << 12 >> 12;
    }

    inline int getDx0() const
    {
        quint16 v = d[5] | (d[6] & 0x3F) << 8;
        return ((qint16)(v << 2)) >> 2;
    }

    inline int getDx1() const
    {
        quint16 v = d[6] >> 6 | d[7] << 2 | (d[8] & 0xF) << 10;
        return ((qint16)(v << 2)) >> 2;
    }

    inline quint8 getBVIndex() const
    {
        return d[8] >> 4 | (d[9] & 0x7) << 4;
    }

    inline int getMag() const
    {
        return d[9] >> 3;
    }

    inline int getHip() const
    {
        return(-1);
    }

    void getPosition(const IStarZoneData *z,float movementFactor, osg::Vec3f& pos) const
    {
        pos = z->getCenter() + z->getFirstAxis() * (getX0() + movementFactor * getDx0())
                + z->getSecondAxis() * (getX1() + movementFactor * getDx1());
    }
    float getBV(void) const {return IndexToBV(getBVIndex());}

private:
    /// 存储结构示意图 28 byte
    /*
              _______________
    0     x0 |               |
    1        |_______        |
    2     x1 |       |_______|
    3        |               |
    4        |_______________|
    5    dx0 |___            |
    6    dx1 |   |___________|
    7        |_______        |
    8     bV |_______|_______|
    9    mag |_________|_____| bV

    int x0          :20;
    int x1          :20;
    int dx0         :14;
    int dx1         :14;
    unsigned int bV :7;
    unsigned int mag:5;
    */
    quint8 d[10];
};

class CCatStar3
{
public:
    enum {MaxPosVal=((1<<17)-1)};

    inline int getX0() const
    {
        quint32 v = d[0] | d[1] << 8 | (d[2] & 0x3) << 16;
        return ((qint32)v) << 14 >> 14;
    }

    inline int getX1() const
    {
        quint32 v = d[2] >> 2 | d[3] << 6 | (d[4] & 0xF) << 14;
        return ((qint32)v) << 14 >> 14;
    }

    inline quint8 getBVIndex() const
    {
        return d[4] >> 4 | (d[5] & 0x7) << 4;
    }

    inline int getMag() const
    {
        return d[5] >> 3;
    }

    inline int getHip() const
    {
        return(-1);
    }

    void getPosition(const IStarZoneData *z,float, osg::Vec3f& pos) const
    {
        pos = z->getCenter() + z->getFirstAxis() * getX0()
                + z->getSecondAxis() * getX1();
    }

    float getBV() const {return IndexToBV(getBVIndex());}

private:
    /*
              _______________
    0     x0 |               |
    1        |___________    |
    2     x1 |           |___|
    3        |_______        |
    4     bV |_______|_______|
    5    mag |_________|_____| bV

    int x0                     :18
    int x1                     :18
    unsigned int bV     :7
    unsigned int mag  :5
    */
    quint8 d[6];
};

class CBinStar1
{
public:
    enum
    {
        MaxPosVal = 0x7FFFFFFF
    };

    void getPosition(const IStarZoneData *z, float movementFactor,
                     osg::Vec3f &pos) const;
    float getBV() const;
    inline quint8 getBVIndex() const{return(bV);}
    int    getMag() const{return(mag);}
    int    getHip() const{return(hip);}
private:
    qint32 hip,x0,x1,dx0,dx1;
    quint32 bV,mag;
};

class CBinStar2
{
public:
    enum
    {
        MaxPosVal = ((1 << 19) - 1)
    };

    void getPosition(const IStarZoneData *z, float movementFactor,
                     osg::Vec3f &pos) const;
    float getBV() const;

    inline quint8 getBVIndex() const{return(bV);}
    int    getMag(){return(mag);}
    int    getHip()const{return(UNVALID_HIP);}
private:
    qint32 x0,x1;
    qint16 dx0,dx1;
    quint8 bV;
    quint8 mag;
};

class CBinStar3
{
public:
    enum
    {
        MaxPosVal = ((1 << 17) - 1)
    };
    void getPosition(const IStarZoneData *z, float movementFactor,
            osg::Vec3f &pos) const;
    float getBV() const;
    inline quint8 getBVIndex() const{return(bV);}
    int    getMag()const{return(mag);}
    int    getHip() const{return(UNVALID_HIP);}
private:
    qint32 x0,x1;
    quint8 bV;
    quint8 mag;
};

#endif // SA_STAR_H
