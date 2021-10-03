#pragma once

/*!
@file       SatImgPainter.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief      Satellite Image Painter Header file
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211001	Lucky		First release(v0.1)
*/

#include <QImage>
#include <QThread>
#include "insight_global.h"
class GeoTiffFile;

/*!
@brief Satellite Imagery Painter Class
*/
class INSIGHT_EXPORT SatImgPainter : public QThread
{
	Q_OBJECT

public:
	SatImgPainter(QObject *parent=0);
	~SatImgPainter();

	// Set tiff bands (B, G, R)
	void setData(GeoTiffFile* multiBand);
	void setData(GeoTiffFile* blue, GeoTiffFile* green, GeoTiffFile* red);

	void setData(GeoTiffFile * tiff[3]);

	const uchar* image() const;
	void stop();

	static QImage::Format imageFormat();
	QImage thumbnail(int size = 100);

signals:
	void imageReady();

public slots:
	void setDrawRange(int min[3], int max[3]);
	void setDrawBoundary(const QRect & imgRect);

protected:
	void run();

private:

	void reload();
	void redraw();

	void drawColor(quint16* red, quint16* green, quint16* blue, int width, int height);
	void drawColor2(quint16* red, quint16* green, quint16* blue, int width, int height);

private:
	class SatImgDrawerData* _d;
};
