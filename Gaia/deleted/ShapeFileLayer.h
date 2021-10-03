#pragma once

#include <QBrush>
#include "gaia_global.h"
#include "MapLayer.h"

class GAIA_EXPORT ShapeFileLayer : public MapLayer
{
public:
	ShapeFileLayer( QGraphicsItem * parent, const QString& name, const QString& id);

public:
	QRectF	boundingRect() const;
	bool	open(const QString& shapeFile, int ignoreCount = 0);
	
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void	setMinVisibleArea(double minArea) { _visibleArea = minArea; };

private:
	QRectF	_bound;
	QVector<float> _areas;
	QVector<QPolygonF> _polygons;
	QVector<QRectF>	_boundingRect;
	double	_visibleArea;
};

