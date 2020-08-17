/** @file
 * SpaceAnt Library file: Frustum functions
 */


#ifndef SA_FRUSTUM_H_
#define SA_FRUSTUM_H_

#include <osg/Vec3>
#include <osg/Matrix>

#include <stdlib.h>

/// Returns the normal vector of two vectors (the normalized cross product)
inline osg::Vec3 NormalVector(const osg::Vec3 &v1, const osg::Vec3 &v2)
{
	osg::Vec3 v = v1 ^ v2;
	v.normalize();
	return v;
}

/// Returns the normal vector of a triangle or 3 points on a plane (assumes
/// counter-clockwise orientation)
inline osg::Vec3 NormalVector(const osg::Vec3 &p1, const osg::Vec3 &p2,
		const osg::Vec3 &p3)
{
	return NormalVector(p2 - p1, p3 - p1);
}

/// The plane class
class Plane
{
public:
	// The plane is represented as Dot(N,X) = c where N is the plane normal
	// vector, not necessarily unit length, c is the plane constant, and X is
	// any point on the plane.
	osg::Vec3 m_vNormal;
	float m_fConstant;

public:
	Plane()
	{}
	Plane(const osg::Vec3 &p1, const osg::Vec3 &p2, const osg::Vec3 &p3)
	{
		m_vNormal = NormalVector(p1, p2, p3);
		m_fConstant = -(p1 * m_vNormal);
	}
	Plane(const osg::Vec3 &vNormal, const osg::Vec3 &p)
	{
		m_vNormal = vNormal;
		m_fConstant = -(p * m_vNormal);
	}
	Plane(const osg::Vec3 &vNormal, const float f)
	{
		m_vNormal = vNormal;
		m_fConstant = f;
	}

	void init(osg::Vec3 &p1, osg::Vec3 &p2, osg::Vec3 &p3)
	{	// Initializes the plane based on three points in the plane
		osg::Vec3f n = NormalVector(p1, p2, p3);
		init(n, p1);
	}

	/// Initializes the plane based on a normal and a point in the plane
	void init(osg::Vec3 &vNormal, osg::Vec3 &p)
	{
		m_vNormal = vNormal;
		m_fConstant = -(p * m_vNormal);
	}

	/**
	 * Classify the given vector with regards to this plane. If the plane
	 * is expressed as (N,D) with N the A,B,C components of the plane then
	 * this will calculate and return N%pt+D.
	 */
	inline float classify(const osg::Vec3& pt) const
	{
		return m_vNormal * pt + m_fConstant;
	}

	/**
	 * This static function classifies a vector with regards to four given plane
	 * components. This will calculate and return A*pt.x+B*pt.y+C*pt.z+D.
	 */
	static float classify(float A, float B, float C, float D,
			const osg::Vec3& pt)
	{
		return A * pt.x() + B * pt.y() + C * pt.z() + D;
	}

	/// Returns the distance between the plane and point p
	float getDistance(const osg::Vec3 &p) const
	{
		return (m_vNormal * p) + m_fConstant;
	}

	/// Returns true if the line intersects the plane and changes vPos to the
	/// location of the intersection
	bool intersection(osg::Vec3 &vPos, osg::Vec3 &vDir)
	{
		float f = m_vNormal * vDir;
		if (abs(f) < 1e-6)
			return false;
		vPos -= vDir * (getDistance(vPos) / f);
		return true;
	}
};

/// A frustum build from OpenGL matrix.
class Frustum
{
protected:
	/// The six clip planes.
	Plane m_plFrustum[6];

public:
	enum
	{
		Out = 0, In = 1, PartIn = 2
	};
	enum
	{
		Right = 0, Left = 1, Bottom = 2, Top = 3, Far = 4, Near = 5
	};

public:
	/// Constructor.
	Frustum()
	{
	}

