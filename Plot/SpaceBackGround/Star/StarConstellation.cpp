#include <osgEarth/LineDrawable>
#include <ISceneCore.h>
#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "GeodesicGrid.h"
#include "StarConstellation.h"

static std::map<QString, const wchar_t *> g_mapConstellationName;

void InitConstellationName()
{
    if(!g_mapConstellationName.empty())
    {
        return;
    }

    g_mapConstellationName["Aquila"] = L"天鹰座";
    g_mapConstellationName["Andromeda"] = L"仙女座";
    g_mapConstellationName["Sculptor"] = L"玉夫座";
    g_mapConstellationName["Ara"] = L"天坛座";
    g_mapConstellationName["Libra"] = L"天秤座";
    g_mapConstellationName["Cetus"] = L"鲸鱼座";
    g_mapConstellationName["Aries"] = L"白羊座";
    g_mapConstellationName["Scutum"] = L"盾牌座";
    g_mapConstellationName["Pyxis"] = L"罗盘座";
    g_mapConstellationName["Bootes"] = L"牧夫座";
    g_mapConstellationName["Caelum"] = L"雕具座";
    g_mapConstellationName["Chamaeleon"] = L"蜓座";
    g_mapConstellationName["Cancer"] = L"巨蟹座";
    g_mapConstellationName["Capricornus"] = L"摩羯座";
    g_mapConstellationName["Carina"] = L"船底座";
    g_mapConstellationName["Cassiopeia"] = L"仙后座";
    g_mapConstellationName["Centaurus"] = L"半人马座";
    g_mapConstellationName["Cepheus"] = L"仙王座";
    g_mapConstellationName["Coma Berenices"] = L"后发座";
    g_mapConstellationName["Canes Venatici"] = L"猎犬座";
    g_mapConstellationName["Auriga"] = L"御夫座";
    g_mapConstellationName["Columba"] = L"天鸽座";
    g_mapConstellationName["Circinus"] = L"圆规座";
    g_mapConstellationName["Crater"] = L"巨爵座";
    g_mapConstellationName["Corona Australis"] = L"南冕座";
    g_mapConstellationName["Corona Borealis"] = L"北冕座";
    g_mapConstellationName["Corvus"] = L"乌鸦座";
    g_mapConstellationName["Crux"] = L"南十字座";
    g_mapConstellationName["Cygnus"] = L"天鹅座";
    g_mapConstellationName["Delphinus"] = L"海豚座";
    g_mapConstellationName["Dorado"] = L"剑鱼座";
    g_mapConstellationName["Draco"] = L"天龙座";
    g_mapConstellationName["Norma"] = L"矩尺座";
    g_mapConstellationName["Eridanus"] = L"波江座";
    g_mapConstellationName["Sagitta"] = L"天箭座";
    g_mapConstellationName["Fornax"] = L"天炉座";
    g_mapConstellationName["Gemini"] = L"双子座";
    g_mapConstellationName["Camelopardalis"] = L"鹿豹座";
    g_mapConstellationName["Canis Major"] = L"大犬座";
    g_mapConstellationName["Ursa Major"] = L"大熊座";
    g_mapConstellationName["Grus"] = L"天鹤座";
    g_mapConstellationName["Hercules"] = L"武仙座";
    g_mapConstellationName["Horologium"] = L"时钟座";
    g_mapConstellationName["Hydra"] = L"长蛇座";
    g_mapConstellationName["Hydrus"] = L"水蛇座";
    g_mapConstellationName["Indus"] = L"印第安座";
    g_mapConstellationName["Lacerta"] = L"蝎虎座";
    g_mapConstellationName["Monoceros"] = L"麒麟座";
    g_mapConstellationName["Lepus"] = L"天兔座";
    g_mapConstellationName["Leo"] = L"狮子座";
    g_mapConstellationName["Lupus"] = L"天狼座";
    g_mapConstellationName["Lynx"] = L"天猫座";
    g_mapConstellationName["Lyra"] = L"天琴座";
    g_mapConstellationName["Antlia"] = L"唧筒座";
    g_mapConstellationName["Microscopium"] = L"显微镜座";
    g_mapConstellationName["Musca"] = L"苍蝇座";
    g_mapConstellationName["Octans"] = L"南极座";
    g_mapConstellationName["Apus"] = L"天燕座";
    g_mapConstellationName["Ophiuchus"] = L"蛇夫座";
    g_mapConstellationName["Orion"] = L"猎户座";
    g_mapConstellationName["Pavo"] = L"孔雀座";
    g_mapConstellationName["Pegasus"] = L"飞马座";
    g_mapConstellationName["Pictor"] = L"绘架座";
    g_mapConstellationName["Perseus"] = L"英仙座";
    g_mapConstellationName["Equuleus"] = L"小马座";
    g_mapConstellationName["Canis Minor"] = L"小犬座";
    g_mapConstellationName["Leo Minor"] = L"小狮座";
    g_mapConstellationName["Vulpecula"] = L"狐狸座";
    g_mapConstellationName["Ursa Minor"] = L"小熊座";
    g_mapConstellationName["Phoenix"] = L"凤凰座";
    g_mapConstellationName["Pisces"] = L"双鱼座";
    g_mapConstellationName["Piscis Austrinus"] = L"南鱼座";
    g_mapConstellationName["Volans"] = L"飞鱼座";
    g_mapConstellationName["Puppis"] = L"船尾座";
    g_mapConstellationName["Reticulum"] = L"网罟座";
    g_mapConstellationName["Sagittarius"] = L"射手座";
    g_mapConstellationName["Scorpius"] = L"天蝎座";
    g_mapConstellationName["Serpens"] = L"巨蛇座";
    g_mapConstellationName["Sextans"] = L"六分仪座";
    g_mapConstellationName["Mensa"] = L"山案座";
    g_mapConstellationName["Taurus"] = L"金牛座";
    g_mapConstellationName["Telescopium"] = L"望远镜座";
    g_mapConstellationName["Tucana"] = L"杜鹃座";
    g_mapConstellationName["Triangulum"] = L"北三角座";
    g_mapConstellationName["Triangulum Australe"] = L"南三角座";
    g_mapConstellationName["Aquarius"] = L"宝瓶座";
    g_mapConstellationName["Virgo"] = L"处女座";
    g_mapConstellationName["Vela"] = L"船帆座";
}

