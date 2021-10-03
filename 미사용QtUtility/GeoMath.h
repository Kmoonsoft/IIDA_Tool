/*!
    @file       GeoMath.h
    @date       2018/04/21
    @author     Lucky Kim(lucky@moonsoftinc.com)
    @version    1.1
    @brief      �������� �Լ�
	@section	Details
	���� �����Լ��� �����Ѵ�.
*/
#pragma once
#include "qtutility_global.h"
#include <QVector3D>

namespace MOON
{

#define	S_EPSILON	1.0E-10
#define	EARTH_R				6378.137	///< �����ݰ�(km)

	static const double PI = 3.14159265358979323846264338327950288419716939937510;	///<	������
	static const double D2R = PI / 180.0;		///< Degree To Radian Factor
	static const double R2D = 180.0 / PI;		///< Radian To Degree Factor

	//-----------------------------------------------------------------------------
	// �ﰢ�Լ� ������
	//-----------------------------------------------------------------------------
	#define SIN(deg)	sin((deg) * D2R)		///< Sine(degree) �Լ�
	#define	COS(deg)	cos((deg) * D2R)		///< Cosine(degree) �Լ�
	#define TAN(deg)	tan((deg) * D2R)		///< Tangent(degree) �Լ�

	//-----------------------------------------------------------------------------
	// ���ﰢ�Լ� ������
	//-----------------------------------------------------------------------------
	#define ASIN(x)	(asin(x) * R2D)				///< ASin(degree) �Լ�
	#define	ACOS(x)	(acos(x) * R2D)				///< ACos(degree) �Լ�
	#define ATAN(x)	(atan(x) * R2D)				///< ATan(degree) �Լ�

	///���� x �� �������� ����ϴ� ��ũ��
	#define X2(x)			((x)*(x))			///< �����Լ�
	#define X3(x)			((x)*(x)*(x))		///< �������Լ�
	#define X4(x)			pow(x, 4.0)			///< �������ռ�
	#define X5(x)			pow(x, 5.0)			///< �ټ������Լ�
	///10�� �¼��� ���ϴ� ��ũ��
	#define POW10(x)		pow(10.0, (x))		///< 10^x�� �Լ�

	#define MIN(x, y)		((x) > (y) ? (y) : (x))	///< �ּҰ� ���ϱ�
	#define MAX(x, y)		((x) > (y) ? (x) : (y))	///< �ִ밪 ���ϱ�

/*!
	@class	GeoMath
	@brief	�����Լ� Utility Ŭ����

	3���� ���÷��� �� ���� ������ ���� ���� �Լ����� �����Ѵ�.<br>
	���⼭ ����ϴ� �⺻ ������ ������ ����.<br>
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
		//3���� �������� �������� �Ÿ� ��� ������ height ������ ������.
		static double	DistanceBetweenXYZ(double x1, double y1, double z1, double x2, double y2, double z2);
		static double	Distance(double lon1, double lat1, double height1, double lon2, double lat2, double height2);

		// �������� ���̰� ���, p2(x2, y2, z2)������ ���̰����� �ϴ� ��(degree)�� ����Ѵ�.
		static double	AngleBetweenXYZ(double x1, double y1, double z1,
			double x2, double y2, double z2,
			double x3, double y3, double z3);
		// �������� ���̰� ���, p2(lon2, lat2, h2)������ ���̰����� �ϴ� ��(degree)�� ����Ѵ�.
		static double	AngleBetween(double lon1, double lat1, double h1,/*�����߽����κ��� �Ÿ�*/
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

		// �ڻ�����2��Ģ�� �̿��� ���� ���ϱ�� �κ��� ���̿� ���̰��� �˶� �̿��Ѵ�.
		// ��� : ���̰��� �ٶ󺸴� ���� ����(c)
		static double cos2ndLawLength(double a, double b, double C);

		// ������ ����(a, b, c)�� �˶� c���� �ٶ󺸴� ������(C) ���
		static double cos2ndLawAngle(double a, double b, double c);

		// �������� ������ ����
		static double remainder(double numerater, int denominator);

		// ������ ���� ���. 2���� ������ ���� ��� ����� �� ��ȯ
		// �Է� :	P1, p2 ������ ����
		//			radius : ���� �ݰ�
		// ��� : ������ ���� ����
		static bool	intersectWithSphere(const double* origin, const double* p2, double radius, double* intersection);
		static bool	intersectWithSphere2(const double* origin, const double* p2, double radius, double* intersection, double* intersection2);

		// �� ���� ������ ������ ����Ѵ�.(radian)
		static double angleBetweenVector3D(const QVector3D& v1, const QVector3D& v2);
		// �� ���� ������ ������ ����Ѵ�.(degree)
		static double degreeBetweenVector3D(const QVector3D& v1, const QVector3D& v2);
	};
}
