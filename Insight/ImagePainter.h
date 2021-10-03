#pragma once

/*!
@file       ImagePainter.h
@date       2021.09.23
@author     Lucky
@version    v0.1
@brief      Image(RGB) Painter Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QImage>
#include <QThread>
#include "GeoTiffFile.h"
#include "insight_global.h"

/*!
@brief Image that has RGB band Painter Class
*/
class INSIGHT_EXPORT ImagePainter : public QThread
{
	Q_OBJECT

public:
	ImagePainter(QObject *parent=0);
	~ImagePainter();

	/*!
	@brief Open Tiff Image file
	@param path : Absolute Image path
	*/
	bool open(const QString& path);

	/*!
	@brief Get Image
	*/
	QImage image() const;

	/*!
	@brief Get Image Format
	*/
	static QImage::Format imageFormat();

	/*!
	@brief Get Thumbnail Image
	*/
	QImage thumbnail(int size = 100);

	/*!
	@brief Get Boundary(UTM grid)
	*/
	QRectF utmRect() const;

	/*!
	@brief Get Lon/Lot Postion with image coordinate(x, y)
	*/
	QPointF lonlat(const QPointF& pos) const;

signals:
	void imageReady();

protected:
	void run(); //Start Thread

private:
	void draw(); // Draw Image from tiff R/G/B bnad
	void drawMono();

protected:

	GeoTiffFile _file = 0;
	QImage _img;
};
