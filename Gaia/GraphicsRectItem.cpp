/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QtDebug>
#include <QVector3D>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "GaiaMath.h"
#include "Figure.h"
#include "GraphicsRectItem.h"

GraphicsRectItem::GraphicsRectItem( QGraphicsItem* parent )
	: GraphicsDrawItem( parent )
{
	_mode = DragMode_None;

	setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
}

GraphicsRectItem::~GraphicsRectItem()
{

}

void GraphicsRectItem::setData( const QRectF& rect )
{
	_rect = rect;
	setRect(rect);
	_internalChanged = false;
	update();
}

QRectF GraphicsRectItem::boundingRect() const
{
	//return Figure::expand(GraphicsDrawItem::boundingRect(), 5);
	return GraphicsDrawItem::boundingRect();
}

QRectF GraphicsRectItem::data() const
{
	return _rect;
}

void GraphicsRectItem::rotate(double angle)
{
	_rotate = angle;

	QTransform t;
	QPointF cp = _rect.center();
	t.translate(cp.x(), cp.y());
	t.rotate(_rotate);
	t.translate(-cp.x(), -cp.y());
	setTransform(t);
	_transform = t;

	update();
}

double GraphicsRectItem::rotation() const
{
	return _rotate;
}

GraphicsRectItem * GraphicsRectItem::clone() const
{
	GraphicsRectItem* item = new GraphicsRectItem(0);
	item->setData(_rect);
	item->rotate(_rotate);
	item->setPen(pen());
	item->setBrush(brush());

	return item;
}

void GraphicsRectItem::translate(double dx, double dy)
{
	_rect.translate(dx, dy);
	setData(_rect);
}

void GraphicsRectItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QTransform t = painter->worldTransform();
	_lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform( t );

	painter->save();	
	painter->setRenderHint(QPainter::Antialiasing, true);

	QPen pen = GraphicsDrawItem::pen();
	double lineWidth = pen.widthF() / _lod;
	pen.setWidthF(lineWidth);
	//pen.setColor(Qt::red);
	painter->setPen(pen);
	painter->setBrush(brush());
	painter->drawRect(_rect);

	//pen.setColor(Figure::invert(pen.color()));
	//painter->setPen(pen);
	//painter->drawRect(Figure::expand(_rect, lineWidth));
	//painter->drawRect(Figure::expand(_rect, -lineWidth));

	double fPointSize = _selectPointRadius / _lod;

	// Selection
	if( group() == NULL && option->state & QStyle::State_Selected )
	{
		QPen newPen  = pen;
		newPen.setColor(Qt::darkGray);
		//newPen.setStyle( Qt::DashLine );
		//painter->setPen( newPen );		
		//painter->setBrush( Qt::transparent );
		//QTransform t = _transform.inverted();
		//QRectF bound = t.mapRect(boundingRect());
		//bound = _transform.mapRect(bound);
		//painter->drawRect( bound ); 

		//painter->setPen( QPen( Qt::yellow ) );
		//painter->setBrush( Qt::yellow );
				
		double dia = fPointSize * 2;
		QPointF topCenter(_rect.center().x(), _rect.top());

		_editPointRects.clear();
		_editPointRects << Figure::expand(_rect.topLeft(), fPointSize);// QRectF(_rect.topLeft().x() - fPointSize / 2, _rect.topLeft().y() - fPointSize / 2, fPointSize, fPointSize);
		_editPointRects << QRectF(_rect.center().x() - fPointSize, _rect.top() - fPointSize, dia, dia);
		_editPointRects << Figure::expand(_rect.topRight(), fPointSize);// QRectF(_rect.topRight().x() - fPointSize / 2, _rect.topRight().y() - fPointSize / 2, fPointSize, fPointSize);
		_editPointRects << QRectF(_rect.left() - fPointSize, _rect.center().y() - fPointSize, dia, dia);
		_editPointRects << QRectF(_rect.right() - fPointSize, _rect.center().y() - fPointSize, dia, dia);
		_editPointRects << Figure::expand(_rect.bottomLeft(), fPointSize); //QRectF(_rect.bottomLeft().x() - fPointSize / 2, _rect.bottomLeft().y() - fPointSize / 2, fPointSize, fPointSize);
		_editPointRects << QRectF(_rect.center().x() - fPointSize, _rect.bottom() - fPointSize, dia, dia);
		_editPointRects << Figure::expand(_rect.bottomRight(), fPointSize); //QRectF(_rect.bottomRight().x() - fPointSize / 2, _rect.bottomRight().y() - fPointSize / 2, fPointSize, fPointSize);

		QPointF rotPoint = topCenter + QPointF(0, -20.0/ _lod);
		QRectF rotRect = Figure::expand(rotPoint, fPointSize);
		_editPointRects << rotRect;

		QRectF rotArc = Figure::expand(rotRect, 2.0/ _lod);
		
		newPen.setStyle(Qt::SolidLine);
		painter->setBrush(Qt::white);
		painter->setPen(newPen);

		// Draw Rotate line
		painter->drawLine(topCenter, rotPoint);
		for (const QRectF& r : _editPointRects)
		{
			painter->drawEllipse(r);
		}

		// Draw Rotation Arc
		painter->drawArc(rotArc, -60*16, 300*16);

		// Add Boundary with rotate region
		QRectF crect = Figure::expand(_rect, fPointSize);
		crect.setTop(rotRect.top());
		setRect(crect);

		if (_mode != DragMode_None)
		{
			painter->setBrush(Qt::transparent);
			painter->drawRect(_editPointRects[(int)_mode]);
		}
	}
	else
	{
		setRect(_rect);
	}
	//painter->setPen(Qt::red);
	//painter->setBrush(Qt::transparent);
	//painter->drawRect(rect());

	if (_drawInfo)
	{
		painter->setPen(pen);
		painter->drawText(_rect.left()+1, _rect.center().y(), _name);
	}

	painter->restore();
}

