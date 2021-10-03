/*!
	@file       GeoMath.h
	@date       2018/04/21
	@author     Lucky Kim(lucky@moonsoftinc.com)
	@version    1.1
    @brief      지리수학 함수
	@section	Details
	관련 수학함수를 구현한다.
*/
#include <math.h>
#include <memory.h>
#include <stdlib.h>

#include "GeoMath.h"
using namespace MOON;

/*!
	@brief	경위도 좌표를 XYZ 좌표로 변환한다.
	@param	ll	경위도 좌표
	@param	pos	XYZ 좌표 변환값
*/
void GeoMath::LL2XYZ( const double* ll, double* pos )
{
	pos[0]	= ll[2] * cos(ll[1]*D2R) * cos(ll[0]*D2R);
	pos[1]	= ll[2] * cos(ll[1]*D2R) * sin(ll[0]*D2R);
	pos[2]	= ll[2] * sin(ll[1]*D2R);
}

/*!
	@brief	경위도 좌표를 XYZ 좌표로 변환한다.
	@param	lon 경도(degree)
	@param	lat 위도(degree)
	@param	alt	고도(km)
	@param	pos	XYZ 좌표 변환값
	@overload 
*/
void GeoMath::LL2XYZ( double lon, double lat, double alt, double* pos )
{
	pos[0]	= alt * cos(lat*D2R) * cos(lon*D2R);
	pos[1]	= alt * cos(lat*D2R) * sin(lon*D2R);
	pos[2]	= alt * sin(lat*D2R);
}

/*!
	@brief	경위도 좌표를 XYZ 좌표로 변환한다.
	@param	lon 경도(degree)
	@param	lat 위도(degree)
	@param	alt	고도(km)
	@param	x 변환값
	@param	y 변환값
	@param	z 변환값
	@overload 
*/
void GeoMath::LL2XYZ( double lon, double lat, double alt, double& x, double& y, double& z )
{
	x = alt * cos(lat*D2R) * cos(lon*D2R);
	y = alt * cos(lat*D2R) * sin(lon*D2R);
	z = alt * sin(lat*D2R);
}

/*!
	@brief	경위도 좌표를 XYZ 좌표로 변환한다.
	@param	lon 경도(degree)
	@param	lat 위도(degree)
	@param	alt	고도(km)
	@param	pos	XYZ 좌표 변환값
	@overload 
*/
void GeoMath::LL2XYZ( double lon, double lat, double alt, float* pos )
{
	pos[0]	= alt * cos(lat*D2R) * cos(lon*D2R);
	pos[1]	= alt * cos(lat*D2R) * sin(lon*D2R);
	pos[2]	= alt * sin(lat*D2R);
}

QVector3D GeoMath::LL2XYZ(double lon, double lat, double alt)
{
	double x, y, z;
	LL2XYZ(lon, lat, alt, x, y, z);
	return QVector3D(x, y, z);
}

QVector3D MOON::GeoMath::LL2XYZ(const QVector3D & lla)
{
	double x, y, z;
	LL2XYZ(lla.x(), lla.y(), lla.z(), x, y, z);
	return QVector3D(x, y, z);
}

