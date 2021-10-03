#include <math.h>

#include <QVector2D>
#include <QLineF>
#include <QPointF>

#include <QMatrix4x4>
#include "MapMath.h"

using namespace Gaia;

void MapMath::LLA2XYZ( double lon, double lat, double pressureAlt, double& x, double& y, double& z )
{

	double sinph = sin( lat * D2R );
	double cosph = cos( lat * D2R );

	double N = E_RADIUS / sqrt( 1.0 - FLATTENING * ( 2.0 - FLATTENING ) * sinph * sinph );

	x = ( N + pressureAlt ) * cosph * cos( lon * D2R );
	y = ( N + pressureAlt ) * cosph * sin( lon * D2R );
	z = ( ( 1.0 - FLATTENING ) * ( 1.0 - FLATTENING ) * N + pressureAlt ) * sinph;		// 지표면
}

void MapMath::XYZ2LL( double x, double y, double z, double& lon, double&lat, double& alt )
{
	double _a = E_RADIUS;
	double _b = _a - (FLATTENING*_a);

	double	a2 = _a*_a;				// _a^2
	double	b2 = _b*_b;				// _b^2
	double	z2 = z*z;				// Z^2

	//double	f = flattening();
	double	e2 = 2.0 * FLATTENING - FLATTENING*FLATTENING;									// first eccentricity squared
	double	ep2 = FLATTENING * ( 2.0 - FLATTENING ) / ((1.0 - FLATTENING)*( 1.0 - FLATTENING ));	// second eccentricity squared

	double	r2 = x*x + y*y;
	double	r = sqrt(r2);
	double	E2 = a2 - b2;
	double	F = 54.0 * b2 * z2;
	double	G = r2 + ( 1.0 - e2 ) * z2 - e2 * E2;
	double	c = ( e2*e2 * F * r2 ) / ( G*G*G );
	double	s = pow( 1.0 + c + sqrt( c*c + 2.0 * c ), 1.0 / 3.0 );
	double	P = F / ( 3.0 * pow( s + 1.0 / s + 1.0, 2.0 ) * G*G );
	double	Q = sqrt( 1.0 + 2.0 * e2*e2 * P );
	double	ro = -( e2 * P * r ) / (1.0 + Q) + sqrt(( a2 / 2.0 ) * (1.0 + 1.0 / Q ) - ((1.0 - e2 ) * P * z2) / (Q * (1.0 + Q)) - P * r2 / 2.0 );
	double	tmp = pow( r - e2 * ro, 2.0 );
	double	U = sqrt( tmp + z2 );
	double	V = sqrt( tmp + ( 1.0 - e2 ) * z2 );
	double	zo = ( b2 * z ) / ( _a * V );

	lat = atan( ( z + ep2 * zo ) / r ) * R2D;			// geodetic latitude
	lon = atan2(y,x) * R2D;							// geodetic longitude
	alt = U * ( 1.0 - b2 / ( _a * V ) );		// altitude

	//return lla;
	//double  len     = sqrt( x*x + y*y );

	//lat            = atan2( z, len )*r2d;
	//lon            = atan2( y, x )*r2d;
}

void MapMath::AzElE2N( double lon1, double lat1, double alt1, double lon2, double lat2, double alt2, double* azim, double* elev )
{
	double dlon, dlat;
	dlon = fabs(lon1 - lon2);
	dlat = fabs(lat1 - lat2);

	*azim = atan(((2*E_RADIUS)*dlat)/(E_RADIUS*dlon*fabs((COS(lat1)+COS(lat2))))) * R2D;

	if((lon1 <= lon2) && (lat1 <= lat2))
	{
		*azim = 90 - *azim;
	}
	else if((lon1 > lon2) && (lat1 <= lat2))
	{
		*azim = 270 + *azim;
	}
	else if((lon1 > lon2) && (lat1 > lat2))
	{
		*azim = 270 - *azim;
	}
	else
	{
		*azim = 90 + *azim;		// 방위각
	}

	alt1/=1000.0;
	alt2/=1000.0;	// m 로 변환

	double x1 = (E_RADIUS+alt1) * COS( lat1 ) * COS( lon1 );
	double y1;
	if( lon1 >= 0.0 )
		y1 = (E_RADIUS+alt1) * COS( lat1 ) * COS( 90.0 - lon1 );
	else
		y1 = -1.0 * (E_RADIUS+alt1) * COS( lat1 ) * COS( 90.0 + lon1 );
	double z1 = (E_RADIUS+alt1) * SIN( lat1 );


	double x2 = (E_RADIUS+alt2) * COS( lat2 ) * COS( lon2 );
	double y2;
	if( lon1 >= 0.0 )
		y2 = (E_RADIUS+alt2) * COS( lat2 ) * COS( 90.0 - lon2 );
	else
		y2 = -1.0 * (E_RADIUS+alt2) * COS( lat2 ) * COS( 90.0 + lon2 );
	double z2 = (E_RADIUS+alt2) * SIN( lat2 );

	//	무선국 1에 대한 방향벡터(지구면과 접하는 평면의 법선벡터)

	//	무선국 1에서 무선국 2에 대한 방향벡터
	double vectorX = x2 - x1;
	double vectorY = y2 - y1;
	double vectorZ = z2 - z1;

	//	평면과 1->2에 대한 방향벡터의 사이각 중 (예각)
	double cosElev = (x1*vectorX + y1*vectorY + z1*vectorZ)/sqrt(x1*x1 + y1*y1 + z1*z1)/sqrt(vectorX*vectorX + vectorY*vectorY + vectorZ*vectorZ);
	double compleAng = acos(cosElev) * R2D;
	if (compleAng < 0.0)
	{
		compleAng = compleAng * (-1.0);
	}
	*elev = 90.0 - compleAng;
	//	elev가 음수가 나오지 않는다면 검토할 필요 있음.
}

