/*!
@file       GeoTiffFile.cpp
@date       2021.09.01
@author     Lucky
@version    v0.1
@brief      Implementation of GeoTiff file manipulation
@details	Using GDAL Library, manipulate Geotiff files
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v0.1)
*/

#include <QtDebug>
#include <QFileInfo>
#include <QVector2D>
#include <QVector3D>
#include <QTransform>
#include <QQuaternion>

#include "GeoTiffFile.h"

QMutex gMutex;

/*!
@brief Constructor
@details Initialize the GDAL Library
*/
GeoTiffFile::GeoTiffFile(QObject *parent)
	: QObject(parent)
{
}

/*!
@brief Desttuctor
@details Close Tiff and Destroy GDAL Library
*/
GeoTiffFile::~GeoTiffFile()
{
	close();
}

/*!
@brief Open Tiff FIle
@param filePath : The full path(path + filename) of the tiff file
*/
bool GeoTiffFile::open(const QString & filePath)
{
	if (_tiff != 0) close();

	if (!QFile::exists(filePath))
	{
		qWarning() << "No exist the tiff file : " + filePath;
		return false;
	}

	_tiff = (GDALDataset*)GDALOpen(filePath.toLatin1(), GA_ReadOnly);

	if (_tiff == 0)
	{
		qWarning() << "Fail to open the tiff file : " + filePath;
		return false;
	}

	qDebug() << "Open Tiff : " << filePath << (qlonglong)(_tiff);

	//char **metadata = _tiff->GetMetadata("SUBDATASETS");

	_nrows = GDALGetRasterYSize(_tiff);		// Image Height
	_ncols = GDALGetRasterXSize(_tiff);		// Image Width
	_nlevels = GDALGetRasterCount(_tiff);	// No. of bands

	//_ncols = _nrows = 4096;

	_dimensions[0] = _ncols;
	_dimensions[1] = _nrows;
	_dimensions[2] = _nlevels;
	qDebug() << QString("Size: %1x%2, Bands:%3").arg(_ncols).arg(_nrows).arg(_nlevels);
	qDebug() << GDALGetDataTypeName(GDALGetRasterDataType(_tiff->GetRasterBand(1)));

	_tiff->GetGeoTransform(_geotransform);
	_fName = filePath;

	qDebug() << _tiff->GetProjectionRef();

	// 좌표계 파일 좌표계
	OGRSpatialReference source = OGRSpatialReference(_tiff->GetProjectionRef());
	//source.importFromWkt(_tiff->GetProjectionRef());
	source.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);

	// WGS 84 좌표계
	OGRSpatialReference* target;
	target = source.CloneGeogCS();

	// Sentinel-1과 같이 좌표계가 없는 경우.. 프로그램 Fault 를 막기 위해 처리함.
	if (target == 0) target = source.Clone();

	target->SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
	//target.importFromEPSG(4326);

	if (_image2WGS84Trans)
	{
		delete _image2WGS84Trans;
		_image2WGS84Trans = 0;
	}
	_image2WGS84Trans = OGRCreateCoordinateTransformation(&source, target);

	if (_WGS842ImageTrans)
	{
		delete _WGS842ImageTrans;
		_WGS842ImageTrans = 0;
	}
	_WGS842ImageTrans = OGRCreateCoordinateTransformation(target, &source);

	QSize s = size();
	int h = size().height() - 1;
	int w = size().width() - 1;
	QPointF topLeft = latLon(0, h);
	QPointF topRight = latLon(w, h);
	QPointF bottomRight = latLon(w, 0);
	QPointF bottomleft = latLon(0, 0);
	_boundary << topLeft << topRight << bottomRight << bottomleft << topLeft;

	return true;
}

bool GeoTiffFile::isOpen() const
{
	return (_tiff != 0);
}

/*!
@brief Get Projection
*/
QString GeoTiffFile::projection() const
{
	QString proj(_tiff->GetProjectionRef());
	return proj;
}

/*!
@brief Get Transform
@details 6-elements geotranform array.
*/
const double * GeoTiffFile::geoTransform() const
{
	return _geotransform;
}

double GeoTiffFile::resolution() const
{
	return fabs(_geotransform[1]);
}

GDALDataType GeoTiffFile::bandType(int z)
{
	GDALRasterBand* band = _tiff->GetRasterBand(z);
	if (band == 0) return GDT_Unknown;
	GDALDataType dataType = GDALGetRasterDataType(band);
	return dataType;
}

/*!
@brief  Get Value for Null data
*/
double GeoTiffFile::noDataValue() const
{
	if (_tiff == 0) return 0;
	return (double)_tiff->GetRasterBand(1)->GetNoDataValue();
}

