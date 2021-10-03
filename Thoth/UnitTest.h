#pragma once

#include <QObject>

class UnitTest : public QObject
{
	Q_OBJECT

public:
	UnitTest(QObject *parent);
	~UnitTest();

	void testAll();
	void testThumbnail();
};
