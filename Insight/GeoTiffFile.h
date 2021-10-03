#pragma once
/*!
@file       GeoTiffFile.h
@date       2021.09.01
@author     Lucky
@version    v0.1
@brief      GeoTiff file class
@details
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QObject>
#include <QPolygonF>
#include <QSize>
#include <QPointF>
#include <QtDebug>
#include <QMutex>
#include <QImage>

#include "geotiff.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdalwarper.h"

#include "insight_global.h"

struct ImageData
{
	void* data = 0;
	int width;
	int height;

	void free()
	{
		if (data)
		{
			CPLFree(data);
		}
	}
};

/*!
@brief      GeoTiff file class
*/
class INSIGHT_EXPORT GeoTiffFile : public QObject
{
	Q_OBJECT

public:
	GeoTiffFile(QObject *parent = 0);
	~GeoTiffFile();

// Static Memethods
	// Crop Tiff Image
	static void cropImage(const QString& filePath, const QString& suffix, int width, int height);


// Public APIs
	// Open 
	bool open(const QString& filePath);
	bool isOpen() const;

	// close
	void close();

	// Get Tiff File Full Path(include path and file name)
	QString fileName() const;
	
	// Get Projection 
	QString projection() const;

	// Get Geographical Transform
	const double* geoTransform() const;
	double resolution() const;

	GDALDataType bandType(int z = 1);

	// Get Value for Null data
	double noDataValue() const;

	// return X, Y, Z
	const int* dimensions() const;
	
	// Return Size of image(Width x Height)
	QSize size() const;

	// Return No. of Bands
	int bandCount() const;

	// Get Band Data of z-th order band
	quint16* data(int z) const;
	// Get box(x, y, x+width, y+height)
	void* data(int z, int x, int y, int width, int height) const;
	quint16* dataUint16(int z, int x, int y, int width, int height) const;
	quint16* data(int z, const QRect& rect) const;
	quint16 value(int x, int y, int z = 1);

	ImageData band(int z) const;

	ImageData overview(int z, uint size);

	// Geographical Crop with physcal coordinate
	void crop(double topLeftX, double topLeftY, double width, double height, const QString& outFile) const;

	// Image Crop with image coordinate
	void cropImage(int xTopLeft, int yTopLeft, int width, int height, const QString& outFile) const;

	QPointF latLon(double x, double y) const;
	QPointF point(double lon, double lat) const;

	QRect imageRect() const; // Image rect(0, 0, size)
	QRectF getUtmRect() const; // UTM Rect

	//QRectF geoBoundary() const;
	QPolygonF geoBoundary() const; // Lon/Lat Bounday Polygon
	void setBoundary(const QPolygonF& poly);

	double minValue(int z = 1);
	double maxValue(int z = 1);
	const double* statistics(int z = 1);

protected:
	GDALDataset* _tiff = 0;
	double		_geotransform[6];      // 6-element geotranform array.
	int			_dimensions[3];           // X,Y, and Z dimensions.
	int			_nrows, _ncols, _nlevels; // dimensions of data in Geotiff. 
	QString		_fName; // file name
	double		_statistics[4]; // Ελ°θ,min, max, mean, stddev
	QPolygonF	_boundary;

	OGRCoordinateTransformation* _image2WGS84Trans = 0;
	OGRCoordinateTransformation* _WGS842ImageTrans = 0;

	QMutex _mutex;
};
