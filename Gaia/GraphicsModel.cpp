/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QtCore>
#include <QtDebug>
#include <QFileInfo>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QUndoStack>

#include "GraphicsUndoCommand.h"
#include "GraphicsDrawItem.h"
#include "GraphicsPolygonItem.h"
#include "ImageLayer.h"
#include "GraphicsRectItem.h"
#include "Figure.h"
#include "GraphicsLayer.h"
#include "GraphicsModel.h"

/*!
@brief Model Private Data
*/
class GraphicsModelData
{
public:
	GraphicsLayer*		topLayer = 0;
	ImageLayer*		imageLayer = 0;
	QPointF			mousePos;
	QPointF			prevPos;

	GraphicsModel::DrawMode	drawMode = GraphicsModel::DrawNone;
	bool			drawRectStart = false;
	QGraphicsItem* drawItem = 0; // Current Drawing Item

	QTime		moveTime;

	QGraphicsRectItem sprites;

	QMap<int, GraphicsDrawItem*> spriteMap;

	QList<GraphicsDrawItem*> copies;
	QUndoStack				undoStack;

	int copyRepeat = 0;
};

GraphicsModel::GraphicsModel(QObject *parent)
    : QGraphicsScene(parent)
{
	_d = new GraphicsModelData;
	_d->topLayer = new GraphicsLayer(0, "Root", "Root");
	addItem(_d->topLayer);
	addItem(&_d->sprites);

	_d->moveTime.start();
}

GraphicsModel::~GraphicsModel()
{
	delete _d;
}

void GraphicsModel::addLayer( GraphicsLayer* layer )
{
	layer->setZValue( _d->topLayer->childItems().count() );
	_d->topLayer->addSubLayer(layer);
}

void GraphicsModel::removeLayer( GraphicsLayer* layer )
{
	if( layer ) _d->topLayer->removeSubLayer(layer);
}

void GraphicsModel::removeLayer(const QString& id)
{
	_d->topLayer->removeSubLayer(id);
}

GraphicsLayer* GraphicsModel::findLayer(const QString& id)
{
	return _d->topLayer->findLayer(id);
}

GraphicsDrawItem * GraphicsModel::sprite(int id) const
{
	return _d->spriteMap.value(id);
}

GraphicsLayer * GraphicsModel::topLayer() const
{
	return _d->topLayer;
}

void GraphicsModel::setImageLayer(ImageLayer* imageLayer)
{
	_d->imageLayer = imageLayer;
}

void GraphicsModel::addSprite(GraphicsDrawItem * item)
{
	item->setParentItem(&_d->sprites);
	//item->setTransform(_d->baseTransform);
	_d->spriteMap[item->id()] = item;

	_d->undoStack.push(new AddFigureCommand(item, &_d->sprites, this));
}

void GraphicsModel::addSprites(QList<GraphicsDrawItem*>& items, bool addUndoStack)
{
	QList<QGraphicsItem*> list;
	for (GraphicsDrawItem* item : items)
	{
		//item->setTransform(_d->baseTransform);
		item->setParentItem(&_d->sprites);
		_d->spriteMap[item->id()] = item;
		list << item;
	}
	if(addUndoStack) _d->undoStack.push(new AddFigureGroupCommand(list, &_d->sprites, this));
}

void GraphicsModel::clearSprites()
{
	for (QGraphicsItem* i : _d->sprites.childItems())
	{
		i->setParentItem(0);
		delete i;
	}

	_d->spriteMap.clear();
}

void GraphicsModel::resetUndoStack()
{
	_d->undoStack.clear();
}

QPointF GraphicsModel::mousePosition() const
{
	return _d->mousePos;
}

void GraphicsModel::drawRect(bool on)
{
	_d->drawMode = on ? DrawRect : DrawNone;
	_d->drawRectStart = false;
	_d->drawItem = 0;
	
	for (QGraphicsItem* item : _d->sprites.childItems())
	{
		item->setFlag(QGraphicsItem::ItemIsSelectable, !on);
		item->setFlag(QGraphicsItem::ItemIsMovable, !on);
	}
}

GraphicsModel::DrawMode GraphicsModel::drawMode() const
{
	return _d->drawMode;
}

void GraphicsModel::requestContextMenu(GraphicsDrawItem * item, const QPoint & screenPos)
{
	emit contextMenuRequested(item->id(), screenPos);
}

void GraphicsModel::selectSprites(const QList<int>& ids)
{
	for (QGraphicsItem* item : _d->sprites.childItems())
	{
		GraphicsDrawItem* drawItem = (GraphicsDrawItem*)item;
		int id = drawItem->id();
		if (ids.contains(id))
		{
			drawItem->setSelected(true);
		}
		else
		{
			drawItem->setSelected(false);
		}
	}
}

