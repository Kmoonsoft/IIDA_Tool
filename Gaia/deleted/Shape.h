#ifndef SHAPE_H
#define SHAPE_H

#include <QVector>

/* -------------------------------------------------------------------- */
/*      Shape types (nSHPType)                                          */
/* -------------------------------------------------------------------- */
#define SHPT_NULL	0
#define SHPT_POINT	1
#define SHPT_POLYLINE	3
#define SHPT_POLYGON	5
#define SHPT_MULTIPOINT	8
#define SHPT_POINTZ	11
#define SHPT_ARCZ	13
#define SHPT_POLYGONZ	15
#define SHPT_MULTIPOINTZ 18
#define SHPT_POINTM	21
#define SHPT_ARCM	23
#define SHPT_POLYGONM	25
#define SHPT_MULTIPOINTM 28
#define SHPT_MULTIPATCH 31

/* -------------------------------------------------------------------- */
/*      Envelop                                      */
/* -------------------------------------------------------------------- */
struct Envelop
{
	double	xmin;
	double	ymin;
	double	xmax;
	double	ymax;

	operator QRectF() const
	{
		return QRectF(xmin, ymin, xmax-xmin, ymax-ymin);
	};

	double area() const
	{
		return (xmax-xmin)*(ymax-ymin);
	}
};

struct GeoPoint
{
	double x;
	double y;
	operator QPointF() const
	{
		return QPointF(x,y);
	};
};

/* -------------------------------------------------------------------- */
/*      PoliLine & Polygon                                              */
/* -------------------------------------------------------------------- */
struct Polygon 
{
	Envelop		envelop;
	QVector<int> parts;
	QVector<GeoPoint> points;
};

#endif // SHAPE_H
