#include <SceneGraph/ISceneGraph.h>
#include <Inner/ILoadResource.h>
#include "StarZoneArrayRenderer.h"
#include "GeodesicGrid.h"

static std::map<QString, const wchar_t *> g_mapStarName;

void InitStarName()
{
    if(!g_mapStarName.empty())
    {
        return;
    }

    g_mapStarName["Polaris"] = L" 北极星";
}

/// 颜色数组
const osg::Vec3f ColorTable[128] =
{
  osg::Vec3f(0.602745f,0.713725f,1.000000f),
  osg::Vec3f(0.604902f,0.715294f,1.000000f),
  osg::Vec3f(0.607059f,0.716863f,1.000000f),
  osg::Vec3f(0.609215f,0.718431f,1.000000f),
  osg::Vec3f(0.611372f,0.720000f,1.000000f),
  osg::Vec3f(0.613529f,0.721569f,1.000000f),
  osg::Vec3f(0.635490f,0.737255f,1.000000f),
  osg::Vec3f(0.651059f,0.749673f,1.000000f),
  osg::Vec3f(0.666627f,0.762092f,1.000000f),
  osg::Vec3f(0.682196f,0.774510f,1.000000f),
  osg::Vec3f(0.697764f,0.786929f,1.000000f),
  osg::Vec3f(0.713333f,0.799347f,1.000000f),
  osg::Vec3f(0.730306f,0.811242f,1.000000f),
  osg::Vec3f(0.747278f,0.823138f,1.000000f),
  osg::Vec3f(0.764251f,0.835033f,1.000000f),
  osg::Vec3f(0.781223f,0.846929f,1.000000f),
  osg::Vec3f(0.798196f,0.858824f,1.000000f),
  osg::Vec3f(0.812282f,0.868236f,1.000000f),
  osg::Vec3f(0.826368f,0.877647f,1.000000f),
  osg::Vec3f(0.840455f,0.887059f,1.000000f),
  osg::Vec3f(0.854541f,0.896470f,1.000000f),
  osg::Vec3f(0.868627f,0.905882f,1.000000f),
  osg::Vec3f(0.884627f,0.916862f,1.000000f),
  osg::Vec3f(0.900627f,0.927843f,1.000000f),
  osg::Vec3f(0.916627f,0.938823f,1.000000f),
  osg::Vec3f(0.932627f,0.949804f,1.000000f),
  osg::Vec3f(0.948627f,0.960784f,1.000000f),
  osg::Vec3f(0.964444f,0.972549f,1.000000f),
  osg::Vec3f(0.980261f,0.984313f,1.000000f),
  osg::Vec3f(0.996078f,0.996078f,1.000000f),
  osg::Vec3f(1.000000f,1.000000f,1.000000f),
  osg::Vec3f(1.000000f,0.999643f,0.999287f),
  osg::Vec3f(1.000000f,0.999287f,0.998574f),
  osg::Vec3f(1.000000f,0.998930f,0.997861f),
  osg::Vec3f(1.000000f,0.998574f,0.997148f),
  osg::Vec3f(1.000000f,0.998217f,0.996435f),
  osg::Vec3f(1.000000f,0.997861f,0.995722f),
  osg::Vec3f(1.000000f,0.997504f,0.995009f),
  osg::Vec3f(1.000000f,0.997148f,0.994296f),
  osg::Vec3f(1.000000f,0.996791f,0.993583f),
  osg::Vec3f(1.000000f,0.996435f,0.992870f),
  osg::Vec3f(1.000000f,0.996078f,0.992157f),
  osg::Vec3f(1.000000f,0.991140f,0.981554f),
  osg::Vec3f(1.000000f,0.986201f,0.970951f),
  osg::Vec3f(1.000000f,0.981263f,0.960349f),
  osg::Vec3f(1.000000f,0.976325f,0.949746f),
  osg::Vec3f(1.000000f,0.971387f,0.939143f),
  osg::Vec3f(1.000000f,0.966448f,0.928540f),
  osg::Vec3f(1.000000f,0.961510f,0.917938f),
  osg::Vec3f(1.000000f,0.956572f,0.907335f),
  osg::Vec3f(1.000000f,0.951634f,0.896732f),
  osg::Vec3f(1.000000f,0.946695f,0.886129f),
  osg::Vec3f(1.000000f,0.941757f,0.875526f),
  osg::Vec3f(1.000000f,0.936819f,0.864924f),
  osg::Vec3f(1.000000f,0.931881f,0.854321f),
  osg::Vec3f(1.000000f,0.926942f,0.843718f),
  osg::Vec3f(1.000000f,0.922004f,0.833115f),
  osg::Vec3f(1.000000f,0.917066f,0.822513f),
  osg::Vec3f(1.000000f,0.912128f,0.811910f),
  osg::Vec3f(1.000000f,0.907189f,0.801307f),
  osg::Vec3f(1.000000f,0.902251f,0.790704f),
  osg::Vec3f(1.000000f,0.897313f,0.780101f),
  osg::Vec3f(1.000000f,0.892375f,0.769499f),
  osg::Vec3f(1.000000f,0.887436f,0.758896f),
  osg::Vec3f(1.000000f,0.882498f,0.748293f),
  osg::Vec3f(1.000000f,0.877560f,0.737690f),
  osg::Vec3f(1.000000f,0.872622f,0.727088f),
  osg::Vec3f(1.000000f,0.867683f,0.716485f),
  osg::Vec3f(1.000000f,0.862745f,0.705882f),
  osg::Vec3f(1.000000f,0.858617f,0.695975f),
  osg::Vec3f(1.000000f,0.854490f,0.686068f),
  osg::Vec3f(1.000000f,0.850362f,0.676161f),
  osg::Vec3f(1.000000f,0.846234f,0.666254f),
  osg::Vec3f(1.000000f,0.842107f,0.656346f),
  osg::Vec3f(1.000000f,0.837979f,0.646439f),
  osg::Vec3f(1.000000f,0.833851f,0.636532f),
  osg::Vec3f(1.000000f,0.829724f,0.626625f),
  osg::Vec3f(1.000000f,0.825596f,0.616718f),
  osg::Vec3f(1.000000f,0.821468f,0.606811f),
  osg::Vec3f(1.000000f,0.817340f,0.596904f),
  osg::Vec3f(1.000000f,0.813213f,0.586997f),
  osg::Vec3f(1.000000f,0.809085f,0.577090f),
  osg::Vec3f(1.000000f,0.804957f,0.567183f),
  osg::Vec3f(1.000000f,0.800830f,0.557275f),
  osg::Vec3f(1.000000f,0.796702f,0.547368f),
  osg::Vec3f(1.000000f,0.792574f,0.537461f),
  osg::Vec3f(1.000000f,0.788447f,0.527554f),
  osg::Vec3f(1.000000f,0.784319f,0.517647f),
  osg::Vec3f(1.000000f,0.784025f,0.520882f),
  osg::Vec3f(1.000000f,0.783731f,0.524118f),
  osg::Vec3f(1.000000f,0.783436f,0.527353f),
  osg::Vec3f(1.000000f,0.783142f,0.530588f),
  osg::Vec3f(1.000000f,0.782848f,0.533824f),
  osg::Vec3f(1.000000f,0.782554f,0.537059f),
  osg::Vec3f(1.000000f,0.782259f,0.540294f),
  osg::Vec3f(1.000000f,0.781965f,0.543529f),
  osg::Vec3f(1.000000f,0.781671f,0.546765f),
  osg::Vec3f(1.000000f,0.781377f,0.550000f),
  osg::Vec3f(1.000000f,0.781082f,0.553235f),
  osg::Vec3f(1.000000f,0.780788f,0.556471f),
  osg::Vec3f(1.000000f,0.780494f,0.559706f),
  osg::Vec3f(1.000000f,0.780200f,0.562941f),
  osg::Vec3f(1.000000f,0.779905f,0.566177f),
  osg::Vec3f(1.000000f,0.779611f,0.569412f),
  osg::Vec3f(1.000000f,0.779317f,0.572647f),
  osg::Vec3f(1.000000f,0.779023f,0.575882f),
  osg::Vec3f(1.000000f,0.778728f,0.579118f),
  osg::Vec3f(1.000000f,0.778434f,0.582353f),
  osg::Vec3f(1.000000f,0.778140f,0.585588f),
  osg::Vec3f(1.000000f,0.777846f,0.588824f),
  osg::Vec3f(1.000000f,0.777551f,0.592059f),
  osg::Vec3f(1.000000f,0.777257f,0.595294f),
  osg::Vec3f(1.000000f,0.776963f,0.598530f),
  osg::Vec3f(1.000000f,0.776669f,0.601765f),
  osg::Vec3f(1.000000f,0.776374f,0.605000f),
  osg::Vec3f(1.000000f,0.776080f,0.608235f),
  osg::Vec3f(1.000000f,0.775786f,0.611471f),
  osg::Vec3f(1.000000f,0.775492f,0.614706f),
  osg::Vec3f(1.000000f,0.775197f,0.617941f),
  osg::Vec3f(1.000000f,0.774903f,0.621177f),
  osg::Vec3f(1.000000f,0.774609f,0.624412f),
  osg::Vec3f(1.000000f,0.774315f,0.627647f),
  osg::Vec3f(1.000000f,0.774020f,0.630883f),
  osg::Vec3f(1.000000f,0.773726f,0.634118f),
  osg::Vec3f(1.000000f,0.773432f,0.637353f),
  osg::Vec3f(1.000000f,0.773138f,0.640588f),
  osg::Vec3f(1.000000f,0.772843f,0.643824f),
  osg::Vec3f(1.000000f,0.772549f,0.647059f)
};

