/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include "GraphicsUndoCommand.h"

AddFigureCommand::AddFigureCommand( QGraphicsItem* item, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent )
	: QUndoCommand( parent )
{
	_item = item;
	_pItem = pItem;
	_scene = scene;
}

AddFigureCommand::~AddFigureCommand()
{	
}

void AddFigureCommand::undo()
{	
	_scene->removeItem( _item );
}

void AddFigureCommand::redo()
{
	_item->setParentItem( _pItem );
}

RemoveFigureCommand::RemoveFigureCommand( QList< QGraphicsItem* >& items, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent )
	: QUndoCommand( parent )
{
	_items = items;
	_pItem = pItem;
	_scene = scene;
}

RemoveFigureCommand::~RemoveFigureCommand()
{	
}

void RemoveFigureCommand::undo()
{	
	for( int i = 0 ; i < _items.count() ; i++ )		_items[ i ]->setParentItem( _pItem );
}

void RemoveFigureCommand::redo()
{
	for( int i = 0 ; i < _items.count() ; i++ )		_scene->removeItem( _items[ i ] );
	
}

AddFigureGroupCommand::AddFigureGroupCommand( QList< QGraphicsItem* >& cItems, QGraphicsItem* pItem, QGraphicsScene* scene/*, const QStringList& strItemIDList*/, QUndoCommand *parent )
	: QUndoCommand( parent )
{
	_cItems = cItems;
	_pItem = pItem;
	_scene = scene;

	_item = new QGraphicsItemGroup( _pItem );
	//_item->info()._strItemIDList = strItemIDList;
}

AddFigureGroupCommand::~AddFigureGroupCommand()
{	
}

void AddFigureGroupCommand::undo()
{	
	_scene->removeItem( _item );
	for( int i = 0 ; i < _cItems.count() ; i++ )
	{
		_cItems[ i ]->setPos( _cItems[ i ]->x() + _item->x(), _cItems[ i ]->y() + _item->y() );
		_cItems[ i ]->setParentItem( _pItem );
		_cItems[ i ]->setSelected( false );
	}
}

void AddFigureGroupCommand::redo()
{	
	_item->setParentItem( _pItem );
	for( int i = 0 ; i < _cItems.count() ; i++ )	_item->addToGroup( _cItems[ i ] );
}

RemoveFigureGroupCommand::RemoveFigureGroupCommand( QList< QGraphicsItemGroup* > items, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent )
	: QUndoCommand( parent )
{
	for( int i = 0 ; i < items.count() ; i++ )
	{
		_cItems << items[ i ]->childItems();
	}
	
	_items = items;
	_pItem = pItem;
	_scene = scene;
}

RemoveFigureGroupCommand::~RemoveFigureGroupCommand()
{	
}

void RemoveFigureGroupCommand::undo()
{	
	for( int i = 0 ; i < _items.count() ; i++ )
	{
		_items[ i ]->setParentItem( _pItem );
		for( int j = 0 ; j < _cItems[ i ].count() ; j++ )	_items[ i ]->addToGroup( _cItems[ i ][ j ] );
	}
}

void RemoveFigureGroupCommand::redo()
{
	for( int i = 0 ; i < _items.count() ; i++ )
	{
		_scene->removeItem( _items[ i ] );
		for( int j = 0 ; j < _cItems[ i ].count() ; j++ )
		{
			_cItems[ i ][ j ]->setPos( _cItems[ i ][ j ]->x() + _items[ i ]->x(), _cItems[ i ][ j ]->y() + _items[ i ]->y() );
			_cItems[ i ][ j ]->setParentItem( _pItem );
			_cItems[ i ][ j ]->setSelected( false );
		}
	}
}

MoveFigureCommand::MoveFigureCommand( QList< QGraphicsItem* >& items, QList< QPointF >& oldPoints, QList< QPointF >& newPoints, QUndoCommand *parent )
	: QUndoCommand( parent )
{
	_items = items;
	_oldPoints = oldPoints;
	_newPoints = newPoints;
}

MoveFigureCommand::~MoveFigureCommand()
{	
}

void MoveFigureCommand::undo()
{	
	for( int i = 0 ; i < _items.count() ; i++ )	_items[ i ]->setPos( _oldPoints[ i ] );
}

void MoveFigureCommand::redo()
{
	for( int i = 0 ; i < _items.count() ; i++ )	_items[ i ]->setPos( _newPoints[ i ] );
}
