#pragma once

/*!
@file       SatImageItem.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Satellite Image(Tiff image) Graphics item Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20210902	Lucky		First release(v0.1)
*/

#include <QObject>
#include <QGraphicsPixmapItem>
#include "gaia_global.h"

class GAIA_EXPORT SatImageItem : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	Q_CLASSINFO("DefaultProperty", "children")
	Q_INTERFACES(QGraphicsItem)

public:
	SatImageItem(QGraphicsItem* parent = Q_NULLPTR);
	~SatImageItem();
	
	using QObject::children;

	bool openImage(const QString& multiBandTiffPath);
	bool openImage(const QString& bluePath, const QString& greenPath, const QString& redPath);
	
	//QRectF boundingRect() const;

	void moveToPos();

	QPointF lonlat(const QPointF& pos);

signals:
	void imageReady();

private:
	void draw();

private:
	class SatImageItemData* _d;
};
