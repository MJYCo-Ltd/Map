/** @file
 * SpaceAnt Library file: 
 */

#include "GeodesicGrid.h"
#include <cassert>

static const float icosahedron_G = 0.5f * (1.0f + sqrt(5.0f));
static const float icosahedron_b = 1.0f / sqrt(1.0f + icosahedron_G * icosahedron_G);
static const float icosahedron_a = icosahedron_b * icosahedron_G;

static const osg::Vec3 icosahedron_corners[12] =
{
		osg::Vec3( icosahedron_a, -icosahedron_b,            0.0),
		osg::Vec3( icosahedron_a,  icosahedron_b,            0.0),
		osg::Vec3(-icosahedron_a,  icosahedron_b,            0.0),
		osg::Vec3(-icosahedron_a, -icosahedron_b,            0.0),
		osg::Vec3(           0.0,  icosahedron_a, -icosahedron_b),
		osg::Vec3(           0.0,  icosahedron_a,  icosahedron_b),
		osg::Vec3(           0.0, -icosahedron_a,  icosahedron_b),
		osg::Vec3(           0.0, -icosahedron_a, -icosahedron_b),
		osg::Vec3(-icosahedron_b,            0.0,  icosahedron_a),
		osg::Vec3( icosahedron_b,            0.0,  icosahedron_a),
		osg::Vec3( icosahedron_b,            0.0, -icosahedron_a),
		osg::Vec3(-icosahedron_b,            0.0, -icosahedron_a)
};

struct TopLevelTriangle
{
	int corners[3];   // index der Ecken
};

static const TopLevelTriangle icosahedron_triangles[20] =
{
  {{ 1, 0,10}}, //  1
  {{ 0, 1, 9}}, //  0
  {{ 0, 9, 6}}, // 12
  {{ 9, 8, 6}}, //  9
  {{ 0, 7,10}}, // 16
  {{ 6, 7, 0}}, //  6
  {{ 7, 6, 3}}, //  7
  {{ 6, 8, 3}}, // 14
  {{11,10, 7}}, // 11
  {{ 7, 3,11}}, // 18
  {{ 3, 2,11}}, //  3
  {{ 2, 3, 8}}, //  2
  {{10,11, 4}}, // 10
  {{ 2, 4,11}}, // 19
  {{ 5, 4, 2}}, //  5
  {{ 2, 8, 5}}, // 15
  {{ 4, 1,10}}, // 17
  {{ 4, 5, 1}}, //  4
  {{ 5, 9, 1}}, // 13
  {{ 8, 9, 5}}  //  8
};

GeodesicGrid::GeodesicGrid(const int lev) :
        m_cnMaxLevel(lev <= 0 ? 1 : lev)
{
    if (m_cnMaxLevel > 0)
	{
        triangles = new Triangle*[m_cnMaxLevel + 1];
		int nr_of_triangles = 20;
        for (int i = 0; i < m_cnMaxLevel; i++)
		{
			triangles[i] = new Triangle[nr_of_triangles];
			nr_of_triangles *= 4;
		}
		for (int i = 0; i < 20; i++)
		{
			const int * const corners = icosahedron_triangles[i].corners;
			initTriangle(0, i, icosahedron_corners[corners[0]],
					icosahedron_corners[corners[1]],
					icosahedron_corners[corners[2]]);
		}
	}
	else
	{
		triangles = 0;
	}
}

GeodesicGrid::~GeodesicGrid(void)
{
    if (m_cnMaxLevel > 0)
	{
        for (int i = m_cnMaxLevel - 1; i >= 0; i--)
			delete[] triangles[i];
		delete[] triangles;
	}
}

void GeodesicGrid::getTriangleCorners(int lev, int index, osg::Vec3 &h0,
		osg::Vec3 &h1, osg::Vec3 &h2) const
{
	if (lev <= 0)
	{
		const int * const corners = icosahedron_triangles[index].corners;
		h0 = icosahedron_corners[corners[0]];
		h1 = icosahedron_corners[corners[1]];
		h2 = icosahedron_corners[corners[2]];
	}
	else
	{
		lev--;
		const int i = index >> 2;
		Triangle &t(triangles[lev][i]);
		switch (index & 3)
		{
		case 0:
		{
			osg::Vec3 c0, c1, c2;
			getTriangleCorners(lev, i, c0, c1, c2);
			h0 = c0;
			h1 = t.e2;
			h2 = t.e1;
		}
			break;
		case 1:
		{
			osg::Vec3 c0, c1, c2;
			getTriangleCorners(lev, i, c0, c1, c2);
			h0 = t.e2;
			h1 = c1;
			h2 = t.e0;
		}
			break;
		case 2:
		{
			osg::Vec3 c0, c1, c2;
			getTriangleCorners(lev, i, c0, c1, c2);
			h0 = t.e1;
			h1 = t.e0;
			h2 = c2;
		}
			break;
		case 3:
			h0 = t.e0;
			h1 = t.e1;
			h2 = t.e2;
			break;
		}
	}
}

