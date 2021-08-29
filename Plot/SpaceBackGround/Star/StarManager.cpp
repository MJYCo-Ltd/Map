#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>

#include "Star.h"
#include "StarManager.h"
#include "GeodesicGrid.h"
#include "StarRender.h"
#include "StarConstellation.h"


#include "Satellite/Date.h"

CStarManager::CStarManager(ISceneGraph *pSceneGraph)
    :m_pSceneGraph(pSceneGraph)
    ,m_unMaxLevel(0)
    ,m_fShowMaxMag(6.0f)
    ,m_bShowStarName(true)
    ,m_bShowConstell(true)
    ,m_pGeodesicGrid(nullptr)
{
    m_pRoot = new osg::Group;
    m_pStarName = new osg::Group;
    m_pConstell = new osg::Group;
    m_pRoot->addChild(m_pStarName);
    m_pRoot->addChild(m_pConstell);

    auto pStarStateSet  = m_pSceneGraph->ResouceLoader()->LoadVirtualProgram("GLSL/Star.glsl");
    m_pStarRender = new CStarRender(pSceneGraph,m_fShowMaxMag);
    if(nullptr != m_pStarRender->getStateSet())
    {
        m_pStarRender->setStateSet(m_pSceneGraph->ResouceLoader()->MergeStateSet(pStarStateSet,m_pStarRender->getStateSet()));
    }
    else
    {
       m_pStarRender->setStateSet(pStarStateSet);
    }

    m_pRoot->addChild(m_pStarRender);

    m_pStarConstellation = new CStarConstellation(pSceneGraph);
}

CStarManager::~CStarManager()
{
    delete m_pGeodesicGrid;
    m_vZones.clear();
}

/// 处理星空文件
void CStarManager::ReadStar(int nMaxLevel, TYPE_STAR_DATA type)
{
    /// 设置文件夹
    std::string sStarsPath = GetDataPath() +"Space/stars";
    QDir containDir(QString::fromLocal8Bit(sStarsPath.c_str()));

    QStringList tmpFiters;

    switch(type)
    {
    case BIN:
        tmpFiters.push_back(QString("*.bin"));
        break;
    case CAT:
        tmpFiters.push_back(QString("*.cat"));
        break;
    }


    /// 查找所有的 指定后缀 的 文件名
    QFileInfoList fileList = containDir.entryInfoList(tmpFiters,QDir::NoDotAndDotDot|QDir::Readable|QDir::Files|QDir::Dirs);

    if (nMaxLevel < 0)
    {
        nMaxLevel = 1000;
    }

    switch(type)
    {
    case BIN:
        foreach (QFileInfo file, fileList)
        {
            ReadBin(file.absoluteFilePath(),nMaxLevel);
        }
        break;
    case CAT:
        foreach (QFileInfo file, fileList)
        {
            ReadCat(file.absoluteFilePath(),nMaxLevel);
        }
        break;
    }

    if(nullptr != m_pGeodesicGrid)
    {
        delete m_pGeodesicGrid;
    }

    /// 初始化星区
    m_pGeodesicGrid = new GeodesicGrid(m_unMaxLevel);

    for (ZoneArrayVector::iterator itor = m_vZones.begin();m_vZones.end() != itor; ++itor)
    {
        IStarZoneArray* pZoneArray = itor->get();
        if(nullptr != pZoneArray)
        {
            switch (type)
            {
            case BIN:
                switch(pZoneArray->getType())
                {
                case 0:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CBinStar1>::initTriangleFunc,pZoneArray);
                    break;
                case 1:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CBinStar2>::initTriangleFunc,pZoneArray);
                    break;
                case 2:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CBinStar3>::initTriangleFunc,pZoneArray);
                    break;
                }
                break;
            case CAT:
                switch(pZoneArray->getType())
                {
                case 0:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CCatStar1>::initTriangleFunc,pZoneArray);
                    break;
                case 1:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CCatStar2>::initTriangleFunc,pZoneArray);
                    break;
                case 2:
                    m_pGeodesicGrid->visitTriangles(pZoneArray->getLevel(),StarZoneArray<CCatStar3>::initTriangleFunc,pZoneArray);
                    break;
                }
                break;
            }
            pZoneArray->scaleAxis();
        }
    }

    ReadStarNames();
    m_pStarConstellation->ReadConstellation();

    /// 获取当前时间
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    QDate tmpDate = tmpDateTime.date();
    QTime tmpTime = tmpDateTime.time();
    double dSeconds = tmpTime.second();
    dSeconds += tmpTime.msec() / 1000.;

    /// 将时间转换成儒略日时间
    Aerospace::CDate tmpJDData(tmpDate.year(),tmpDate.month(),tmpDate.day()
                               ,tmpTime.hour(),tmpTime.minute(),dSeconds);

    double dJD = tmpJDData.GetJD();

    /// 设置信息
    m_pStarRender->setGeodesicGrid(m_vZones,m_pGeodesicGrid,m_unMaxLevel
                                   ,m_mapHip2Name,m_pStarName,dJD);
    m_pStarConstellation->setGeodesicGrid(m_unMaxLevel,m_vZones,m_pGeodesicGrid,m_pConstell,dJD);
}

