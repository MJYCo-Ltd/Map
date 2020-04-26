/*
 ===============================================================================
                               FILE : Frustum.cpp
                               DATE : 2013-1-16
                              AUTHO : MA ZHIHAO
 ===============================================================================

               Copyright (c) 2013 by Visual Simulation Group
                        Dep. Space Command, Academy of Equipment.

   No part of this source code may be reproduced or distributed in any form
   or by any means, or stored in a database or retrieval system, without the
   prior written consent of Academy of Equipment.
 ===============================================================================
*/

/** @file
 * SpaceAnt Library file: 
 */

#include "Frustum.h"

static inline void __InvertMatrix(double m00, double m01, double m02,
		double m10, double m11, double m12, double m20, double m21, double m22,
		double m[3][3])
{
	m[0][0] = m11 * m22 - m12 * m21;
	m[0][1] = m02 * m21 - m01 * m22;
	m[0][2] = m01 * m12 - m02 * m11;
	m[1][0] = m12 * m20 - m10 * m22;
	m[1][1] = m00 * m22 - m02 * m20;
	m[1][2] = m02 * m10 - m00 * m12;
	m[2][0] = m10 * m21 - m11 * m20;
	m[2][1] = m01 * m20 - m00 * m21;
	m[2][2] = m00 * m11 - m01 * m10;
	double s = 1.0 / (m00 * m[0][0] + m01 * m[1][0] + m02 * m[2][0]);
	m[0][0] *= s;
	m[0][1] *= s;
	m[0][2] *= s;
	m[1][0] *= s;
	m[1][1] *= s;
	m[1][2] *= s;
	m[2][0] *= s;
	m[2][1] *= s;
	m[2][2] *= s;
}

osg::Vec3 GetEyePosition(const osg::Matrix &modelView)
{
	static double m[3][3];
	__InvertMatrix(modelView(0, 0), modelView(0, 1), modelView(0, 2),
			modelView(1, 0), modelView(1, 1), modelView(1, 2), modelView(2, 0),
			modelView(2, 1), modelView(2, 2), m);
//	ReportNotify("%lf, %lf, %lf")
	return osg::Vec3(
			-(m[0][0] * modelView(3, 0) + m[1][0] * modelView(3, 1)
					+ m[2][0] * modelView(3, 2)),
			-(m[0][1] * modelView(3, 0) + m[1][1] * modelView(3, 1)
					+ m[2][1] * modelView(3, 2)),
			-(m[0][2] * modelView(3, 0) + m[1][2] * modelView(3, 1)
					+ m[2][2] * modelView(3, 2)));
}
