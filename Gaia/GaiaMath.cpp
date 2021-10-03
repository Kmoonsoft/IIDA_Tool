/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <math.h>
#include <QVector2D>

#include "GaiaMath.h"

using namespace Gaia;

double GaiaMath::angleBetween(const QPointF & p1, const QPointF & center, const QPointF & p2)
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
double GaiaMath::angleWithXAxis(const QLineF & line)
{
	QPointF xaxis(line.p1().x() + 1.0, line.p1().y());
	double a = angleBetween(line.p2(), line.p1(), xaxis);
	return a;
}