const int * GeoTiffFile::dimensions() const
{
	return _dimensions;
}

QSize GeoTiffFile::size() const
{
	return QSize(_dimensions[0], _dimensions[1]);
}

int GeoTiffFile::bandCount() const
{
	return _dimensions[2];
}

/*!
@brief Get the uint(usigned 2bytes) data of Band
@param z : z-order or band index
*/
quint16 * GeoTiffFile::data(int z) const
{
	return (quint16 *)data(z, 0, 0, _ncols, _nrows);
}

/*!
@brief Get Box Data(x, y, x+width, y+height)
*/
void * GeoTiffFile::data(int z, int x, int y, int width, int height) const
{
	if (_tiff == 0) return 0;
	if (z > _nlevels) return 0;
	if (x < 0 || (x + width) > _ncols || y < 0 || (y + height) > _nrows) return 0;

	// Data Lock For Multi Thread(GUI and Image Worker)
	QMutexLocker lock(&gMutex);

	GDALRasterBand* band = _tiff->GetRasterBand(z);
	if (band == 0) return 0;

	GDALDataType dataType = GDALGetRasterDataType(band);

	int pbs;
	//qDebug() << band->GetScale(&pbs);

	int nbytes = GDALGetDataTypeSizeBytes(dataType);

	int imageWidth = qMin(width, _ncols - x);
	int imageHeight = qMin(height, _nrows - y);

	if(nbytes == 0 ) return 0;

	int w = imageWidth;
	int h = imageHeight;

	void* data = CPLMalloc(w * h * nbytes);

	CPLErr e = band->RasterIO(GF_Read, x, y, imageWidth, imageHeight, data, w, h, dataType, 0, 0);
	if (e == CE_None)
	{
		return data;
	}

	CPLFree(data);
	return 0;
}

quint16 * GeoTiffFile::dataUint16(int z, int x, int y, int width, int height) const
{
	void* rst = data(z, x, y, width, height);
	if (rst == 0) return 0;

	size_t size = width * height* sizeof(quint16);
	quint16* d = new quint16[size];
	memcpy(d, rst, size);
	CPLFree(rst);
	return d;
}

quint16 * GeoTiffFile::data(int z, const QRect & rect) const
{
	return (quint16 *)data(z, rect.x(), rect.y(), rect.width(), rect.height());
}

quint16 GeoTiffFile::value(int x, int y, int z)
{
	if (_tiff == 0) return 0;
	if (x < 0 || x >= _ncols || y < 0 || y >= _nrows) return 0;

	GDALDataType bandType = GDALGetRasterDataType(_tiff->GetRasterBand(z));
	if (bandType != GDT_UInt16) return 0;

	quint16 data;
	GDALRasterBand* band = _tiff->GetRasterBand(z);
	CPLErr e = band->RasterIO(GF_Read, x, y, 1, 1, &data, 1, 1, bandType, 0, 0);
	
	if (e == CE_None) return data;
	return 0;
}

ImageData GeoTiffFile::band(int z) const
{
	ImageData img;
	void* rst = data(z);
	if (rst == 0) return img;

	img.width = _ncols;
	img.height = _nrows;
	img.data = rst;
	return img;
}

ImageData GeoTiffFile::overview(int z, uint size)
{
	GDALRasterBand *band = _tiff->GetRasterBand(z);

	GDALDataType dataType = GDALGetRasterDataType(band);
	int nbytes = GDALGetDataTypeSizeBytes(dataType);

	void* data = CPLMalloc(size * size * nbytes);
	int xStep = _ncols / size;
	int yStep = _nrows / size;

	CPLErr e = band->RasterIO(GF_Read, 0, 0, _ncols, _nrows, data, size, size, dataType, 0, 0);

	ImageData img;

	//QString msg;
	if (e == CE_None)
	{
		img.width = size;
		img.height = size;

		img.data = data;

	}
	else
	{
		CPLFree(data);
	}

	return img;
}

