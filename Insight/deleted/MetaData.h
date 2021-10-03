#pragma once

/*!
@file       MetaData.h
@date       2021.09.02
@author     Lucky
@version    v0.1
@brief		Satellite Image meta data defines header file
@details
*/

#include <QObject>
#include <QDateTime>
#include <QPolygonF>
#include "insight_global.h"

enum LineOrder { North2South, South2North };

enum SatImageFileType
{
	UNKNOWN_TYPE, 
	SINGLE_FILE,	// contains all channels(B, G, R) in one file
	MULTI_FILE		// B, G, R files are seperated
};

struct INSIGHT_EXPORT Period
{
	QDateTime from;
	QDateTime to;

	QDateTime mid() const
	{
		qint64 msec = from.msecsTo(to);
		QDateTime dt = from.addMSecs(msec / 2);
		return dt;
	}
};

/*!
@brief KOMPSAT Metadata from Aux File
*/
struct INSIGHT_EXPORT SatImageMeta
{
	SatImageFileType type;
	LineOrder lineOrder;
	Period period;
	QPolygonF geoBoundary;

	// Not Used Yet!
	double minValue;
	double maxValue;

	// For K5 L1A
	double calibrationConstant;
	double rescalingFactor;
};

class INSIGHT_EXPORT MetaData : public QObject
{
	Q_OBJECT

public:
	MetaData(QObject *parent);
	~MetaData();

	static SatImageMeta readK2Meta(const QString& auxFilePath);
	static SatImageMeta readK3Meta(const QString & auxFilePath);
};