void GraphicsRectItem::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
	if (event->button() == Qt::LeftButton)
	{
		_mode = DragMode_None;
		for (int i = 0; i < _editPointRects.count(); i++)
		{
			const QRectF& r = _editPointRects[i];
			if (mapRectToScene(r).contains(event->scenePos()))
			{
				_mode = (RectEditMode)i;
				break;
			}
		}

		if (_mode != DragMode_None)
		{
			update();
			return;
		}
	}

	GraphicsDrawItem::mousePressEvent( event );
}

void GraphicsRectItem::mouseMoveEvent( QGraphicsSceneMouseEvent* event )
{
	QRectF tRect = _rect;
	QPointF curPos = event->scenePos();
	QPointF lastPos = event->lastScenePos();

	QTransform t;
	t.rotate(-_rotate);
	QPointF dp1 = curPos - lastPos;
	QPointF dp = t.map(dp1);
	_internalChanged = true;

	double min = 10.0 / _lod;
	switch( _mode )
	{
	case DragMode_TopLeft:
		tRect.setTopLeft(_rect.topLeft() + dp);
		if( tRect.width() >= min) _rect.setLeft( tRect.left() );
		if( tRect.height() >= min) _rect.setTop( tRect.top() );
		break;
	case DragMode_Top:
		tRect.setTop( _rect.top() + dp.y() );
		if( tRect.height() >= min) _rect.setTop( tRect.top() );
		break;
	case DragMode_TopRight:
		tRect.setTopRight(_rect.topRight() + dp);
		if( tRect.width() >= min) _rect.setRight( tRect.right() );
		if( tRect.height() >= min) _rect.setTop( tRect.top() );
		break;
	case DragMode_Left:
		tRect.setLeft(_rect.left() + dp.x());
		if( (tRect.width() + dp.x()) > min) _rect.setLeft( tRect.left());
		break;
	case DragMode_Right:
		tRect.setRight( _rect.right() + dp.x() );
		if( tRect.width() >= min) _rect.setRight( tRect.right() );
		break;
	case DragMode_BottomLeft:
		tRect.setBottomLeft( _rect.bottomLeft() + dp );
		if( tRect.width() >= min) _rect.setLeft( tRect.left() );
		if( tRect.height() >= min) _rect.setBottom( tRect.bottom() );
		break;
	case DragMode_Bottom:
		tRect.setBottom( _rect.bottom() + dp.y() );
		if( tRect.height() >= min) _rect.setBottom( tRect.bottom() );
		break;
	case DragMode_BottomRight:
		tRect.setBottomRight( _rect.bottomRight() + dp );
		if( tRect.width() >= min) _rect.setRight( tRect.right() );
		if( tRect.height() >= min) _rect.setBottom( tRect.bottom() );
		break;
	case DragMode_Rotate:
	{	
		QPointF cp(_rect.center() + pos());
		QVector3D v1(lastPos - cp);
		QVector3D v2(curPos - cp);
		float sign = QVector3D::crossProduct(v1, v2).z();
		sign = sign > 0 ? 1.0f : -1.0f;
		float ang = Gaia::GaiaMath::angleBetween(lastPos, cp, curPos);
		_rotate += sign*ang;
		rotate(_rotate);
		break;
	}
	default : 
		_rect.translate(dp);
		//GraphicsDrawItem::mouseMoveEvent(event);
		break;
	}
	
	update();
}