CStarZoneArrayRenderer::CStarZoneArrayRenderer(ISceneGraph *pSceneGraph, IStarZoneArray *za, double JD
                                               , GeodesicGrid *pGrid, const QMap<int, QString> &hip2Names)
    :m_pSceneGraph(pSceneGraph)
    ,m_fMinMag(10000.f)
    ,m_dJD(JD)
    ,m_mapHip2Names(hip2Names)
{
    InitStarName();

    m_nLevel = za->getLevel();
    m_nStartCount = za->getZonesCount();

    m_vStar.reserve(za->getZonesCount());

    for (unsigned int i = 0; i < m_nStartCount; i++)
    {
        osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

        geom->setUseDisplayList(true);
        m_vStar.push_back(geom);
    }

    ZoneInitContext cp;
    cp.render = this;
    cp.za = za;
    pGrid->visitTriangles(m_nLevel, initFunc, &cp);
}

bool CStarZoneArrayRenderer::initFunc(int lev, unsigned int index, const osg::Vec3f &c0,
        const osg::Vec3f &c1, const osg::Vec3f &c2, void *context)
{
    ZoneInitContext *cp = (ZoneInitContext*) context;
    if (lev == cp->render->m_nLevel)
    {
        cp->render->readZones(cp->za, index, c0, c1, c2);
        return false;
    }
    return true;
}

