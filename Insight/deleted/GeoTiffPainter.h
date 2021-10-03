#pragma once

/*!
@file       GeoTiffPainter.h
@date       2021.09.01
@author     Lucky
@version    v0.1
@brief      Draw GeoTiff File Thread
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20210901	Lucky		First release(v0.1)
*/

#include <QPixmap>
#include <SatImgPainter.h>
#include "insight_global.h"

class INSIGHT_EXPORT GeoTiffPainter : public SatImgPainter
{
	Q_OBJECT

public:
	GeoTiffPainter(QObject *parent = Q_NULLPTR);
	~GeoTiffPainter();

	bool openTiff(const QString& multiBandTiffPath);
	bool openTiff(const QString& bluePath, const QString& greenPath, const QString& redPath);

	const class GeoTiffFile* tiff(int band) const;
	QPixmap pixmap() const;

	QRectF boundingRect() const;

private:
	class GeoTiffPainterData* _d;
};