void GraphicsRectItem::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
	_mode = DragMode_None;

	GraphicsDrawItem::mouseReleaseEvent( event );
	update();
}

void GraphicsRectItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
	const static QList< Qt::CursorShape> cursors = { Qt::SizeFDiagCursor, Qt::SizeVerCursor, Qt::SizeBDiagCursor, Qt::SizeHorCursor, Qt::SizeHorCursor
				, Qt::SizeBDiagCursor, Qt::SizeVerCursor, Qt::SizeFDiagCursor, Qt::PointingHandCursor };

	bool onEditPoint = false;
	for (int i = 0; i < _editPointRects.count(); i++)
	{
		const QRectF& r = _editPointRects[i];
		if (mapRectToScene(r).contains(event->scenePos()))
		{
			onEditPoint = true;
			setCursor(cursors[i]);
			break;
		}
	}

	if (!onEditPoint) setCursor(Qt::SizeAllCursor);

	GraphicsDrawItem::hoverMoveEvent(event);
}

//void GraphicsRectItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
//{
//	setSelected( true );
//	
//	WorkspaceGraphicsScene* tScene = dynamic_cast<WorkspaceGraphicsScene*>( scene() );
//	if( tScene == NULL )	return;
//
//	QMenu menu;	
//	GraphicsFigureItemControl::instance()->addLineMenu( menu );
//	GraphicsFigureItemControl::instance()->addFillMenu( menu, _info._drawInfo );
//	menu.addSeparator();
//	GraphicsFigureItemControl::instance()->addGroupMenu( menu, tScene );
//	menu.addSeparator();
//	GraphicsFigureItemControl::instance()->addMoveZMenu( menu );
//
//	QAction* action = menu.exec( event->screenPos() );
//	if( action == NULL )	return;
//	
//	GraphicsFigureItemControl::instance()->processMenuAction( action, _info._drawInfo, tScene );	
//
//	update();
//}

// Serialization
QDataStream & operator<<(QDataStream &out, const GraphicsRectItem &item)
{
	out << (GraphicsDrawItem&)(item);
	out << item._rect;
	out << item._rotate;
	return out;
}

// Deserialization
QDataStream & operator>>(QDataStream &in, GraphicsRectItem &item)
{
	in >> (GraphicsDrawItem&)(item);
	in >> item._rect;
	in >> item._rotate;
	return in;
}
