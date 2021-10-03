#pragma once
/*!
@file       Gaia.h
@date       2021.09.12
@author     Lucky
@version    v0.1
@brief		Mathmathics class Header file
@details

*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include "gaia_global.h"

#include <QLineF>
#include <QPointF>

///////////////////////////////////////////////////////////////////////////////
// Mathmathics class for Gaia
// Using Units
// distance, height : km
// angle : degree
// position : km
///////////////////////////////////////////////////////////////////////////////

namespace Gaia
{
#define X2(x)	(x)*(x)
#define SIN(deg)	sin((deg) * D2R)
#define	COS(deg)	cos((deg) * D2R)
#define TAN(deg)	tan((deg) * D2R)
#define ATAN(x)		(atan(x) * R2D)
#define ASIN(x)		(asin(x) * R2D)

	static const double PI	= 3.14159265358979323846264338327950288419716939937510;
	static const double D2R	= ( PI / 180.0 );
	static const double R2D = 1.0/D2R;
	static const double E_RADIUS = 	6378.137;
	static const double GEO_HEIGHT = 	35786.0;
	static const double FLATTENING = 1.0 / 298.257223563;

	/*!
	@brief Basic Math class
	*/
	class GAIA_EXPORT GaiaMath
	{
	public:
		/*
		@brief Calculate the center angle(degrees)
		*/
		static double angleBetween(const QPointF& p1, const QPointF& center, const QPointF& p2);
		static double angleWithXAxis(const QLineF& line);

	};
	   
};
