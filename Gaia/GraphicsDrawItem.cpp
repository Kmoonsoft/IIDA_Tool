
/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QCursor>
#include <QGraphicsSceneContextMenuEvent>

#include "GraphicsModel.h"
#include "GraphicsDrawItem.h"

GraphicsDrawItem::GraphicsDrawItem(QGraphicsItem* parent)
	: QGraphicsRectItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setAcceptHoverEvents(true);
}

GraphicsDrawItem::~GraphicsDrawItem()
{
}

int GraphicsDrawItem::id() const
{
	return _id;
}

void GraphicsDrawItem::setId(int id)
{
	_id = id;
}

QString GraphicsDrawItem::name() const
{
	return _name;
}

void GraphicsDrawItem::setName(const QString & name)
{
	_name = name;
}

bool GraphicsDrawItem::changed() const
{
	return _internalChanged;
}

void GraphicsDrawItem::resetChanged()
{
	_internalChanged = false;
}

void GraphicsDrawItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	_drawInfo = true;
	QGraphicsRectItem::hoverEnterEvent(event);
}

void GraphicsDrawItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setCursor(Qt::ArrowCursor);
	_drawInfo = false;
	QGraphicsRectItem::hoverLeaveEvent(event);
}

void GraphicsDrawItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	setSelected( true );
	GraphicsModel* model = dynamic_cast<GraphicsModel*>(scene());
	if (model)
	{
		model->requestContextMenu(this, event->screenPos());
	}
	//QMenu menu;
	//QAction *setClassAction = menu.addAction("Set Class");
	//QAction *action = menu.exec(event->screenPos());

	//if (action == setClassAction)
	//{
	//	GraphicsModel* model = dynamic_cast<GraphicsModel*>(scene());
	//	if (model)
	//	{

	//	}
	//}
}

// Serialization
QDataStream & operator<<(QDataStream &out, const GraphicsDrawItem &item)
{
	out << item.rect();
	//out << item._flags;
	out << item._selectPointRadius;
	out << item._boundaryMargin;
	out << item._lod;
	return out;
}

// Deserialization
QDataStream & operator>>(QDataStream &in, GraphicsDrawItem &item)
{
	QRectF r;
	in >> r;
	item.setRect(r);
	//in >> item._flags;
	in >> item._selectPointRadius;
	in >> item._boundaryMargin;
	in >> item._lod;
	return in;
}
