#include "BackgroundImageLayer.h"

BackgroundImageLayer::BackgroundImageLayer(const QString& imgFile, QGraphicsItem *parent, const QString& name, const QString& id)
:MapLayer(parent, name, id)
{
	QPixmap background(imgFile);
	QGraphicsPixmapItem* baseImg = new QGraphicsPixmapItem(background );
	addToGroup(baseImg);
	QTransform t;
	t.scale(360.0/background.width(), 180.0/background.height());
	//t.translate(-180, -90);
	t.translate(-background.width()/2.0, background.height()/2.0);
	t.rotate(180, Qt::XAxis);
	baseImg->setTransform(t);
}
