#pragma once
#include "gaia_global.h"

#include <QList>
#include <QVector>
#include <QPointF>
#include <QPolygonF>

///////////////////////////////////////////////////////////////////////////////
// Mathmathics class for map
// using units
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

	class GAIA_EXPORT MapMath
	{
	public:
		static void LLA2XYZ( double lon, double lat, double pressureAlt, double& x, double& y, double& z );		//	LLA => XYZ
		static void XYZ2LL( double x, double y, double z, double& lon, double&lat, double& alt);
		static void AzElE2N( double lon1, double lat1, double alt1, double lon2, double lat2, double alt2, double* azim, double* elev );

		static bool SplitAt180Degree(const QPointF& p1, const QPointF& p2, QPointF& pa, QPointF& pb) ;
		static bool MakeInterpolatedLine(const QList<QPointF>& plist, int inter, QVector<QPointF>& rst);

		static double DistanceBetweenXYZ(double x1, double y1, double z1, double x2, double y2, double z2);
		static void calcGeoVisibleArea(double geoLongitude, QList<QPolygonF>& areas);

		static double AngleBetween(const QPointF& p1, const QPointF& center, const QPointF& p2);
		static double Angle(const QLineF& line);
	};


	class Interpolation 
	{
	public :
		void set(int count, double* xlist, double* ylist)
		{
			_count = count;
			_x = xlist;
			_y = ylist;
		};

		double get(double p) const
		{
			double rst = 0;
			for(int i = 0;i < _count; i++)
			{
				double temp = 1;
				int k = i;
				for(int j=0;j < _count; j++)
				{
					if(k==j)
					{
						continue;
					}
					else
					{
						temp = temp * ((p-_x[j])/(_x[k]-_x[j]));
					}
				}
				temp = _y[i]*temp;
				rst += temp;
			}

			return rst;
		}

	private:
		double* _x;
		double* _y;
		int		_count;
	};

};
