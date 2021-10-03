#pragma once

/*!
@file       ImagePainter.h
@date       2021.09.15
@author     ACJ
@version    v0.1
@brief      GraphicsDrawItem Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QGraphicsRectItem>
#include "gaia_global.h"

/*!
@brief GraphicsDrawItem is a base class for all draw items(Sprites)
*/
class GAIA_EXPORT GraphicsDrawItem : public QGraphicsRectItem
{
	
public:
	
	/*!@brief Define Itme Types*/
	enum {Type = UserType + 1024, PointType, LineType, RectType, PolygonType};
	
	GraphicsDrawItem(QGraphicsItem* parent = 0);
	~GraphicsDrawItem();

	int id() const;
	void setId(int id);

	QString name() const;
	void setName(const QString& name);

	/*!
	@brief Make clone of this item. clone has no parent item.
	*/
	virtual GraphicsDrawItem* clone() const  = 0;

	/*! @brief Return type */
	int type() const { return Type; };

	bool changed() const;
	void resetChanged();

	virtual void translate(double dx, double dy) = 0;

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected:
	
	int _id = -1;
	QString _name;
	//GraphicsItemFlags _flags;			// Graphics Item Flasgs
	double _selectPointRadius = 5.0;	// Selection point(circle) radius(px)
	double _boundaryMargin = 5.0;		// 
	double _lod = 1.0;					// Current LOD(Level of details)
	bool _internalChanged = false;		// Changed by Mouse/Keyboard
	bool _drawInfo = false;

	//Related Non-Members
public:
	// For serialization
	friend QDataStream & operator<<(QDataStream &out, const GraphicsDrawItem &item);
	friend QDataStream & operator>>(QDataStream &in, GraphicsDrawItem &item);
};


