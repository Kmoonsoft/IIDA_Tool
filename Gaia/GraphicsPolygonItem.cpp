/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QPainter>
#include <QtDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>

#include "GraphicsPolygonItem.h"
#include "Figure.h"


GraphicsPolygonItem::GraphicsPolygonItem( QGraphicsItem* parent )
	: GraphicsDrawItem( parent )
{
	setFlags( QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
}

GraphicsPolygonItem::~GraphicsPolygonItem()
{

}

void GraphicsPolygonItem::setData( const QPolygonF& polygon, bool closed)
{
	_polygon = polygon;
	if (_polygon.first() == _polygon.last()) _polygon.takeLast();
	_closed = closed;
	_internalChanged = false;

	setRect(_polygon.boundingRect());
	updateData();
}

QPolygonF GraphicsPolygonItem::data() const
{
	return _polygon;
}

GraphicsPolygonItem * GraphicsPolygonItem::clone() const
{
	GraphicsPolygonItem* item = new GraphicsPolygonItem(0);
	item->setData(_polygon);
	item->setPen(pen());
	item->setBrush(brush());
	return item;
}

bool GraphicsPolygonItem::deleteSelectedPoint()
{
	if (_dragIdx == -1) return false;
	_polygon.removeAt(_dragIdx);
	_selectRects.removeAt(_dragIdx);
	_dragIdx = -1;
	_internalChanged = true;

	// 1포인터만 남는다면 완전히 삭제
	if (_polygon.count() < 2) return false;

	updateData();
	return true;
}

void GraphicsPolygonItem::translate(double dx, double dy)
{
	_polygon.translate(dx, dy);
	setData(_polygon);
}

void GraphicsPolygonItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	if (_polygon.count() < 2) return;

	QTransform t = painter->worldTransform();
	const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform( t);

	painter->save();
	QPen pen = GraphicsDrawItem::pen();
	pen.setWidthF(pen.widthF() / lod);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::Antialiasing, true);

	QPainterPath pp;
	pp.addPolygon( _polygon );
	//pp.setFillRule( _info._drawInfo._fillRule );

	if (_closed) // Closed : Polygon
	{
		painter->drawPolygon(_polygon);
	}
	else // Open : Polyline
	{
		painter->drawPolyline(_polygon);
	}
	
	if( group() == NULL && option->state & QStyle::State_Selected )
	{
		QRectF bRect = _polygon.boundingRect();
		QPen newPen = pen;
		newPen.setColor(Qt::darkGray);
		newPen.setStyle( Qt::DashLine );
		painter->setPen( newPen );		
		painter->setBrush( Qt::transparent );
		painter->drawRect(bRect );
		
		painter->setBrush( Qt::white );
				
		float fPointSize = _selectPointRadius / lod;

		newPen.setStyle(Qt::SolidLine);
		painter->setBrush(Qt::white);
		painter->setPen(newPen);

		_selectRects.clear();
		for (const QPointF& p : _polygon)
		{
			QRectF r = Figure::expand(p, fPointSize);
			_selectRects << r;
			painter->drawEllipse(r);
		}

		setRect(Figure::expand(bRect, fPointSize));

		if (_dragIdx > -1)
		{
			painter->setBrush(Qt::transparent);
			painter->drawRect(_selectRects[_dragIdx]);
		}
	}
	else
	{
		_dragIdx = -1;
		setRect(_polygon.boundingRect());
	}

	painter->restore();
}

void GraphicsPolygonItem::contextMenuEvent( QGraphicsSceneContextMenuEvent* event )
{
	setSelected( true );
	
	//WorkspaceGraphicsScene* tScene = dynamic_cast<WorkspaceGraphicsScene*>( scene() );
	//if( tScene == NULL )	return;

	//QMenu menu;	
	//GraphicsFigureItemControl::instance()->addLineMenu( menu );
	//GraphicsFigureItemControl::instance()->addFillMenu( menu, _info._drawInfo );
	//menu.addSeparator();
	//menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill ) );
	//menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill ) );
	//
	//if( _info._drawInfo._fillRule == Qt::OddEvenFill )
	//{
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setChecked( true );
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setEnabled( false );

	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setChecked( false );
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setEnabled( true );
	//}
	//if( _info._drawInfo._fillRule == Qt::WindingFill )
	//{
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setChecked( false );
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_OddEvenFill )->setEnabled( true );

	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setChecked( true );
	//	GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_WindingFill )->setEnabled( false );
	//}
	//menu.addAction( GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_Closed ) );
	//GraphicsFigureItemControl::instance()->action( GraphicsFigureItemControl::Action_Closed )->setChecked( _info._drawInfo._bClosed );
	//menu.addSeparator();
	//GraphicsFigureItemControl::instance()->addGroupMenu( menu, tScene );
	//menu.addSeparator();
	//GraphicsFigureItemControl::instance()->addMoveZMenu( menu );

	//QAction* action = menu.exec( event->screenPos() );
	//if( action == NULL )	return;
	//
	//GraphicsFigureItemControl::instance()->processMenuAction( action, _info._drawInfo, tScene );	

	update();
}

void GraphicsPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		_dragIdx = -1;

		for(int i = 0;i < _selectRects.count(); i++)
		{ 
			const QRectF& r = _selectRects[i];
			if (mapRectToScene(r).contains(event->scenePos()))
			{
				_dragIdx = i;
			}
		}
	}

	GraphicsDrawItem::mousePressEvent(event);
	update();
}

void GraphicsPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	if (_dragIdx > -1)
	{
		QPointF& p = _polygon[_dragIdx];
		p += event->scenePos() - event->lastScenePos();

		updateData();

		event->accept();
		_internalChanged = true;
	}
	else
	{
		GraphicsDrawItem::mouseMoveEvent(event);
	}
}

void GraphicsPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	//_dragIdx = -1;

	GraphicsDrawItem::mouseReleaseEvent(event);
}

void GraphicsPolygonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF spos = event->scenePos() - pos();
	QPointF interPoint;
	int idx;
	qDebug() << event->scenePos() << pos() << spos;
	qDebug() << _polygon;

	bool ok = Figure::intersected(_polygon, spos, 5.0/_lod, interPoint, idx);
	if (ok)
	{
		_polygon.insert(idx, interPoint);
		updateData();
		_internalChanged = true;
	}


	GraphicsDrawItem::mouseDoubleClickEvent(event);
}

void GraphicsPolygonItem::updateData()
{
	QRectF tRect = _polygon.boundingRect();

	////QRectF r = Figure::expand(tRect, _boundaryMargin);
	setRect(tRect);
	update();
}
