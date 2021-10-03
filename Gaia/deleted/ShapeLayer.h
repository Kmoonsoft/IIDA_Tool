#pragma once

/*!
@file       ShapeLayer.h
@date       2021.09.15
@author     Lucky
@version    v1.0
@brief		ShaeLayer Header file
@details
*/

#include <qline.h>
#include <QPen>
#include <QBrush>

#include "gaia_global.h"
#include "MapLayer.h"

class GAIA_EXPORT ShapeLayer : public MapLayer
{
public:
	ShapeLayer(QGraphicsItem * parent, const QString& name, const QString& id);
	~ShapeLayer();

public:

	int addPoint(const QPointF& point, int id = -1);
	int addLine(const QLineF& line, int id = -1);
	int addPath(const QPainterPath& path, int id = -1);
	int addRect(const QRectF& rect, int id = -1);
	int addPolygon(const QPolygonF& poly, int id = -1);
	int addEllipse(const QRectF& rect, int id = -1);
	
	QRectF	boundingRect() const;

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:
	//QRectF	_bound;
	QMap<int, QPainterPath> _paths;
	QMap<int, QPointF> _points;
	int _idman = 1;
	QRectF _bRect;
};