// Mouse Press
// Start Draw
void GraphicsModel::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	_d->prevPos = event->scenePos();

	if (_d->drawMode == DrawRect)
	{
		if (!_d->drawRectStart)
		{
			GraphicsRectItem* item = new GraphicsRectItem(0);
			QPen pen(Qt::red, Qt::DotLine);
			item->setPen(pen);
			addSprite(item);
			_d->drawItem = item;
			_d->drawRectStart = true;
		}
		else
		{
		}

		//_d->drawRectStart = !_d->drawRectStart;
	}

	QGraphicsScene::mousePressEvent(event);
}

// End Draw
void GraphicsModel::mouseMoveEvent ( QGraphicsSceneMouseEvent * event)
{	
	// move map
	if (event->buttons() & Qt::LeftButton)
	{
		if (_d->drawMode == DrawRect)
		{
			if (_d->drawItem && _d->drawRectStart)
			{
				GraphicsRectItem* item = (GraphicsRectItem*)_d->drawItem;
				QRectF r = Figure::boundingRect(QLineF(_d->prevPos, event->scenePos()));
				item->setData(r);
			}
		}
		else if( _d->drawMode == DrawNone )
		{
			

			//QPointF df = _d->mousePos - event->pos();
			//_d->mousePos = event->pos();

			////this->scroll(event->x(), event->y() );
			//this->horizontalScrollBar()->setValue(horizontalScrollBar()->value() + df.x());
			//this->verticalScrollBar()->setValue(verticalScrollBar()->value() + df.y());
			//viewport()->update();
		}
	}

	// 100 msec 이내 변화는 처리하지 않는다.
	if (_d->moveTime.elapsed() > 100)
	{
		QPointF	pos = event->scenePos();
		pos.setY(pos.y()); //y를 화면아래쪽으로 증가하게 처리
		QPointF lonlat;
		if (_d->imageLayer)lonlat = _d->imageLayer->lonlat(pos);
		emit currentPosition(pos, lonlat);

		_d->moveTime.restart();
		update();
	}

	_d->mousePos = event->scenePos();
	QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsModel::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	if (_d->drawItem)
	{
		GraphicsDrawItem* item = dynamic_cast<GraphicsDrawItem*>(_d->drawItem);
		item->setPen(QPen(Qt::green));
		_d->drawRectStart = false;
		emit drawDone(item);
		_d->drawItem = 0;
	}

	if (_d->drawMode != DrawNone)
	{
		_d->drawMode = DrawNone;
		for (QGraphicsItem* item : _d->sprites.childItems())
		{
			item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
		}
	}

	QGraphicsScene::mouseReleaseEvent(event);

	// Update selected items
	QList<QGraphicsItem*> list = selectedItems();
	QList<int> selList;

	for (QGraphicsItem* item : list)
	{
		GraphicsDrawItem* drawItem = dynamic_cast<GraphicsDrawItem*>(item);
		if (drawItem)
		{
			selList << drawItem->id();
			if (drawItem->changed())
			{
				emit updated(drawItem);
				drawItem->resetChanged();
			}
		}
	}

	emit selected(selList);
}

void GraphicsModel::keyPressEvent(QKeyEvent * keyEvent)
{
	if (keyEvent->key() == Qt::Key_Delete) // Delete Items
	{
		QList<int> delList;
		for (QGraphicsItem* item : selectedItems())
		{
			if (item->type() == GraphicsDrawItem::PolygonType)
			{
				GraphicsPolygonItem* poly = (GraphicsPolygonItem*)item;
				bool deleted = poly->deleteSelectedPoint();
				if (!deleted) // Deleted selected Polygon
				{
					delList << poly->id();
					removeItem(item);
					delete item;
				}
				else //Deleted selected point of Polygon
				{
					emit updated(poly);
				}
			}
			else
			{
				GraphicsDrawItem* drawItem = (GraphicsDrawItem*)item;
				delList << drawItem->id();
				removeItem(item);
				delete item;
			}
		}

		if (!delList.isEmpty()) emit deleted(delList);
	}
	else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_C ) // Copy Items
	{
		_d->copies.clear();
		_d->copyRepeat = 1;
		for (QGraphicsItem* item : selectedItems())
		{
			GraphicsDrawItem* copy = dynamic_cast<GraphicsDrawItem*>(item);
			if(copy) _d->copies << copy;
		}
	}
	else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_V ) // Paste
	{
		//QList< GraphicsDrawItem* > copyList;
		for (GraphicsDrawItem* item : _d->copies)
		{
			item->setSelected(false);
			GraphicsDrawItem* copyItem = item->clone();
			copyItem->translate(_d->copyRepeat*10, _d->copyRepeat*10);
			copyItem->setSelected(true);
			//copyList << copyItem;
			addSprite(copyItem);
			copyItem->update();
			emit pasted(copyItem);
		}
		_d->copyRepeat++;

		//addSprites(copyList);
	}
	else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Z) // Undo
	{
		_d->undoStack.undo();
	}
	else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_Y) // Redo
	{
		_d->undoStack.redo();
	}
}
