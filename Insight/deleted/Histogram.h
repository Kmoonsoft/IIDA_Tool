#pragma once
#include <qstringlist.h>
#include <qvector.h>
#include "insight_global.h"

struct INSIGHT_EXPORT HistogramData
{
	QVector<QVector<qulonglong>> histograms;
	QStringList names;
	QVector<double> steps;

	int noBin = 512;		// No of bins
	double min = 0.0;
	double max = 100.0;

	void clear()
	{
		histograms.clear();
		names.clear();
		steps.clear();
	};

	void prepare(int bandCount, int binCount, const QStringList& nameList)
	{
		clear();
		for (int i = 0; i < bandCount; i++)
		{
			histograms << QVector<qulonglong>();
			histograms[i].reserve(binCount);
			steps << 1.0;
		}

		noBin = binCount;
		names = nameList;
	};

	void setRange(double minValue, double maxValue)
	{
		min = minValue;
		max = maxValue;

		for (int i = 0; i < histograms.count(); i++)
		{
			steps[i] = (max - min) / noBin;
		}
	}

	void addHistogram(int index, double value)
	{
		int idx = (value - min) / steps[index];
		if (idx < 0) idx = 0;
		else if (idx >= histograms[index].count()) idx = histograms[index].count() - 1;
		histograms[index][idx]++;
	}
};


struct INSIGHT_EXPORT Histogram
{
	QVector<qulonglong> data;
	double min;
	double max;
	QString name;
	qulonglong maxHistogram;
	qulonglong count = 0;

	void findMaxHistogram();

	// ratio :0 ~ 1
	int finePercent(double ratio);
	
};

