#pragma once

/*!
@file       GraphicsUndoCommand.h
@date       2021.09.15
@author     ACJ
@version    v0.1
@brief      Graphics Item Undo Command Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ			First release(v0.1)
*/

#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItemGroup>

/*!
@brief Add Single Item Command class
*/
class AddFigureCommand : public QUndoCommand
{	
public:
    AddFigureCommand( QGraphicsItem* item, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent = 0 );
    ~AddFigureCommand();
    void undo();
    void redo();

private:
    QGraphicsItem*					_item;
	QGraphicsItem*					_pItem;
    QGraphicsScene*					_scene;   
};

/*!
@brief Remove Items Command class
*/
class RemoveFigureCommand : public QUndoCommand
{	
public:
    RemoveFigureCommand( QList< QGraphicsItem* >& items, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent = 0 );
    ~RemoveFigureCommand();
    void undo();
    void redo();

private:
    QList< QGraphicsItem* > _items;
	QGraphicsItem*			_pItem;
    QGraphicsScene*			_scene;
};

/*!
@brief Add Multi-Items Command class
*/
class AddFigureGroupCommand : public QUndoCommand
{	
public:
    AddFigureGroupCommand( QList< QGraphicsItem* >& cItems, QGraphicsItem* pItem, QGraphicsScene* scene/*, const QStringList& strItemIDList*/, QUndoCommand *parent = 0 );
    ~AddFigureGroupCommand();
    void undo();
    void redo();

private:
    QList< QGraphicsItem* > _cItems;
	QGraphicsItemGroup*		_item;
	QGraphicsItem*			_pItem;
	QGraphicsScene*			_scene;
};

/*!
@brief Remove Multi-Items Command class
*/
class RemoveFigureGroupCommand : public QUndoCommand
{	
public:
    RemoveFigureGroupCommand( QList< QGraphicsItemGroup* > items, QGraphicsItem* pItem, QGraphicsScene* scene, QUndoCommand *parent = 0 );
    ~RemoveFigureGroupCommand();
    void undo();
    void redo();

private:
    QList< QList< QGraphicsItem* > > _cItems;
	QList< QGraphicsItemGroup* > _items;
	QGraphicsItem*			_pItem;
	QGraphicsScene*			_scene;
};

/*!
@brief Move Item Command class
*/
class MoveFigureCommand : public QUndoCommand
{	
public:
    MoveFigureCommand( QList< QGraphicsItem* >& items, QList< QPointF >& oldPoints, QList< QPointF >& newPoints, QUndoCommand *parent = 0 );
    ~MoveFigureCommand();
    void undo();
    void redo();

private:
	QList< QGraphicsItem* > _items;
	QList< QPointF > _oldPoints;
	QList< QPointF > _newPoints;
};