bool MapMath::SplitAt180Degree( const QPointF& p1, const QPointF& p2, QPointF& pa, QPointF& pb )
{
	if( fabs(p1.x() - p2.x() ) > 181.0 )
	{
		if(p1.x() > 0 ) 
		{
			double d = (p2.x() + 360.0) - p1.x();
			double d1 = 180.0 - p1.x();
			double dy = p2.y() - p1.y();
			double y = p1.y() + dy * d1/d;
			pa.setX(180.0);
			pa.setY(y);
			pb.setX(-180.0);
			pb.setY(y);
		}
		else
		{
			double d = (p1.x() + 360.0) - p2.x();
			double d1 = 180.0 + p1.x();
			double dy = p2.y() - p1.y();
			double y = p1.y() + dy * d1/d;
			pa.setX(-180.0);
			pa.setY(y);
			pb.setX(180.0);
			pb.setY(y);
		}
		return true;
	}
	else return false;
}

bool MapMath::MakeInterpolatedLine( const QList<QPointF>& plist, int inter, QVector<QPointF>& rst )
{
	QPointF dp = plist[inter] - plist[inter+1];

	int cnt = dp.manhattanLength() / 30;
	if(cnt < 1 ) return false;

	double dt = 1.0/(cnt+1);
	rst.reserve(cnt);

	QVector<double> tlist;
	QVector<double> xlist;
	QVector<double> ylist;
	int listCount = plist.count();
	for(int a = 0; a < listCount; a++ ) 
	{
		tlist << a;
		xlist << plist[a].x();
		ylist << plist[a].y();
	}
	Interpolation xp;
	Interpolation yp;
	xp.set(listCount, tlist.data(), xlist.data());
	yp.set(listCount, tlist.data(), ylist.data());

	double t = inter + dt;
	for( int i = 0; i < cnt; i++, t += dt )
	{
		rst << QPointF(xp.get(t), yp.get(t));
	}

	return true;
}

double MapMath::DistanceBetweenXYZ( double x1, double y1, double z1, double x2, double y2, double z2 )
{
	return sqrt(X2(x1 - x2) + X2(y1 - y2) + X2(z1 - z2));
}

