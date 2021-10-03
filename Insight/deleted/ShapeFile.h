#pragma once

#include <QObject>
#include <QVector>
#include <QPolygonF>

#include "gdal_priv.h"
#include "gdalwarper.h"
#include "ogrsf_frmts.h"
#include "insight_global.h"

class INSIGHT_EXPORT ShapeFile : public QObject
{
	Q_OBJECT

public:
	ShapeFile(QObject *parent=0);
	~ShapeFile();

	bool open(const QString& filePath);
	void close();

	QList<QPolygonF> getShape(const QPolygonF& boundary, int layer = 0) const;

private:
	GDALDataset* _shp = 0;
	QString _fName;
};
