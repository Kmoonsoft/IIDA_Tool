/*!
    @file       GeoMath.h
    @date       2018/04/21
    @author     Lucky Kim(lucky@moonsoftinc.com)
    @version    1.1
    @brief      지리수학 함수
	@section	Details
	관련 수학함수를 선언한다.
*/
#pragma once
#include "qtutility_global.h"
#include <QVector3D>

namespace MOON
{

#define	S_EPSILON	1.0E-10
#define	EARTH_R				6378.137	///< 지구반경(km)

	static const double PI = 3.14159265358979323846264338327950288419716939937510;	///<	원주율
	static const double D2R = PI / 180.0;		///< Degree To Radian Factor
	static const double R2D = 180.0 / PI;		///< Radian To Degree Factor

	//-----------------------------------------------------------------------------
	// 삼각함수 재정의
	//-----------------------------------------------------------------------------
	#define SIN(deg)	sin((deg) * D2R)		///< Sine(degree) 함수
	#define	COS(deg)	cos((deg) * D2R)		///< Cosine(degree) 함수
	#define TAN(deg)	tan((deg) * D2R)		///< Tangent(degree) 함수

	//-----------------------------------------------------------------------------
	// 역삼각함수 재정의
	//-----------------------------------------------------------------------------
	#define ASIN(x)	(asin(x) * R2D)				///< ASin(degree) 함수
	#define	ACOS(x)	(acos(x) * R2D)				///< ACos(degree) 함수
	#define ATAN(x)	(atan(x) * R2D)				///< ATan(degree) 함수

	///숫자 x 의 제곱수를 계산하는 매크로
	#define X2(x)			((x)*(x))			///< 제곱함수
	#define X3(x)			((x)*(x)*(x))		///< 세제곱함수
	#define X4(x)			pow(x, 4.0)			///< 네제곱합수
	#define X5(x)			pow(x, 5.0)			///< 다섯제곱함수
	///10의 승수를 구하는 매크로
	#define POW10(x)		pow(10.0, (x))		///< 10^x승 함수

	#define MIN(x, y)		((x) > (y) ? (y) : (x))	///< 최소값 구하기
	#define MAX(x, y)		((x) > (y) ? (x) : (y))	///< 최대값 구하기

/*!
	@class	GeoMath
	@brief	수학함수 Utility 클래스

	3차원 디스플레이 및 지구 기하학 관련 수학 함수들을 제공한다.<br>
	여기서 사용하는 기본 단위는 다음과 같다.<br>
	Distance, Altitude : km<br>
	Longitude, Latitude, Azimuth, Elevation : degree<br>
*/
	class QTUTILITY_EXPORT GeoMath
	{
	public:
		// Lon, Lat, Alt to XYZ
		static void LL2XYZ(const double* ll, double* xyz);
		static void LL2XYZ(double lon, double lat, double alt, double* xyz);
		static void LL2XYZ(double lon, double lat, double alt, double& x, double& y, double& z);
		static void LL2XYZ(double lon, double lat, double alt, float* xyz);
		static QVector3D LL2XYZ(double lon, double lat, double alt);
		static QVector3D LL2XYZ(const QVector3D& lla);
		static void LL2Norm(const double* ll, double* pos);

		// XYZ to Lon, Lat, Alt
		static void XYZ2LL(double x, double y, double z, double& lon, double& lat, double& alt);
		static void	XYZ2LL(const float* xyz, double& lon, double& lat, double& alt);
		static void	XYZ2LL(float x, float y, float z, double& lon, double& lat, double& alt);
		static void	XYZ2LL(const float* xyz, float*lla);
		static void	XYZ2LL(const double* xyz, double*lla);
		static void	XYZ2LL(const double* xyz, double& lon, double& lat, double& alt);
		static QVector3D XYZ2LL(const QVector3D& xyz);

		// Distance and Angle
		//3차원 공간상의 두점간의 거리 계산 단위는 height 단위를 따른다.
		static double	DistanceBetweenXYZ(double x1, double y1, double z1, double x2, double y2, double z2);
		static double	Distance(double lon1, double lat1, double height1, double lon2, double lat2, double height2);

		// 세점간의 사이각 계산, p2(x2, y2, z2)지점을 사이각으로 하는 각(degree)를 계산한다.
		static double	AngleBetweenXYZ(double x1, double y1, double z1,
			double x2, double y2, double z2,
			double x3, double y3, double z3);
		// 세점간의 사이각 계산, p2(lon2, lat2, h2)지점을 사이각으로 하는 각(degree)를 계산한다.
		static double	AngleBetween(double lon1, double lat1, double h1,/*지구중심으로부터 거리*/
			double lon2, double lat2, double h2,
			double lon3, double lat3, double h3);

		// Azimuth & Elevation
		static void AzEl(float lon1, float lat1, float alt1, float lon2, float lat2, float alt2, float* azim, float* elev, float earth);

		static void AzimDistEarthToEarth(double lonT, double latT, double lonR, double latR, double* azim, double* dist, double earthR);
		static void RelativePoint(double fromLon, double fromLat, double* toLon, double* toLat, double distanceMeter, double azimuth, double earthR);
		static void RelativePoint(double txLon, double txLat, double txAlt, double dist, double azim, double elev, double* rxLon, double* rxLat, double* rxAlt, double earthR);

		// Make Random value
		static double RangeRandom(double min, double max);

		static bool VisibleCheck(double lon1, double lat1, double alt1, double lon2, double lat2, double alt2);

		// 코사인제2법칙을 이용한 길이 구하기로 두변의 길이와 사이각을 알때 이용한다.
		// 출력 : 사이각이 바라보는 변의 길이(c)
		static double cos2ndLawLength(double a, double b, double C);

		// 세변의 길이(a, b, c)를 알때 c변이 바라보는 꼭지각(C) 계산
		static double cos2ndLawAngle(double a, double b, double c);

		// 더블형의 나머지 연산
		static double remainder(double numerater, int denominator);

		// 구와의 교점 계산. 2개의 교점이 있을 경우 가까운 점 반환
		// 입력 :	P1, p2 공간의 두점
		//			radius : 구의 반경
		// 출력 : 구와의 교점 여부
		static bool	intersectWithSphere(const double* origin, const double* p2, double radius, double* intersection);
		static bool	intersectWithSphere2(const double* origin, const double* p2, double radius, double* intersection, double* intersection2);

		// 두 벡터 사이의 각도를 계산한다.(radian)
		static double angleBetweenVector3D(const QVector3D& v1, const QVector3D& v2);
		// 두 벡터 사이의 각도를 계산한다.(degree)
		static double degreeBetweenVector3D(const QVector3D& v1, const QVector3D& v2);
	};
}