/*!
@brief Crop 
@param topLeftX : Start X-position in UTM(meter)
@param topLeftY : Start Y-position in UTM(meter)
@param width : Width(meter)
@param height : Height(meter)
@param outFile : Output file path
*/
void GeoTiffFile::crop(double topLeftX, double topLeftY, double width, double height, const QString& outFile) const
{
	GDALDataset *dstTiff;
	GDALDriver *pDriver = _tiff->GetDriver();

	//pInputRaster = (GDALDataset*)GDALOpen(inputPath, GA_ReadOnly);

	//the affine transformation information, you will need to adjust this to properly
	//display the clipped raster
	double transform[6];
	_tiff->GetGeoTransform(transform);

	//adjust top left coordinates
	transform[0] = topLeftX;
	transform[3] = topLeftY;


	//determine dimensions of the new (cropped) raster in cells
	int xSize = round(width / transform[1]);
	int ySize = round(height / transform[1]);

	//create the new (cropped) dataset
	dstTiff = pDriver->Create(outFile.toLatin1(), xSize, ySize, 1, GDT_Float32, NULL); //or something similar

	dstTiff->SetGeoTransform(transform);

	const char* proj = _tiff->GetProjectionRef();
	if (proj != NULL && strlen(proj) > 0)
	{
		dstTiff->SetProjection(proj);
	}

	dstTiff->SetMetadata(_tiff->GetMetadata());

	//for (int i = 0; i < _tiff->GetRasterCount(); i++)
	//{
	//	GDALRasterBand* srcBand = _tiff->GetRasterBand(i + 1);
	//	GDALDataType eBandType = srcBand->GetRasterDataType();
	//	dstTiff->AddBand(eBandType, NULL);
	//	GDALRasterBand* dstBand = dstTiff->GetRasterBand(i + 1);
	//	dstBand->copy
	//	dstBand->AddSimpleSource(poSrcBand,
	//		xoff, yoff,
	//		xsize, ysize,
	//		0, 0, xsize, ysize);
	//	poVRTBand->CopyCommonInfoFrom(poSrcBand);

	//	// Again, I copy my own metadata
	//	papszMD = poSrcBand->GetMetadata(MD_DOMAIN_MSAT);
	//	if (papszMD != NULL)
	//		poVRTBand->SetMetadata(papszMD, MD_DOMAIN_MSAT);
	//}

	//GDALRasterBand* band = _tiff->GetRasterBand(1);


	//now all you have to do is find the number of columns and rows the top left corner
	//of the cropped raster if offset from the original raster, and use those values to 
	//read data from the original raster and copy/write the data to the new (cropped) raster
}

/*!
@brief Cropping
*/
void GeoTiffFile::cropImage(int xTopLeft, int yTopLeft, int width, int height, const QString & outFile) const
{
	GDALDataset *dstTiff;
	GDALDriver *pDriver = _tiff->GetDriver();

	if (_nlevels <= 0)
	{
		qDebug() << "Crop Image! No Band Data";
		return;
	}
	//pInputRaster = (GDALDataset*)GDALOpen(inputPath, GA_ReadOnly);

	//the affine transformation information, you will need to adjust this to properly
	//display the clipped raster
	double scale = _geotransform[1];
	double transform[6];
	_tiff->GetGeoTransform(transform);

	//adjust top left coordinates
	transform[0] += xTopLeft * _geotransform[1];
	transform[3] += yTopLeft * _geotransform[5];

	int bandCount = _tiff->GetRasterCount();

	//create the new (cropped) dataset
	dstTiff = pDriver->Create(outFile.toLatin1(), width, height, bandCount, _tiff->GetRasterBand(1)->GetRasterDataType(), NULL); //or something similar

	dstTiff->SetGeoTransform(transform);

	const char* proj = _tiff->GetProjectionRef();
	if (proj != NULL && strlen(proj) > 0)
	{
		dstTiff->SetProjection(proj);
	}

	dstTiff->SetMetadata(_tiff->GetMetadata());

	for (int i = 0; i < bandCount; i++)
	{
		GDALRasterBand* srcBand = _tiff->GetRasterBand(i + 1);
		GDALDataType eBandType = srcBand->GetRasterDataType();
		dstTiff->AddBand(eBandType, NULL);
		GDALRasterBand* dstBand = dstTiff->GetRasterBand(i + 1);
		size_t size = GDALGetDataTypeSizeBytes(eBandType) * width * height;
		void* buffer = CPLMalloc(size); //new char[size];// 확인...
		
		//read data
		srcBand->RasterIO(GF_Read, xTopLeft, yTopLeft, width, height, buffer, width, height, eBandType, 0, 0, nullptr);
		// write data
		dstBand->RasterIO(GF_Write, 0, 0, width, height, buffer, width, height, eBandType, 0, 0, nullptr);

		char** meta = srcBand->GetMetadata();

		if (meta != NULL)
		{
			dstBand->SetMetadata(meta);
		}

		CPLFree(buffer);
		//delete[] buffer;
	}

	GDALClose(dstTiff);
}

