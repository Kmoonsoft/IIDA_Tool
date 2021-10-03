#include "Histogram.h"

void Histogram::findMaxHistogram()
{
	count = 0;
	maxHistogram = 0;
	for (qulonglong d : data)
	{
		count += d;
		if (maxHistogram < d) maxHistogram = d;
	}
}

int Histogram::finePercent(double ratio)
{
	if (count <= 0) findMaxHistogram();
	qulonglong per = count * ratio;

	qulonglong cnt = 0;
	int idx;
	for (int i = 0; i < data.count(); i++)
	{
		cnt += data[i];
		if (cnt >= per) return i;
	}

	return 0;
}