	/// Update the frustum
	void update(const osg::Matrix &modelview, const osg::Matrix &projection)
	{
		static float clip[16];
		static float t;
		/* Combine the two matrices (multiply projection by modelview) */
		clip[0] = modelview(0, 0) * projection(0, 0)
				+ modelview(0, 1) * projection(1, 0)
				+ modelview(0, 2) * projection(2, 0)
				+ modelview(0, 3) * projection(3, 0);
		clip[1] = modelview(0, 0) * projection(0, 1)
				+ modelview(0, 1) * projection(1, 1)
				+ modelview(0, 2) * projection(2, 1)
				+ modelview(0, 3) * projection(3, 1);
		clip[2] = modelview(0, 0) * projection(0, 2)
				+ modelview(0, 1) * projection(1, 2)
				+ modelview(0, 2) * projection(2, 2)
				+ modelview(0, 3) * projection(3, 2);
		clip[3] = modelview(0, 0) * projection(0, 3)
				+ modelview(0, 1) * projection(1, 3)
				+ modelview(0, 2) * projection(2, 3)
				+ modelview(0, 3) * projection(3, 3);

		clip[4] = modelview(1, 0) * projection(0, 0)
				+ modelview(1, 1) * projection(1, 0)
				+ modelview(1, 2) * projection(2, 0)
				+ modelview(1, 3) * projection(3, 0);
		clip[5] = modelview(1, 0) * projection(0, 1)
				+ modelview(1, 1) * projection(1, 1)
				+ modelview(1, 2) * projection(2, 1)
				+ modelview(1, 3) * projection(3, 1);
		clip[6] = modelview(1, 0) * projection(0, 2)
				+ modelview(1, 1) * projection(1, 2)
				+ modelview(1, 2) * projection(2, 2)
				+ modelview(1, 3) * projection(3, 2);
		clip[7] = modelview(1, 0) * projection(0, 3)
				+ modelview(1, 1) * projection(1, 3)
				+ modelview(1, 2) * projection(2, 3)
				+ modelview(1, 3) * projection(3, 3);

		clip[8] = modelview(2, 0) * projection(0, 0)
				+ modelview(2, 1) * projection(1, 0)
				+ modelview(2, 2) * projection(2, 0)
				+ modelview(2, 3) * projection(3, 0);
		clip[9] = modelview(2, 0) * projection(0, 1)
				+ modelview(2, 1) * projection(1, 1)
				+ modelview(2, 2) * projection(2, 1)
				+ modelview(2, 3) * projection(3, 1);
		clip[10] = modelview(2, 0) * projection(0, 2)
				+ modelview(2, 1) * projection(1, 2)
				+ modelview(2, 2) * projection(2, 2)
				+ modelview(2, 3) * projection(3, 2);
		clip[11] = modelview(2, 0) * projection(0, 3)
				+ modelview(2, 1) * projection(1, 3)
				+ modelview(2, 2) * projection(2, 3)
				+ modelview(2, 3) * projection(3, 3);

		clip[12] = modelview(3, 0) * projection(0, 0)
				+ modelview(3, 1) * projection(1, 0)
				+ modelview(3, 2) * projection(2, 0)
				+ modelview(3, 3) * projection(3, 0);
		clip[13] = modelview(3, 0) * projection(0, 1)
				+ modelview(3, 1) * projection(1, 1)
				+ modelview(3, 2) * projection(2, 1)
				+ modelview(3, 3) * projection(3, 1);
		clip[14] = modelview(3, 0) * projection(0, 2)
				+ modelview(3, 1) * projection(1, 2)
				+ modelview(3, 2) * projection(2, 2)
				+ modelview(3, 3) * projection(3, 2);
		clip[15] = modelview(3, 0) * projection(0, 3)
				+ modelview(3, 1) * projection(1, 3)
				+ modelview(3, 2) * projection(2, 3)
				+ modelview(3, 3) * projection(3, 3);
		/* Extract the numbers for the RIGHT plane */
		m_plFrustum[0].m_vNormal.x() = clip[3] - clip[0];
		m_plFrustum[0].m_vNormal.y() = clip[7] - clip[4];
		m_plFrustum[0].m_vNormal.z() = clip[11] - clip[8];
		m_plFrustum[0].m_fConstant = clip[15] - clip[12];
		t = m_plFrustum[0].m_vNormal.length();
		m_plFrustum[0].m_vNormal /= t;
		m_plFrustum[0].m_fConstant /= t;

		/* Extract the numbers for the LEFT plane */
		m_plFrustum[1].m_vNormal.x() = clip[3] + clip[0];
		m_plFrustum[1].m_vNormal.y() = clip[7] + clip[4];
		m_plFrustum[1].m_vNormal.z() = clip[11] + clip[8];
		m_plFrustum[1].m_fConstant = clip[15] + clip[12];
		t = m_plFrustum[1].m_vNormal.length();
		m_plFrustum[1].m_vNormal /= t;
		m_plFrustum[1].m_fConstant /= t;

		/* Extract the BOTTOM plane */
		m_plFrustum[2].m_vNormal.x() = clip[3] + clip[1];
		m_plFrustum[2].m_vNormal.y() = clip[7] + clip[5];
		m_plFrustum[2].m_vNormal.z() = clip[11] + clip[9];
		m_plFrustum[2].m_fConstant = clip[15] + clip[13];
		t = m_plFrustum[2].m_vNormal.length();
		m_plFrustum[2].m_vNormal /= t;
		m_plFrustum[2].m_fConstant /= t;

		/* Extract the TOP plane */
		m_plFrustum[3].m_vNormal.x() = clip[3] - clip[1];
		m_plFrustum[3].m_vNormal.y() = clip[7] - clip[5];
		m_plFrustum[3].m_vNormal.z() = clip[11] - clip[9];
		m_plFrustum[3].m_fConstant = clip[15] - clip[13];
		t = m_plFrustum[3].m_vNormal.length();
		m_plFrustum[3].m_vNormal /= t;
		m_plFrustum[3].m_fConstant /= t;

		/* Extract the FAR plane */
		m_plFrustum[4].m_vNormal.x() = clip[3] - clip[2];
		m_plFrustum[4].m_vNormal.y() = clip[7] - clip[6];
		m_plFrustum[4].m_vNormal.z() = clip[11] - clip[10];
		m_plFrustum[4].m_fConstant = clip[15] - clip[14];
		t = m_plFrustum[4].m_vNormal.length();
		m_plFrustum[4].m_vNormal /= t;
		m_plFrustum[4].m_fConstant /= t;

		/* Extract the NEAR plane */
		m_plFrustum[5].m_vNormal.x() = clip[3] + clip[2];
		m_plFrustum[5].m_vNormal.y() = clip[7] + clip[6];
		m_plFrustum[5].m_vNormal.z() = clip[11] + clip[10];
		m_plFrustum[5].m_fConstant = clip[15] + clip[14];
		t = m_plFrustum[5].m_vNormal.length();
		m_plFrustum[5].m_vNormal /= t;
		m_plFrustum[5].m_fConstant /= t;
	}

	bool isInFrustum(const osg::Vec3 &vPos, const float fRadius) const
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_plFrustum[i].getDistance(vPos) < -fRadius)
				return false;
		}
		return true;
	}

	/// Check the status of given sphere.
	int checkFrustum(const osg::Vec3 &pos, float radius) const
	{
		static float clfy;
		for (int i = 0; i < 4; i++)
		{
			clfy = m_plFrustum[i].classify(pos);
			if (clfy < -radius)
				return Out;
			else if (clfy < radius)
				return PartIn;
		}
		return In;
	}
	/// Check the status of given triangle.
	int checkFrustum(const osg::Vec3 &v1, const osg::Vec3 &v2,
			const osg::Vec3 &v3) const
	{
		osg::Vec3 pos = (v1 + v2 + v3) / 3.0f;
		return checkFrustum(pos, (v1 - pos).length());
	}
};

#endif /* SA_FRUSTUM_H_ */