/// 设置视星等
void CStarManager::SetShowMaxMag(float fMaxMag)
{
    if(fabs(m_fShowMaxMag - fMaxMag) > 1e-7f)
    {
        m_fShowMaxMag = fMaxMag;
    }
}

osg::Node *CStarManager::getNode()
{
    return(m_pRoot);
}

/// 设置恒星名字
void CStarManager::SetStarNameVisible(bool bVisible)
{
    if(m_bShowStarName == bVisible)
    {
        return;
    }
    else
    {
        m_bShowStarName = bVisible;

        if(m_bShowStarName)
        {
            m_pStarName->setNodeMask(-1);
        }
        else
        {
            m_pStarName->setNodeMask(0);
        }
    }
}

/// 控制星区显示
void CStarManager::SetConstellation(bool bVisible)
{
    if(m_bShowConstell == bVisible)
    {
        return;
    }
    else
    {
        m_bShowConstell = bVisible;

        if(m_bShowConstell)
        {
            m_pConstell->setNodeMask(-1);
        }
        else
        {
            m_pConstell->setNodeMask(0);
        }
    }
}

/// 读取星星名字
bool CStarManager::ReadStarNames()
{
    std::string s = GetDataPath() +"Space/stars/names";

    FILE *file=fopen(s.c_str(), "rb");
    if (!file)
    {
        return(false);
    }

    m_mapHip2Name.clear();

    qint32 hip;
    QString name;
    char c;
    while (!feof(file))
    {
        if (fread(&hip, sizeof(hip), 1, file) != 1)
        {
            continue;
        }

        name.clear();

        while (true)
        {
            if (fread(&c, sizeof(c), 1, file) != 1)
            {
                break;
            }

            if (c == '>')
            {
                break;
            }

            name.append(c);
        }
        m_mapHip2Name[hip] = name;
    }
    fclose(file);

    return(!m_mapHip2Name.empty());
}

