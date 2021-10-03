#pragma once

/*!
@file       GraphicsModel.h
@date       2021.09.01
@author     ACJ
@version    v0.1
@brief		Graphics Item Model Class Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	ACJ		First release(v0.1)
*/

#include <QGraphicsScene>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariantList>

#include "gaia_global.h"

class GraphicsLayer;
class ImageLayer;
class GraphicsDrawItem;

class GAIA_EXPORT GraphicsModel : public QGraphicsScene
{
   Q_OBJECT

public:
	
	/*!
	@brief Draw Mode with mouse
	*/
	enum DrawMode
	{
		DrawNone, DrawRect, DrawPoygon, DrawMerge, DrawSync, DrawBuffer
	};

    GraphicsModel(QObject *parent=0);
    ~GraphicsModel();

    void	addLayer(GraphicsLayer* layer);
    void	removeLayer(GraphicsLayer* layer);
    void	removeLayer(const QString& id);

    GraphicsLayer*		findLayer(const QString& id);
	GraphicsDrawItem *	sprite(int id) const;

    GraphicsLayer*		topLayer() const;
    void			setImageLayer(ImageLayer* imageLayer);

	void			addSprite(GraphicsDrawItem* item);
	void			addSprites(QList<GraphicsDrawItem*>& items, bool addUndoStack = true);
	void			resetUndoStack();

	/*!@brief Return Scene Mouse position*/
	QPointF			mousePosition() const;


	DrawMode		drawMode() const;

	void			requestContextMenu(GraphicsDrawItem* item, const QPoint& screenPos);

signals:
	void			currentPosition(QPointF scenePos, QPointF lonLat);
	void			drawDone(GraphicsDrawItem*);
	void			updated(GraphicsDrawItem*);
	void			selected(const QList<int> ids);
	void			deleted(const QList<int> ids);
	void			pasted(GraphicsDrawItem*);
	void			contextMenuRequested(int id, const QPoint& screenPos);

public slots:
	void			clearSprites();
	void			selectSprites(const QList<int>& ids);
	void			drawRect(bool on);

protected:
	void			mousePressEvent(QGraphicsSceneMouseEvent *event);
    void			mouseMoveEvent ( QGraphicsSceneMouseEvent * event);
	void			mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	
	void			keyPressEvent(QKeyEvent *keyEvent);

private:
	class GraphicsModelData* _d;
};

