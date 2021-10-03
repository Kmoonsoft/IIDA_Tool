#include <QFile>
#include <QTextStream>
#include <QtXml>
#include "MetaData.h"

MetaData::MetaData(QObject *parent)
	: QObject(parent)
{
}

MetaData::~MetaData()
{
}

QPointF goeCoordStringToPoint(const QString&  str)
{
	QPointF p;
	QStringList list = str.split(',');// str.split(QRegExp("[\\s+|,]"));
	if (list.count() < 2) return p;

	p.setX(list[1].toDouble()); // Lon
	p.setY(list[0].toDouble()); // Lat

	return p;
}

SatImageMeta MetaData::readK2Meta(const QString & auxFilePath)
{
	SatImageMeta meta;
	//AUX_IMAGE_TL_LATLONG_DEG	  37.01766698  126.22724020
	//AUX_IMAGE_TR_LATLONG_DEG	  37.02245601  126.44024565
	//AUX_IMAGE_BL_LATLONG_DEG	  36.84745587  126.23340360
	//AUX_IMAGE_BR_LATLONG_DEG	  36.85221555  126.44593688

	//AUX_STRIP_ACQ_DATE_UT	20201212
	//AUX_STRIP_ACQ_START_UT	004616.446659
	//AUX_STRIP_ACQ_END_UT	004825.445436

	QFile f(auxFilePath);

	meta.type = SINGLE_FILE;

	if (!f.open(QFile::ReadOnly | QFile::Text))
	{
		return meta;
	}

	QTextStream s(&f);
	QString line;
	QMap<QString, QString> map;
	while (s.readLineInto(&line))
	{
		QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
		if (list.count() < 2) continue;

		map[list[0]] = list.mid(1).join(',');
	}

	f.close();

	QString date = map["AUX_STRIP_ACQ_DATE_UT"];
	QString from = map["AUX_STRIP_ACQ_START_UT"];
	int dot = from.lastIndexOf('.');
	if (dot > 0) from = from.left(dot);

	QString to = map["AUX_STRIP_ACQ_END_UT"];
	dot = to.lastIndexOf('.');
	if (dot > 0) to = to.left(dot);

	meta.period.from = QDateTime::fromString(date + from, "yyyyMMddhhmmss");
	meta.period.to = QDateTime::fromString(date + to, "yyyyMMddhhmmss");

	QPointF tl = goeCoordStringToPoint(map["AUX_IMAGE_TL_LATLONG_DEG"]);

	meta.geoBoundary << tl;
	meta.geoBoundary << goeCoordStringToPoint(map["AUX_IMAGE_TR_LATLONG_DEG"]);
	meta.geoBoundary << goeCoordStringToPoint(map["AUX_IMAGE_BR_LATLONG_DEG"]);
	meta.geoBoundary << goeCoordStringToPoint(map["AUX_IMAGE_BL_LATLONG_DEG"]);
	meta.geoBoundary << tl;

	return meta;
}

SatImageMeta MetaData::readK3Meta(const QString & auxFilePath)
{
	QFile f(auxFilePath);
	SatImageMeta meta;
	meta.type = MULTI_FILE;

	if (!f.open(QFile::ReadOnly | QFile::Text))
	{
		return meta;
	}

	QString xmlString = f.readAll();
	f.close();

	QDomDocument document;
	document.setContent(xmlString);
	QDomElement root = document.firstChildElement();

	//qDebug() << root.tagName() << root.text();
	QDomElement image = root.firstChildElement("Image");
	//qDebug() << image.tagName();
	QDomElement pan = image.firstChildElement("PAN");
	//qDebug() << pan.tagName();
	QDomElement imagetime = pan.firstChildElement("ImagingTime");
	//qDebug() << imagetime.tagName();
	QDomElement startTime = imagetime.firstChildElement("ImagingStartTime");
	//qDebug() << startTime.tagName();
	QDomElement fromUTC = startTime.firstChildElement("UTC");
	//qDebug() << fromUTC.tagName();
	QString from = fromUTC.text();
	int dot = from.lastIndexOf('.');
	if (dot > 0) from = from.left(dot);
	meta.period.from = QDateTime::fromString(from, "yyyyMMddhhmmss");

	QDomElement endTime = imagetime.firstChildElement("ImagingEndTime");
	QDomElement toUTC = endTime.firstChildElement("UTC");
	QString to = toUTC.text();
	dot = to.lastIndexOf('.');
	if (dot > 0) to = to.left(dot);
	meta.period.to = QDateTime::fromString(to, "yyyyMMddhhmmss");
	if (dot > 0) meta.period.to = meta.period.to.addSecs(1);

	//qDebug() << meta.period.from.toString("yyyy-MM-dd hh:mm:ss");
	//qDebug() << meta.period.to.toString("yyyy-MM-dd hh:mm:ss");

	return meta;
}