/*!
@brief Crop TIFF Image(Static)
*/
void GeoTiffFile::cropImage(const QString & filePath, const QString & suffix, int cropWidth, int cropHeight)
{
	QFileInfo fi(filePath);
	QString fileName = fi.absolutePath() + "/" + fi.baseName();

	GeoTiffFile file;

	bool ok = file.open(filePath);

	if (ok)
	{
		const int* sizes = file.dimensions();

		int maxX = sizes[0] / cropWidth;
		if (sizes[0] % cropWidth) maxX += 1;
		int maxY = sizes[1] / cropHeight;
		if (sizes[1] % cropHeight) maxY += 1;

		for (int i = 0; i < maxX; i++)
		{
			int x = cropWidth * i;
			int width = cropWidth;
			if (sizes[0] < cropWidth * (i + 1)) width = sizes[0] - x;

			for (int j = 0; j < maxY; j++)
			{
				int y = cropHeight * j;
				int height = cropHeight;
				if (sizes[1] < cropHeight * (j + 1)) height = sizes[1] - y;

				QString subIdx = suffix + QString("%1%2.%3").arg(i, 2, 10, QChar('0')).arg(j, 2, 10, QChar('0')).arg(fi.suffix());
				file.cropImage(x, y, width, height, fileName + subIdx);

			}
		}

	}
}

// Input x:Image X, y:Image Y
QPointF GeoTiffFile::latLon(double x, double y) const
{
	if (_image2WGS84Trans == 0) return QPointF();

	// convert geo grid X, Y
	double xp = _geotransform[1] * x + _geotransform[2] * y + /*_geotransform[1] * 0.5 + _geotransform[2] * 0.5 + */_geotransform[0];
	double yp = _geotransform[4] * x + _geotransform[5] * y + /*_geotransform[4] * 0.5 + _geotransform[5] * 0.5 +*/ _geotransform[3];

	// Transform
	_image2WGS84Trans->Transform(1, &xp, &yp);

	// Return lon, lat point
	//return QPointF(yp, xp);
	return QPointF(xp, yp);
}

/*!
@brief Get the image position on geographical point(lon/lat)
*/
QPointF GeoTiffFile::point(double lon, double lat) const
{
	if (_WGS842ImageTrans == 0) return QPoint();

	double x = lon;
	double y = lat;
	_WGS842ImageTrans->Transform(1, &x, &y);

	x = (((x - _geotransform[0]) / _geotransform[1]));
	y = (((y - _geotransform[3]) / _geotransform[5]));

	return QPointF(x, y);
}

QRect GeoTiffFile::imageRect() const
{
	QRect r(0, 0, _ncols, _nrows);
	return r;
}

QRectF GeoTiffFile::getUtmRect() const
{
	QRectF r(_geotransform[0], _geotransform[3], _ncols * _geotransform[1], _nrows * _geotransform[5]);
	return r;
}

/*!
@brief Get Geographical Boundary Rectangle(Lon/Lat coordinate)
*/
QPolygonF GeoTiffFile::geoBoundary() const
{
	return _boundary;
}

void GeoTiffFile::setBoundary(const QPolygonF & poly)
{
	_boundary = poly;
}

double GeoTiffFile::minValue(int z)
{
	if (_tiff == 0) return 0;
	int success = 0;
	GDALRasterBand* srcBand = _tiff->GetRasterBand(z);
	if (srcBand == 0) return 0;

	double min = srcBand->GetMinimum(&success);
	
	double max, mean, stddev;
	if (success == false)
	{
		CPLErr err = srcBand->GetStatistics(true, true, &min, &max, &mean, &stddev);
	}

	return min;
}

double GeoTiffFile::maxValue(int z)
{
	if (_tiff == 0) return 0;
	int success = 0;
	GDALRasterBand* srcBand = _tiff->GetRasterBand(z);
	if (srcBand == 0) return 0;

	double max = srcBand->GetMaximum(&success);
	if (success == false)
	{
		double min, mean, stddev;
		CPLErr err = srcBand->GetStatistics(true, true, &min, &max, &mean, &stddev);
	}

	return max;
}

const double * GeoTiffFile::statistics(int z)
{
	GDALRasterBand* srcBand = _tiff->GetRasterBand(z);
	if (srcBand == 0) return 0;
	CPLErr err = srcBand->GetStatistics(true, true, &_statistics[0], &_statistics[1], &_statistics[2], &_statistics[3]);
	if (err == CPLErr::CE_None) return _statistics;
	return 0;
}

/*!
@brief Close Tiff file
*/
void GeoTiffFile::close()
{
	qDebug() << "Close Tiff: " << _fName << (qlonglong)(_tiff);
	if (_tiff)
	{
		GDALClose(_tiff);
		_tiff = 0;

		_image2WGS84Trans = 0;
		_WGS842ImageTrans = 0;
	}
}

/*!
@brief Get Opend Tiff File Full Path
*/
QString GeoTiffFile::fileName() const
{
	return _fName;
}