void MapMath::calcGeoVisibleArea(double geoLongitude, QList<QPolygonF>& areas)
{
	areas.clear();

	double phi = geoLongitude*D2R;
	double theta = asin(E_RADIUS/(E_RADIUS+GEO_HEIGHT));

	double x = E_RADIUS * sin(theta)*cos(phi);
	double y = E_RADIUS * sin(theta)*sin(phi);
	double z = E_RADIUS * cos(theta);
	QVector3D iv(x, y, z);
	QVector3D axis(cos(phi), sin(phi), 0.0);

	QPolygonF polygon;
	for( int a = 0; a <= 360; a+=2 )
	{
		QQuaternion rotAxis = QQuaternion::fromAxisAndAngle (axis, a);
		QVector3D v = rotAxis.rotatedVector(iv);
		double lon, lat, alt;
		MapMath::XYZ2LL (v.x(), v.y(), v.z(), lon, lat, alt);
		polygon << QPointF(lon, lat);
	}

	areas << polygon;

	return;

	//double R = E_RADIUS;
	//double oX, oY, oZ;
	//MapMath::LLA2XYZ (geoLongitude, 0, GEO_HEIGHT, oX, oY, oZ);

	//double length = E_RADIUS + GEO_HEIGHT;

	//QMatrix4x4 m;
	//m.translate(oX, oY, oZ);
	//m.rotate (geoLongitude, 0, 0, 1);
	////m.rotate(0, 0, 1, 0);

	//double angle = asin(R/length) - 0.00000001;
	//double ini = length * tan(angle);
	//QVector3D iv(-length, ini, 0);
	//double slant = length / cos(angle);// 빔의 slant 길이

	//double prevLon = -999.9;
	//QPolygonF polygon;
	//for (int k = 0; k <= 360; k += 2)
	//{
	//	QQuaternion xaxis = QQuaternion::fromAxisAndAngle (-1, 0, 0, k);

	//	QVector3D v = xaxis.rotatedVector(iv);
	//	v = m.map (v);	// map to satellite position
	//	double p[3] = {v.x(), v.y(), v.z()};

	//	double dx = p[0] - oX;
	//	double dy = p[1] - oY;
	//	double dz = p[2] - oZ;

	//	bool signx = (dx > 0);
	//	bool signy = (dy > 0);
	//	bool signz = (dz > 0);

	//	double t1, t2;
	//	double a = dx*dx + dy*dy + dz*dz;
	//	double b = 2.0*(dx*oX + dy*oY + dz*oZ);
	//	double c = oX*oX + oY*oY + oZ*oZ - R*R;
	//	double b24ac = b*b - 4*a*c;// b^2-4ac
	//	double lon, lat, alt;
	//	//if (b24ac < 0)	// 지표와 교점이 없는 경우
	//	//{
	//	//	//memcpy(_3DCntrVtx[i].pointer(), p, sizeof(double)*3);
	//	//	continue;
	//	//}
	//	//else	// 지표와 만나는 경우
	//	{
	//		double temp = sqrt(b24ac);
	//		t1 = (-b - temp) / a / 2;
	//		//t2 = (-b - temp) / a / 2;
	//	}

	//	double x1 = oX + t1 * dx;
	//	double y1 = oY + t1 * dy;
	//	double z1 = oZ + t1 * dz;

	//	//double x2 = oX + t2 * dx;
	//	//double y2 = oY + t2 * dy;
	//	//double z2 = oZ + t2 * dz;

	//	double d1 = MapMath::DistanceBetweenXYZ(oX, oY, oZ, x1, y1, z1);
	//	//double d2 = MapMath::DistanceBetweenXYZ(oX, oY, oZ, x2, y2, z2);

	//	double xt, yt, zt;
	//	double d = 0.0;
	//	//if (d1 < d2)	// 지표와 2점 이상 만날 경우 가까운 점으로 처리한다.
	//	//{
	//		xt = x1;
	//		yt = y1;
	//		zt = z1;
	//		d = d1;
	//	//}
	//	//else 
	//	//{
	//	//	xt = x2;
	//	//	yt = y2;
	//	//	zt = z2;
	//	//	d = d2;
	//	//}

	//	//// 반대 방향으로 만나는 경우
	//	//if( (xt - oX >= 0) == !signx && (yt - oY >= 0) == !signy && (zt - oZ >= 0) == !signz )
	//	//{
	//	//	MapMath::XYZ2LL (p[0], p[1], p[2], lon, lat, alt);
	//	//	//memcpy(_3DCntrVtx[i].pointer(), p, sizeof(double)*3);
	//	//}
	//	//else // 지향 방향과 같은 방향으로 지표와 만나는 경우
	//	//{
	//		//if( d <= slant || sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]) <= R )
	//		//{
	//			MapMath::XYZ2LL (xt, yt, zt, lon, lat, alt);
	//			//if( lon < 0 ) lon += 360.0;
	//			//_3DCntrVtx[i].x = xt;
	//			//_3DCntrVtx[i].y = yt;
	//			//_3DCntrVtx[i].z = zt;
	//		//}
	//		//else
	//		//{
	//		//	MapMath::XYZ2LL (p[0], p[1], p[2], lon, lat, alt);
	//		//	//memcpy(_3DCntrVtx[i].pointer(), p, sizeof(double)*3);
	//		//}
	//	//}

	//	if( prevLon < -180 ) prevLon = lon;

	//	QPointF pf(lon, lat);
	//	if( fabs(prevLon - lon) > 180 )
	//	{
	//		QPointF pa, pb;
	//		MapMath::SplitAt180Degree(polygon.last(), pf, pa, pb);
	//		polygon << pa;
	//		areas << polygon;
	//		polygon.clear();
	//		polygon << pb;
	//	}

	//	polygon << pf;
	//	prevLon = lon;
	//}
	//areas << polygon;
}

double MapMath::AngleBetween(const QPointF & p1, const QPointF & center, const QPointF & p2)
{
	QVector2D v1(p1 - center);
	QVector2D v2(p2 - center);

	double v = QVector2D::dotProduct(v1, v2) / (v1.length()*v2.length());
	if (v >= 1.0) return 0.0;
	else if (v <= -1.0) return 180.0;
	double a = acos(v);
	a = a * R2D;
	return a;
}

// Angle with X-axis
double MapMath::Angle(const QLineF & line)
{
	QPointF xaxis(line.p1().x() + 1.0, line.p1().y());
	double a = AngleBetween(line.p2(), line.p1(), xaxis);
	return a;
}