int GeodesicGrid::getPartnerTriangle(int lev, int index) const
{
	if (lev == 0)
	{
		assert(index<20);
		return (index & 1) ? index + 1 : index - 1;
	}
	switch (index & 7)
	{
	case 2:
	case 6:
		return index + 1;
	case 3:
	case 7:
		return index - 1;
	case 0:
		return (lev == 1) ?
				index + 5 : (getPartnerTriangle(lev - 1, index >> 2) << 2) + 1;
	case 1:
		return (lev == 1) ?
				index + 3 : (getPartnerTriangle(lev - 1, index >> 2) << 2) + 0;
	case 4:
		return (lev == 1) ?
				index - 3 : (getPartnerTriangle(lev - 1, index >> 2) << 2) + 1;
	case 5:
		return (lev == 1) ?
				index - 5 : (getPartnerTriangle(lev - 1, index >> 2) << 2) + 0;
	default:
		assert(0);
		break;
	}
	return 0;
}

void GeodesicGrid::initTriangle(int lev, int index, const osg::Vec3 &c0,
		const osg::Vec3 &c1, const osg::Vec3 &c2)
{
	assert((c0^c1)*c2 >= 0.0);
	Triangle &t(triangles[lev][index]);
	t.e0 = c1 + c2;
	t.e0.normalize();
	t.e1 = c2 + c0;
	t.e1.normalize();
	t.e2 = c0 + c1;
	t.e2.normalize();
	lev++;
    if (lev < m_cnMaxLevel)
	{
		index *= 4;
		initTriangle(lev, index + 0, c0, t.e2, t.e1);
		initTriangle(lev, index + 1, t.e2, c1, t.e0);
		initTriangle(lev, index + 2, t.e1, t.e0, c2);
		initTriangle(lev, index + 3, t.e0, t.e1, t.e2);
	}
}

void GeodesicGrid::visitTriangles(int maxVisitLevel, VisitFunc *func,
		void *context) const
{
	if (func && maxVisitLevel >= 0)
	{
        if (maxVisitLevel > m_cnMaxLevel)
            maxVisitLevel = m_cnMaxLevel;
		for (int i = 0; i < 20; i++)
		{
			const int * const corners = icosahedron_triangles[i].corners;
			visitTriangles(0, i, icosahedron_corners[corners[0]],
					icosahedron_corners[corners[1]],
					icosahedron_corners[corners[2]], maxVisitLevel, func,
					context);
		}
	}
}

void GeodesicGrid::visitTriangles(int maxVisitLevel, VisitFunc2 *func,
		void *context) const
{
	if (func && maxVisitLevel >= 0)
	{
        if (maxVisitLevel > m_cnMaxLevel)
            maxVisitLevel = m_cnMaxLevel;
		for (int i = 0; i < 20; i++)
		{
			const int * const corners = icosahedron_triangles[i].corners;
			visitTriangles(0, i, icosahedron_corners[corners[0]],
					icosahedron_corners[corners[1]],
					icosahedron_corners[corners[2]], maxVisitLevel, func,
					context, 2);
		}
	}
}

void GeodesicGrid::visitTriangles(int lev, unsigned int index,
		const osg::Vec3 &c0, const osg::Vec3 &c1, const osg::Vec3 &c2,
		int maxVisitLevel, VisitFunc *func, void *context) const
{
	if (!(*func)(lev, index, c0, c1, c2, context))
    {
        return;
    }

	Triangle &t(triangles[lev][index]);
	lev++;
	if (lev <= maxVisitLevel)
	{
		index *= 4;
		visitTriangles(lev, index + 0, c0, t.e2, t.e1, maxVisitLevel, func,
				context);
		visitTriangles(lev, index + 1, t.e2, c1, t.e0, maxVisitLevel, func,
				context);
		visitTriangles(lev, index + 2, t.e1, t.e0, c2, maxVisitLevel, func,
				context);
		visitTriangles(lev, index + 3, t.e0, t.e1, t.e2, maxVisitLevel, func,
				context);
	}
}

void GeodesicGrid::visitTriangles(int lev, unsigned int index,
		const osg::Vec3 &c0, const osg::Vec3 &c1, const osg::Vec3 &c2,
		int maxVisitLevel, VisitFunc2 *func, void *context, int value) const
{
	value = (*func)(lev, index, c0, c1, c2, context, value);
	if (value == 0)
    {
        return;
    }

	Triangle &t(triangles[lev][index]);
	lev++;
	if (lev <= maxVisitLevel)
	{
		index *= 4;
		visitTriangles(lev, index + 0, c0, t.e2, t.e1, maxVisitLevel, func,
				context, value);
		visitTriangles(lev, index + 1, t.e2, c1, t.e0, maxVisitLevel, func,
				context, value);
		visitTriangles(lev, index + 2, t.e1, t.e0, c2, maxVisitLevel, func,
				context, value);
		visitTriangles(lev, index + 3, t.e0, t.e1, t.e2, maxVisitLevel, func,
				context, value);
	}
}