CStarConstellation::CStarConstellation(ISceneGraph *pSceneGraph):
    m_pSceneGraph(pSceneGraph)
{
    InitConstellationName();
}

bool CStarConstellation::ReadConstellation()
{
    std::string s = GetDataPath() +"Space/stars/constellations";

    FILE *file=fopen(s.c_str(), "rb");
    if (!file)
    {
        return(false);
    }

    m_mapName2Hips.clear();

    QString name;
    qint32 count, hip;
    char c;

    QVector<int> vHips;
    osg::Vec3 tmp(0.f,0.f,0.f);
    while (!feof(file))
    {
        vHips.clear();
        name.clear();

        while (1)
        {
            if (fread(&c, sizeof(char), 1, file) != 1)
            {
                break;
            }

            if (c == '>')
            {
                break;
            }

            name.append(c);
        }

        /// 读取星星个数
        if (fread(&count, sizeof(count), 1, file) != 1)
        {
            continue;
        }
        count *= 2;

        vHips.reserve(count);

        for (int i = 0; i < count; ++i)
        {
            if (fread(&hip, sizeof(hip), 1, file) != 1)
            {
                break;
            }
            vHips.push_back(hip);
            m_mapHip2Pos[hip] = tmp;
        }

        if (vHips.size() != count)
        {
            continue;
        }
        else
        {
            m_mapName2Hips[name] = vHips;
        }
    }
    fclose(file);

    return(!m_mapName2Hips.empty());
}

///更新网格
void CStarConstellation::setGeodesicGrid(int nLevel, ZoneArrayVector &vZoneData, GeodesicGrid *pGeodesicGrid, osg::Group* pRoot,double dJD)
{
    FindContext context;
    context.vZoneArray = &vZoneData;
    context.pBuilder = this;
    context.dJD = dJD;
    pGeodesicGrid->visitTriangles(nLevel, findPosition, &context);

    BuildGeometry(pRoot);
}

/// 构建渲染体
void CStarConstellation::BuildGeometry(osg::Group* pRoot)
{
    osg::Geometry *geom = new osg::Geometry;
    osg::Vec3Array *verts = new osg::Vec3Array;
    osg::Vec4Array *colors = new osg::Vec4Array;

    int nIndexVert=0;
    geom->setVertexArray(verts);
    colors->push_back(osg::Vec4(0, 0.25, 0.5, 0.8));
    geom->setColorArray(colors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);

    /// 构建星座名字
    for (QMap<QString,QVector<int> >::iterator itor = m_mapName2Hips.begin();
         itor != m_mapName2Hips.end(); ++itor)
    {
        osg::Vec3 center(0, 0, 0);


        osg::DrawElementsUShort *el = new osg::DrawElementsUShort(GL_LINES);

        osg::Vec3 vTemp;
        for (int j = 0; j < itor.value().size();++j)
        {
            vTemp = m_mapHip2Pos[itor.value()[j]];

            verts->push_back(vTemp);
            center += vTemp;
            el->push_back(nIndexVert++);
        }
        center /= (double)itor.value().size();

        osg::ref_ptr<osgEarth::Text> text = new osgEarth::Text;
        text->setFontResolution(40, 40);
        text->setColor(osg::Vec4(0, 0.25, 0.5, 1.0));


        if (g_mapConstellationName.find(itor.key()) != g_mapConstellationName.end())
        {
            text->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/fzsh.ttf"));
            text->setText(g_mapConstellationName[itor.key()]);
        }
        else
        {
            text->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/courbi.ttf"));
            text->setText(itor.key().toUtf8().data(),osgText::String::ENCODING_UTF8);
        }

        text->setCharacterSize(26);
        text->setAxisAlignment(osgText::Text::SCREEN);
        text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
        text->setBackdropType(osgText::Text::OUTLINE);
        text->setBackdropColor(osg::Vec4(1, 1, 1, 1));
        text->setPosition(center);
        text->setAlignment(osgText::Text::CENTER_CENTER);
        pRoot->addChild(text);


        geom->addPrimitiveSet(el);
    }

    pRoot->addChild(geom);
}

/// 查找星星位置
bool CStarConstellation::findPosition(int lev, unsigned int index, const osg::Vec3f &c0,
        const osg::Vec3f &c1, const osg::Vec3f &c2, void *context)
{
    FindContext *fp = (FindContext*) context;
    IStarZoneArray *za = fp->vZoneArray->at(lev);
    if (!za)
    {
        return true;
    }

    IStarZoneData *zone = za->getZone(index, c0, c1, c2);
    double factor = za->getMovementFactor(fp->dJD);
    osg::Vec3 pos;

    for (unsigned int i = 0; i < zone->getStarsCount(); ++i)
    {
        if (fp->pBuilder->m_mapHip2Pos.find(zone->getStarHip(i))
                != fp->pBuilder->m_mapHip2Pos.end())
        {
            zone->getStarPosition(i, factor, pos);
            fp->pBuilder->m_mapHip2Pos[zone->getStarHip(i)] = pos;
        }
    }
    return true;
}
