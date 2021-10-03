#ifndef GRIDLAYER_H
#define GRIDLAYER_H

#include <QPen>
#include "MapLayer.h"
#include "gaia_global.h"

class GridItem;

class GAIA_EXPORT GridLayer : public MapLayer
{
public:
	GridLayer(QGraphicsItem * parent, const QString& name, const QString& id);
	~GridLayer();

	QRectF	boundingRect() const;
	//QPainterPath shape() const;

	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void	setPen(const QPen& p) { _pen = p; };

private:
	QPen	_pen;
};

#endif // GRIDLAYER_H