void CStarZoneArrayRenderer::readZones(IStarZoneArray *za, unsigned int idx, const osg::Vec3f &c0,
        const osg::Vec3f &c1, const osg::Vec3f &c2)
{
    IStarZoneData *zone = za->getZone(idx, c0, c1, c2);
    double movementfactor = za->getMovementFactor(m_dJD);
    osg::Vec3Array *vertices = new osg::Vec3Array;
    vertices->reserve(zone->getStarsCount());
    osg::Vec4Array *colors = new osg::Vec4Array;
    colors->reserve(zone->getStarsCount());
    osg::DrawElementsUShort *elements = new osg::DrawElementsUShort(
            GL_POINTS);

    static osg::Vec3 pos;
    for (unsigned int i = 0; i < zone->getStarsCount(); i++)
    {
        zone->getStarPosition(i, movementfactor, pos);
        pos.normalize();
        vertices->push_back(pos);
        unsigned char clr = zone->getStarBVIndex(i);

        if(clr > 128)
        {
            clr = 127;
        }

        float mag = za->convertMag(zone->getStarMag(i));

        if(mag < m_fMinMag)
        {
            m_fMinMag = mag;
        }

        osg::Vec4 starColor(ColorTable[clr].x(), ColorTable[clr].y(),ColorTable[clr].z(),mag*1e-3);
        colors->push_back(starColor);
        elements->push_back(i);

        if(mag<= 8)
        {
            QMap<int, QString>::const_iterator itor = m_mapHip2Names.find(zone->getStarHip(i));
            if(m_mapHip2Names.end() != itor)
            {
                osg::ref_ptr<osgText::Text> text = new osgText::Text;
                text->setFontResolution(40, 40);
                starColor.a() = 1.0;
                text->setColor(starColor);
                text->setCharacterSize(24);
                if (g_mapStarName.find(itor.value()) != g_mapStarName.end())
                {
                    text->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/fzsh.ttf"));
                    text->setText(g_mapStarName[itor.value()]);
                }
                else
                {
                    text->setFont(m_pSceneGraph->ResouceLoader()->LoadFont("fonts/courbi.ttf"));
                    text->setText(itor.value().toStdWString().data());
                }
                text->setAxisAlignment(osgText::Text::SCREEN);
                text->setCharacterSizeMode(osgText::Text::SCREEN_COORDS);
                text->setBackdropType(osgText::Text::OUTLINE);
                text->setBackdropColor(osg::Vec4(0, 0, 0, 1));
                text->setPosition(pos);
                text->setAlignment(osgText::Text::LEFT_CENTER);

                m_vStarNames.push_back(text);
            }
        }
    }

    m_vStar[idx]->setVertexArray(vertices);
    m_vStar[idx]->setColorArray(colors);
    m_vStar[idx]->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    m_vStar[idx]->addPrimitiveSet(elements);
}

osg::Geometry *CStarZoneArrayRenderer::getGeometry(float maxMag, unsigned int idx)
{
    if (maxMag > 0 && maxMag < m_fMinMag)
    {
        return nullptr;
    }
    else
    {
        return m_vStar[idx];
    }
}
