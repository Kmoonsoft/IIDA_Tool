#pragma once
#include "StationLayer.h"
#include "maplayer.h"

#include "gaia_global.h"

struct GAIA_EXPORT LinkItem
{
	QString			id;
	StationLayer*	tx;
	StationLayer*	rx;
	bool			visible;
	int				type;
	QPen			pen;

	bool operator==(const LinkItem& link)
	{
		return (id == link.id);
	};
};

class GAIA_EXPORT LinkLayer : public MapLayer
{
public:
	LinkLayer(QGraphicsItem * parent, const QString& name, const QString& id) : MapLayer(parent, name, id){};
	~LinkLayer();

public:
	QRectF	boundingRect() const;   

	//////
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	LinkItem*	addLink(StationLayer* tx, StationLayer* rx, const QString& id, int type = 0);
	void		removeLink(const QString& id);

	LinkItem*	getLink(const QString& id);
	//void		setVisibleByStationID(const QString& id, bool visible);

	void		clear();
	void		setPen(const QPen& pen, int type = 0);

private:
	QMap<QString, LinkItem*> links;
};
