/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QVector2D>
#include <QPainter>
#include <QTransform>

#include "GaiaMath.h"
#include "Figure.h"

using namespace Gaia;

QRectF Figure::expand(const QRectF & rect, double buffer)
{
	return QRectF(rect.left() - buffer, rect.top() - buffer, rect.width() + buffer*2, rect.height() + buffer*2);
}

QRectF Figure::expand(const QPointF & point, double buffer)
{
	QPointF buf(buffer, buffer);
	return QRectF(point - buf, point + buf);
}

QPolygonF Figure::expand(const QLineF & line, double buffer)
{
	QRectF r(0, -buffer, line.length(), buffer*2.0);
	double angle = GaiaMath::angleWithXAxis(line);
	QTransform t;
	t.translate(line.p1().x(), line.p1().y());
	t.rotate(angle);
	QPolygonF poly = t.map(r);

	return poly;
}

QPolygonF Figure::expand(const QPolygonF & line, double buffer)
{
	QPolygonF poly;
	for (int i = 1; i < line.count(); i++)
	{
		QLineF l(line.at(i - 1), line.at(i));
		QPolygonF linePoly = expand(l, buffer);
		poly = poly.united(linePoly);
	}

	return poly;
}

bool Figure::intersected(const QLineF & line, const QPointF& point, double buffer, QPointF& interSection)
{
	QVector2D Q(line.p2() - line.p1());

	QVector2D P(point - line.p1());
	QVector2D proj = QVector2D::dotProduct(P, Q) / Q.lengthSquared() * Q;
	QPointF interPoint = proj.toPointF() + line.p1();

	int ll = QVector2D(interPoint-line.p1()).length() + QVector2D(interPoint-line.p2()).length();
	double dist = QLineF(point, interPoint).length();

	if (Q.length() < ll || dist > buffer) return false;
	interSection = interPoint;
	return true;

	//QPointF tp = point;
	//if (tp == interPoint)
	//{
	//	tp = tp + QPointF(1, 0);
	//	interPoint -= QPointF(1, 0);
	//}
	//QLineF interLine(tp, interPoint);
	//QLineF::IntersectType type = interLine.intersect(line, &interSection);
	//double dist = QLineF(point, interPoint).length();
	//bool ok = (type != QLineF::BoundedIntersection && dist < buffer);
	//return ok;
}

bool Figure::intersected(const QPolygonF & poly, const QPointF & point, double buffer, QPointF & interSection, int & idx)
{
	QPointF p = poly.last();
	for (int i = 0; i < poly.count(); i++)
	{
		QLineF line(p, poly.at(i));
		bool ok = intersected(line, point, buffer, interSection);
		if (ok)
		{
			idx = i;
			return true;
		}
		p = poly.at(i);
	}

	return false;
}

QRectF Figure::boundingRect(const QLineF & line)
{
	return QRectF(QPointF(qMin(line.x1(), line.x2()), qMin(line.y1(), line.y2())), 
		QPointF(qMax(line.x1(), line.x2()), qMax(line.y1(), line.y2())));
}

QColor Figure::invert(const QColor & c)
{
	QColor color(255 - c.red(), 255- c.green(), 255 - c.red());
	return color;
}

QIcon Figure::makeColorIcon(const QColor & color, int width, int height)
{
	QPixmap m(width, height);
	QPainter p(&m);
	p.fillRect(0, 0, width, height, color);

	return QIcon(m);
}