/// 处理Bin文件
bool CStarManager::ReadBin(const QString &sFilename, int maxlevel)
{
    /// 如果打开文件失败
    QFile *pFile = new QFile(sFilename);
    if(!pFile->open(QIODevice::ReadOnly))
    {
        delete pFile;
        return(false);
    }

    int *head_data = (int*) pFile->map(0,pFile->size());
    int type = *head_data++;
    int level = *head_data++;

    if (level > maxlevel)
    {
        return false;
    }

    int findIndex = -1;
    for (int i = 0; i < (int) m_vZones.size(); i++)
    {
        if (m_vZones[i]->getLevel() == level)
        {
            findIndex = i;
            break;
        }
    }

    if (findIndex != -1)
    {
        return false;
    }

    switch (type)
    {
    case 0:
    {
        osg::ref_ptr<StarZoneArray<CBinStar1> > zone = new StarZoneArray<CBinStar1>();
        if (!zone->read(type, level, pFile, head_data))
        {
            return false;
        }
        m_vZones.push_back(zone);

        if (m_unMaxLevel < zone->getLevel())
        {
            m_unMaxLevel = zone->getLevel();
        }
        break;
    }

    case 1:
    {
        osg::ref_ptr<StarZoneArray<CBinStar2> > zone = new StarZoneArray<CBinStar2>();
        if (!zone->read(type, level, pFile, head_data))
        {
            return false;
        }
        m_vZones.push_back(zone);
        if (m_unMaxLevel < zone->getLevel())
        {
            m_unMaxLevel = zone->getLevel();
        }
        break;
    }

    case 2:
    {
        osg::ref_ptr<StarZoneArray<CBinStar3> > zone = new StarZoneArray<CBinStar3>();
        if (!zone->read(type, level, pFile, head_data))
            return false;
        m_vZones.push_back(zone);
        if (m_unMaxLevel < zone->getLevel())
        {
            m_unMaxLevel = zone->getLevel();
        }
        break;
    }

    default:
        return false;
    }

    return true;
}

/// 处理Cat文件
bool CStarManager::ReadCat(const QString &sFilename, int nMaxLevel)
{
    quint32 magic,major,minor,type,level;

    /// 如果打开文件失败
    QFile *pFile = new QFile(sFilename);
    if(!pFile->open(QIODevice::ReadOnly))
    {
        delete pFile;
        return(false);
    }

    quint32* pData = (quint32*)pFile->map(0,pFile->size());

    /// 获取数据
    magic = *pData++;
    type  = *pData++;
    major = *pData++;
    minor = *pData++;
    level = *pData++;


    /// 判断是否需要进行
    const bool bSwap = (magic == FILE_MAGIC_OTHER_ENDIAN);
    if (bSwap)
    {
        type = qFromLittleEndian(type);
        major = qFromLittleEndian(major);
        minor = qFromLittleEndian(minor);
        level = qFromLittleEndian(level);
    }

    if(level > nMaxLevel)
    {
        pFile->close();
        delete pFile;
        return(false);
    }

    /// 根据星星类型创建不同的对象
    switch (type)
    {
    case 0:
        if (major <= MAX_MAJOR_FILE_VERSION)
        {
            osg::ref_ptr<StarZoneArray<CCatStar1> > zone = new StarZoneArray<CCatStar1>();
            if (!zone->read(type, level, pFile, pData))
            {
                return false;
            }
            m_vZones.push_back(zone);
            if (m_unMaxLevel < zone->getLevel())
            {
                m_unMaxLevel = zone->getLevel();
            }
        }
        break;
    case 1:
        if (major <= MAX_MAJOR_FILE_VERSION)
        {
            osg::ref_ptr<StarZoneArray<CCatStar2> > zone = new StarZoneArray<CCatStar2>();
            if (!zone->read(type, level, pFile, pData))
            {
                return false;
            }
            m_vZones.push_back(zone);
            if (m_unMaxLevel < zone->getLevel())
            {
                m_unMaxLevel = zone->getLevel();
            }
        }
        break;
    case 2:
        if (major <= MAX_MAJOR_FILE_VERSION)
        {
            osg::ref_ptr<StarZoneArray<CCatStar3> > zone = new StarZoneArray<CCatStar3>();
            if (!zone->read(type, level, pFile, pData))
            {
                return false;
            }
            m_vZones.push_back(zone);
            if (m_unMaxLevel < zone->getLevel())
            {
                m_unMaxLevel = zone->getLevel();
            }
        }
        break;
    default:
        break;
    }
    return(true);
}
