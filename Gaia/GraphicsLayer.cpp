/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QtGui>
#include <QtWidgets>
#include <float.h>
#include "GraphicsLayer.h"

GraphicsLayer::GraphicsLayer(QGraphicsItem *parent, const QString& name, const QString& id)
	: QGraphicsItemGroup(parent)
{
	_id = id;
	_name = name;
	_lodMin = 0.0;
	_lodMax = DBL_MAX;
	_visibility = true;
	_pen = QPen(Qt::black);
}

//MapLayer::~MapLayer()
//{
//}

int GraphicsLayer::type() const
{
	return Type;
}

void GraphicsLayer::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	//painter->setPen(_pen);
	//painter->setBrush(_brush);
	//setVisibility(_visibility);
	//if( !_visibility ) 
	//{
	//	QGraphicsItemGroup::paint(painter, option, widget);
	//	return;
	//}

	//qDebug() << scene()->sceneRect();
	qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
	lodCheck(lod);
	QGraphicsItemGroup::paint(painter, option, widget);
}

void GraphicsLayer::lodCheck(double lod)
{
	setVisible( _visibility  && lod >= _lodMin && lod <= _lodMax ) ;

	QList<GraphicsLayer*> list = childLayers();
	foreach(GraphicsLayer* child, list)
	{
		//MapLayer* l = dynamic_cast<MapLayer*>(i);
		child->lodCheck(lod);
	}

	//QList<QGraphicsItem*> list = childItems();
	//foreach(QGraphicsItem* i, list)
	//{
	//	MapLayer* l = dynamic_cast<MapLayer*>(i);
	//	if( l ) l->lodCheck(lod);
	//}
}

int GraphicsLayer::childCount() const
{
	return childItems().count();
}

void GraphicsLayer::addSubLayer( GraphicsLayer* layer )
{
	if( layer == 0 ) return;

	addToGroup(layer);
	_subLayers << layer;
}

void GraphicsLayer::removeSubLayer( GraphicsLayer* layer )
{
	if( layer == 0 ) return;

	removeFromGroup(layer);
	scene()->removeItem(layer);
	_subLayers.removeAll(layer);
}

bool GraphicsLayer::removeSubLayer( const QString& id)
{
	foreach(GraphicsLayer* l, _subLayers)
	{
		if( l->id() == id ) 
		{
			removeSubLayer(l);
			return true;
		}
	}

	foreach(GraphicsLayer* l, _subLayers)
	{
		if( l->removeSubLayer(id) ) return true;
	}

	return false;
}

QRectF GraphicsLayer::visibleRect() const 
{
	QList<QGraphicsView*> l = scene()->views();
	if( l.isEmpty() ) return QRectF();

	QGraphicsView* view = l.at(0);
	return view->mapToScene(view->rect()).boundingRect();
}

GraphicsLayer*	GraphicsLayer::findLayer(const QString& id)
{
	foreach(GraphicsLayer* l, _subLayers)
	{
		if( l->id() == id ) return l;
		GraphicsLayer* ll = l->findLayer(id);
		if(ll ) return ll;
	}

	return 0;
}

int GraphicsLayer::addToGroup(QGraphicsItem * item, int id)
{
	if (id == -1) id = _itemId++;
	else
	{
		if (id > _itemId)_itemId = id;
	}

	_subItemMap[id] = item;
	QGraphicsItemGroup::addToGroup(item);
	return id;
}

void	GraphicsLayer::clear()
{
	_subItemMap.clear();
	_itemId = 1;

	QList<QGraphicsItem*> children = childItems();
	for (int i = 0; i < children.count(); i++)
	{
		removeFromGroup(children[i]);
		delete children[i];
	}

	//qDeleteAll(childItems());
	//qDeleteAll(_subLayers);
	_subLayers.clear();
}

QGraphicsItem * GraphicsLayer::item(int id)
{
	return _subItemMap.value(id, 0);
}
