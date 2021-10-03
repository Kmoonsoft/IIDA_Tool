#pragma once
/*!
@file       GraphicsLayer.h
@date       2021.09.13
@author     ACJ
@version    v0.1
@brief      Graphics Layer Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>
#include <QList>
#include "gaia_global.h"

/*!
@brief GraphicsLayer Base class
*/
class GAIA_EXPORT GraphicsLayer : public QGraphicsItemGroup
{

public:

	enum { Type = UserType + 100 };

	GraphicsLayer(QGraphicsItem *parent = 0, const QString& name = QString(), const QString& id = QString());
	//~MapLayer();

	QString id() const { return _id; };
	QString	name() const { return _name; };
	void	setName(const QString& name) { _name=name; };
	void	setLODRange(double min, double max){_lodMin=min, _lodMax=max;};
	void	setLODMin(double min){_lodMin = min;};
	void	setLODMax(double max){_lodMax = max;};
	void	setVisibility(bool v){_visibility = v; setVisible(_visibility);};

	GraphicsLayer*	findLayer(const QString& id);

	int		type() const;
	double	lodMin() const { return _lodMin; };
	double	lodMax() const { return _lodMax; };
	bool	visibility() const { return _visibility; };
	virtual void	lodCheck(double lod);

	void	addSubLayer( GraphicsLayer* layer );
	void	removeSubLayer( GraphicsLayer* layer );
	bool	removeSubLayer( const QString& id);

	int		childCount() const;
	QList<GraphicsLayer*>	childLayers() const { return _subLayers;};

	QRectF	visibleRect() const;

	void	setPen(const QPen& pen) { _pen = pen; };
	QPen	pen() const { return _pen; };

	void	setBrush(const QBrush& brush) { _brush = brush; };
	QBrush	Brush() const { return _brush; };

	int		addToGroup(QGraphicsItem* item, int id = -1);
	void	clear();

	QGraphicsItem* item(int id);

protected:
	void	paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

protected:

	QString		_id;
	QString		_name;	
	double		_lodMin;
	double		_lodMax;
	bool		_visibility;
	QList<GraphicsLayer*>	_subLayers;

	QPen	_pen;
	QBrush	_brush;

	QMap<int, QGraphicsItem*> _subItemMap;
	int		_itemId = 1;
};