/*!
	@brief	경위도 좌표를 Normalized XYZ 좌표로 변환한다.
	@param	ll	경위도 좌표
	@param	pos	Normalized XYZ 좌표 변환값
*/
void GeoMath::LL2Norm( const double* ll, double* pos )
{
	pos[0]	= cos(ll[1]*D2R) * cos(ll[0]*D2R);
	pos[1]	= cos(ll[1]*D2R) * sin(ll[0]*D2R);
	pos[2]	= sin(ll[1]*D2R);
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	x X좌표
	@param	y Y좌표
	@param	z Z좌표
	@param	lon	경도 변환값
	@param	lat	위도 변환값
	@param	alt	고도 변환값
*/
void GeoMath::XYZ2LL( double x, double y, double z, double& lon, double& lat, double& alt)
{
	alt = sqrt(x*x + y*y + z*z);
	if( alt == 0 )
	{
		lon = 0; lat = 0; alt = 0;
		return;
	}

	lat = asin(z/alt);

	double temp = y/alt/cos(lat);
	if( temp >= 1.0 ) lon = 90.0;
	else if( temp <= -1.0 ) lon = -90.0;
	else lon = asin(temp) / D2R;

	if( x < 0 )
	{
		y > 0 ? (lon = 180.0 - lon) : (lon = -180 - lon);
	}
	lat /= D2R;
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	xyz XYZ 좌표
	@param	lon	경도 변환값
	@param	lat	위도 변환값
	@param	alt	고도 변환값
	@overload 
*/
void GeoMath::XYZ2LL( const float* xyz, double& lon, double& lat, double& alt)
{
	XYZ2LL((double)xyz[0], (double)xyz[1], (double)xyz[2], lon, lat, alt);
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	x X좌표
	@param	y Y좌표
	@param	z Z좌표
	@param	lon	경도 변환값
	@param	lat	위도 변환값
	@param	alt	고도 변환값
	@overload 
*/
void GeoMath::XYZ2LL( float x, float y, float z, double& lon, double& lat, double& alt)
{
	XYZ2LL( (double)x, (double)y, (double)z, lon, lat, alt);
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	xyz XYZ 좌표
	@param	lla 경위고도 변환값
	@overload 
*/
void GeoMath::XYZ2LL( const float* xyz, float*lla)
{
	double lon, lat, alt;
	XYZ2LL((double)xyz[0], (double)xyz[1], (double)xyz[2], lon, lat, alt);
	lla[0] = lon;
	lla[1] = lat;
	lla[2] = alt;
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	xyz XYZ 좌표
	@param	lla 경위고도 좌표 변환값
	@overload 
*/
void GeoMath::XYZ2LL( const double* xyz, double*lla)
{
	XYZ2LL(xyz[0], xyz[1], xyz[2], lla[0], lla[1], lla[2]);
}

/*!
	@brief	XYZ 좌표를 경위도 좌표로 변환한다.
	@param	xyz XYZ 좌표
	@param	lon	경도 변환값
	@param	lat	위도 변환값
	@param	alt	고도 변환값
	@overload 
*/
void GeoMath::XYZ2LL( const double* xyz, double& lon, double& lat, double& alt)
{
	XYZ2LL( xyz[0], xyz[1], xyz[2], lon, lat, alt);
}

QVector3D MOON::GeoMath::XYZ2LL(const QVector3D & xyz)
{
	double lon, lat, alt;
	XYZ2LL(xyz[0], xyz[1], xyz[2], lon, lat, alt);
	return QVector3D(lon, lat, alt);
}


/*!
	@brief	3차원 공간상의 두점간의 거리 계산
	@param	x1 지점 1의 X좌표
	@param	y1 지점 1의 Y좌표
	@param	z1 지점 1의 Z좌표
	@param	x2 지점 2의 X좌표
	@param	y2 지점 2의 Y좌표
	@param	z2 지점 2의 Z좌표
	@return	거리(km)
*/
double GeoMath::DistanceBetweenXYZ(double x1, double y1, double z1, double x2, double y2, double z2)
{
	return sqrt(X2(x1 - x2) + X2(y1 - y2) + X2(z1 - z2));
}

/*!
	@brief	3차원 공간상의 두점간의 거리 계산
	@param	lon1 지점 1의 경도(degree)
	@param	lat1 지점 1의 위도(degree)
	@param	height1 지점 1의 높이(km). 지구중심에서 높이
	@param	lon2 지점 2의 경도(degree)
	@param	lat2 지점 2의 위도(degree)
	@param	height2 지점 2의 높이(km). 지구중심에서 높이
	@return	거리(km)
*/
double GeoMath::Distance( double lon1, double lat1, double height1, double lon2, double lat2, double height2 )
{
	double x1, y1, z1, x2, y2, z2;
	LL2XYZ( lon1, lat1, height1, x1, y1, z1 );
	LL2XYZ( lon2, lat2, height2, x2, y2, z2 );
	return DistanceBetweenXYZ( x1, y1, z1, x2, y2, z2 );
}

/*!
	@brief	3차원 공간상의 세점의 사이각 계산
	@param	x1 지점 1의 X좌표
	@param	y1 지점 1의 Y좌표
	@param	z1 지점 1의 Z좌표
	@param	x2 지점 2의 X좌표
	@param	y2 지점 2의 Y좌표
	@param	z2 지점 2의 Z좌표
	@param	x3 지점 3의 X좌표
	@param	y3 지점 3의 Y좌표
	@param	z3 지점 3의 Z좌표
	@return	지점 2에서의 사이각(degree)
*/
double GeoMath::AngleBetweenXYZ( double x1, double y1, double z1,
							   double x2, double y2, double z2, 
							   double x3, double y3, double z3 )
{
	double a = DistanceBetweenXYZ( x1, y1, z1, x2, y2, z2 );//Distance between p2, p1
	double c = DistanceBetweenXYZ( x3, y3, z3, x2, y2, z2 );//Distance Between p2, p3
	double b = DistanceBetweenXYZ( x1, y1, z1, x3, y3, z3 );//Distance Between p1, p3
	return ACOS( ( X2(a) + X2(c) - X2(b) ) / (2*a*c) );
}

/*!
	@brief	3차원 공간상의 세점의 사이각 계산
	@param	lon1 지점 1의 경도(degree)
	@param	lat1 지점 1의 위도(degree)
	@param	h1 지점 1의 높이(km). 지구중심에서 높이
	@param	lon2 지점 2의 경도(degree)
	@param	lat2 지점 2의 위도(degree)
	@param	h2 지점 2의 높이(km). 지구중심에서 높이
	@param	lon3 지점 3의 경도(degree)
	@param	lat3 지점 3의 위도(degree)
	@param	h3 지점 3의 높이(km). 지구중심에서 높이
	@return	지점 2에서의 사이각(degree)
*/
double GeoMath::AngleBetween( double lon1, double lat1, double h1,/*지구중심으로부터 거리*/
						double lon2, double lat2, double h2, 
						double lon3, double lat3, double h3 )
{
	double x1, y1, z1, x2, y2, z2, x3, y3, z3;
	LL2XYZ( lon1, lat1, h1, x1, y1, z1 );
	LL2XYZ( lon2, lat2, h2, x2, y2, z2 );
	LL2XYZ( lon3, lat3, h3, x3, y3, z3 );

	return AngleBetweenXYZ( x1, y1, z1, x2, y2, z2, x3, y3, z3 );
}

/*!
	@brief	지점 1에서 지점 2를 바라보는 방위각 앙각 계산
	@param	lon1 지점 1의 경도(degree)
	@param	lat1 지점 1의 위도(degree)
	@param	alt1 지점 1의 높이(km). 지구중심에서 높이
	@param	lon2 지점 2의 경도(degree)
	@param	lat2 지점 2의 위도(degree)
	@param	alt2 지점 2의 높이(km). 지구중심에서 높이
	@param	azim 방위각(degree) 계산값
	@param	elev 앙각(degree) 계산값
	@param	earth 지구 반경(km)
	
*/
void GeoMath::AzEl(float lon1, float lat1, float alt1, float lon2, float lat2, float alt2, float* azim, float* elev, float earth)
{
	float dist = Distance(lon1, lat1, alt1, lon2, lat2, alt2);
	double dlon, dlat;

	dlon = fabs(lon1 - lon2);
	dlat = fabs(lat1 - lat2);

    *azim = atan(((2*earth)*(dlat))/(earth*dlon*fabs((COS(lat1)+COS(lat2))))) / D2R;
	
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

	dist /= 1000.0;
	alt1 /= 1000.0;
	alt2 /= 1000.0;	// km 로 변환
	
    //double theta	= (alt1 - alt2) / dist - dist / 2000.0 / earth /4 * 3;
	double theta	= (alt2 - alt1) / dist - dist / (2000.0 * earth *4/3.0);
	*elev = theta / D2R;
}

/*!
	@brief	지표면 두지점의 방위각 거리 계산
	@param	lonT 지점 1의 경도(degree)
	@param	latT 지점 1의 위도(degree)
	@param	lonR 지점 2의 경도(degree)
	@param	latR 지점 2의 위도(degree)
	@param	azim 방위각(degree) 계산값
	@param	dist 거리(km) 계산값
	@param	earthR 지구 반경(km)
*/
void GeoMath::AzimDistEarthToEarth(double lonT, double latT, double lonR, double latR, double* azim, double* dist, double earthR)
{
	if( lonT == lonR && latT == latR ) 
	{
		*azim = 0.0;
		*dist = 0.0;
		return;
	}

	double dlon,dlat,e,g,h;
	dlon = ::fabs(lonT - lonR);
	dlat = ::fabs(latT - latR);

	*dist = PI/360.0*::sqrt(X2(EARTH_R)*X2(dlon)*(COS(latT) +
		COS(latR))*(COS(latT) + COS(latR)) + 4*earthR*earthR * dlat*dlat);
	*azim = ATAN((2.0*earthR*dlat) / (earthR*dlon*::fabs(COS(latT)+COS(latR))));

	if((lonT<=lonR)&&(latT<=latR)) *azim = 90.0 - *azim;
	else if((lonT>lonR)&&(latT<=latR)) *azim = 270.0 + *azim;
	else if((lonT>lonR)&&(latT>latR)) *azim = 270.0 - *azim;
	else *azim = 90.0 + *azim;		// 방위각
}

/*!
	@brief	지표면 한지점에서 방위각방향 거리 만큼 떨어진 지점의 지표면 좌표 계산
	@param	lonT 지점 1의 경도(degree)
	@param	latT 지점 1의 위도(degree)
	@param	lonR 상대 지점의 경도(degree) 계산값
	@param	latR 상대 지점의 위도(degree) 계산값
	@param	earthR 지구 반경(km)
	@param	dist 거리(km)
	@param	azim 방위각(degree)
*/
void GeoMath::RelativePoint( double lonT, double latT, double* lonR, double* latR, double dist, double azim, double earthR )
{
	if( dist == 0 )
	{
		*lonR = lonT;
		*latR = latT;
		return;
	}

	double dlon, dlat;
	double aalon, aalat;
	double da, dd;
	double azim_cal, dist_cal;
	
	double tolAz=0.0000027;			// 1/100초 오차
	double tolDist = dist/100000.0;	// 0.001% 거리 오차

	double az = azim;  
	double dt = dist;  

	// 루프를 반복하면서 오차를 줄여나간다. 1000km이내는 5번 이내만 실행된다.
	for(int i = 0; i< 1000; i++)
	{
		dlon = SIN(azim)*dist;
		dlat = COS(azim)*dist;
		aalon = ::fabs((dlon)/(earthR*D2R)/::fabs(COS(latT)));
		aalat = ::fabs(dlat/(earthR*D2R));
		*lonR = lonT + aalon*((SIN(az)>=0)?(1.0):(-1.0));;
		*latR = latT + aalat*((COS(az)>=0)?(1.0):(-1.0));
		AzimDistEarthToEarth(lonT, latT, *lonR, *latR, &azim_cal, &dist_cal, earthR);
		da = az - azim_cal;
		dd = dt - dist_cal;
		if((::fabs(da) < tolAz) && (::fabs(dd) < tolDist)) break;
		else
		{
			azim = azim + da;
			dist = dist + dd;
		}
	}
}

/*!
	@brief	한지점에서 방위각, 앙각방향 거리 만큼 떨어진 지점의 좌표 계산
	@param	txLon 기준 지점의 경도(degree)
	@param	txLat 기준 지점의 위도(degree)
	@param	txAlt 기준 지점의 고도(km)
	@param	dist 거리(km)
	@param	azim 방위각(degree)
	@param	elev 앙각(degree)
	@param	earthR 지구 반경(km)
	@param	rxLon 상대 지점의 경도(degree)
	@param	rxLat 상대 지점의 위도(degree)
	@param	rxAlt 상대 지점의 고도(km)
*/
void GeoMath::RelativePoint(double txLon, double txLat, double txAlt, double dist, double azim, double elev, double* rxLon, double* rxLat, double* rxAlt, double earthR)
{
	if( elev == 90.0 || elev == -90.0 || dist == 0 ) // 수직아래, 또는 수직위라면 경위도는 같고, 고도만 거리를 더해주면 된다.
	{
		*rxLon = txLon;
		*rxLat = txLat;
		*rxAlt = txAlt + dist*(elev/90.0);// -90이면 빼고, 90이면 더한다.
		return;
	}

	// 상대 경위도 좌표 계산
	RelativePoint(txLon, txLat, rxLon, rxLat, dist, azim, earthR);

	// 사인법칙을 이용하여 푼다. a/sin(A) = b/sin(B) = c/sin(C)
	double alpha = dist*360.0/(2.0*PI*earthR);	// 중심각(degEARTH_R)
	*rxAlt = COS(elev)*(earthR+txAlt)/COS(-elev-alpha) - earthR;
}

/*!
	@brief	범위내의 랜덤한 수를 반환한다.
	@param	MIN	범위 최소값
	@param	MAX	범위 최대값
	@return	범위내의 랜덤 수
*/
double GeoMath::RangeRandom(double MIN, double MAX)
{
	return (double)rand() / (RAND_MAX + 1) * (MAX - MIN) + MIN;
}

/*!
	@brief	지점 1에서 지점 2로의 가시 여부를 체크한다.
	@param	lon1 지점 1의 경도(degree)
	@param	lat1 지점 1의 위도(degree)
	@param	alt1 지점 1의 높이(km). 지구중심에서 높이
	@param	lon2 지점 2의 경도(degree)
	@param	lat2 지점 2의 위도(degree)
	@param	alt2 지점 2의 높이(km). 지구중심에서 높이
	@return	가시 여부
*/
bool GeoMath::VisibleCheck( double lon1, double lat1, double alt1, double lon2, double lat2, double alt2)
{
	return AngleBetween(lon1, lat1, alt1, lon2, lat2, alt2, 0.0, 0.0, 0.0) > 90.0;
}

/*!
	@brief	코사인제2법칙을 이용한 길이 구하기로 두변의 길이와 사이각을 알때 이용한다.
	@param	a 삼각형에서 A변의 길이
	@param	b 삼각형에서 B변의 길이
	@param	C 삼각형에서 C각의 크기(degree)
	@return	사이각이 바라보는 변의 길이(c)
*/
double GeoMath::cos2ndLawLength(double a, double b, double C)
{
	if (C > 180) C = 360.0 - C;
	if (C == 180.0) return a + b;
	double c = sqrt( a*a + b*b - 2.0*a*b*COS(C) );
	return c;
}

/*!
	@brief	세변의 길이(a, b, c)를 알때 c변이 바라보는 꼭지각(C) 계산
	@param	a 삼각형에서 A변의 길이
	@param	b 삼각형에서 B변의 길이
	@param	c 삼각형에서 C변의 길이
	@return	c변이 바라보는 꼭지각(C)(degree)
*/
double GeoMath::cos2ndLawAngle(double a, double b, double c)
{
	if ((a == 0 || b == 0) && (a + b == c)) return 0.0;
	if (a + b == c) return 180.0;

	double C = ACOS( (a*a + b*b - c*c)/(2.0*a*b) );
	return C;
}

/*!
	@brief	double 형 나머지 계산
	@param	numerater	피제수
	@param	denominator	제수
	@return	나머지
*/
double GeoMath::remainder(double numerater, int denominator)
{
	double decimals = numerater - (int)numerater;
	return decimals + (int)numerater%denominator;
}

/*!
	@brief	구와의 교점 계산. 2개의 교점이 있을 경우 가까운 점 반환
	@param	origin	피준점
	@param	p2 다른 시험점
	@param	radius 구의 반경
	@param	intersection 구와의 교점으로 반환값이다.
	@return	구와의 교점 여부
*/
bool	GeoMath::intersectWithSphere(const double* origin, const double* p2, double radius, double* intersection)
{
	double dx = p2[0] - origin[0];
    double dy = p2[1] - origin[1];
    double dz = p2[2] - origin[2];

	bool signx = (dx > 0);
	bool signy = (dy > 0);
	bool signz = (dz > 0);

    double t1, t2;
    double a = dx*dx + dy*dy + dz*dz;
    double b = 2*(dx*origin[0] + dy*origin[1] + dz*origin[2]);
    double c = origin[0]*origin[0] + origin[1]*origin[1] + origin[2]*origin[2] - radius*radius;
    double b24ac = b*b - 4*a*c;
	double lon, lat, alt;
    if (b24ac < 0)
    {
        memcpy(intersection, p2, sizeof(double)*3);
        return false;
    }
    else
    {
        double temp = sqrt(b24ac);
        t1 = (-b + temp) / a / 2;
        t2 = (-b - temp) / a / 2;
    }

    double x1 = origin[0] + t1*dx;
    double y1 = origin[1] + t1*dy;
    double z1 = origin[2] + t1*dz;

    double x2 = origin[0] + t2*dx;
    double y2 = origin[1] + t2*dy;
    double z2 = origin[2] + t2*dz;

	double d1 = GeoMath::DistanceBetweenXYZ(origin[0], origin[1], origin[2], x1, y1, z1);
	double d2 = GeoMath::DistanceBetweenXYZ(origin[0], origin[1], origin[2], x2, y2, z2);

	double xt, yt, zt;
    if (d1 < d2) 
	{
		xt = x1;
		yt = y1;
		zt = z1;
	}
    else 
	{
		xt = x2;
		yt = y2;
		zt = z2;
	}

	// 반대 방향
	if( (xt - origin[0] > 0) == !signx && (yt - origin[1] > 0) == !signy && (zt - origin[2] > 0) == !signz )
	{
		memcpy(intersection, p2, sizeof(double)*3);
		return false;
	}
	
	intersection[0] = xt;
	intersection[1] = yt;
	intersection[2] = zt;

	return true;
}


/*!
	@brief	구와의 교점 계산. 2개의 교점이 있을 경우 모두 반환
	@param	origin	피준점
	@param	p2 다른 시험점
	@param	radius 구의 반경
	@param	intersection 구와의 근교점으로 반환값이다.
	@param	intersection2 구와의 원교점으로 반환값이다.
	@return	구와의 교점 여부
*/
bool	GeoMath::intersectWithSphere2(const double* origin, const double* p2, double radius, double* intersection, double* intersection2)
{
	double dx = p2[0] - origin[0];
    double dy = p2[1] - origin[1];
    double dz = p2[2] - origin[2];

	bool signx = (dx > 0);
	bool signy = (dy > 0);
	bool signz = (dz > 0);

    double t1, t2;
    double a = dx*dx + dy*dy + dz*dz;
    double b = 2*(dx*origin[0] + dy*origin[1] + dz* origin[2]);
    double c = origin[0]*origin[0] + origin[1]*origin[1] + origin[2]*origin[2] - radius*radius;
    double b24ac = b * b - 4 * a * c;
	double lon, lat, alt;
    if (b24ac < 0)
    {
        memcpy(intersection, p2, sizeof(double)*3);
        return false;
    }
    else
    {
        double temp = sqrt(b24ac);
        t1 = (-b + temp) / a / 2;
        t2 = (-b - temp) / a / 2;
    }

    double x1 = origin[0] + t1*dx;
    double y1 = origin[1] + t1*dy;
    double z1 = origin[2] + t1*dz;

    double x2 = origin[0] + t2*dx;
    double y2 = origin[1] + t2*dy;
    double z2 = origin[2] + t2*dz;

	double d1 = GeoMath::DistanceBetweenXYZ(origin[0], origin[1], origin[2], x1, y1, z1);
	double d2 = GeoMath::DistanceBetweenXYZ(origin[0], origin[1], origin[2], x2, y2, z2);

	double xt, yt, zt;
	double xt2, yt2, zt2;

    if (d1 < d2) 
	{
		xt = x1;
		yt = y1;
		zt = z1;
		xt2 = x2;
		yt2 = y2;
		zt2 = z2;
	}
    else 
	{
		xt = x2;
		yt = y2;
		zt = z2;
		xt2 = x1;
		yt2 = y1;
		zt2 = z1;
	}

	// 반대 방향 인 경우
	if( (xt - origin[0] > 0) == !signx && (yt - origin[1] > 0) == !signy && (zt - origin[2] > 0) == !signz )
	{
		memcpy(intersection, p2, sizeof(double)*3);
	}
	else
	{
		intersection[0] = xt;
		intersection[1] = yt;
		intersection[2] = zt;
	}

	// 반대 방향 인 경우
	if( (xt2 - origin[0] > 0) == !signx && (yt2 - origin[1] > 0) == !signy && (zt2 - origin[2] > 0) == !signz )
	{
		memcpy(intersection2, p2, sizeof(double)*3);
	}
	else
	{
		intersection2[0] = xt2;
		intersection2[1] = yt2;
		intersection2[2] = zt2;
	}
	

	return true;
}

double GeoMath::angleBetweenVector3D( const QVector3D& v1, const QVector3D& v2 )
{
	double v = QVector3D::dotProduct(v1, v2)/(v1.length()*v2.length());
	if( v >= 1.0 ) return 0.0;
	double a = acos(v);
	return a;
}

double GeoMath::degreeBetweenVector3D( const QVector3D& v1, const QVector3D& v2 )
{

	double v = QVector3D::dotProduct(v1, v2)/(v1.length()*v2.length());
	if( v >= 1.0 ) return 0.0;
	else if( v <= -1.0 ) return 180.0;
	double a = acos(v);
	return a*R2D;
}
