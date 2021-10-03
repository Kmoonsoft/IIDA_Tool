#include "ImagePainter.h"
#include "GeoTiffFile.h"

#include "UnitTest.h"

struct UnitTestData
{

};

UnitTest::UnitTest(QObject *parent)
	: QObject(parent)
{
}

UnitTest::~UnitTest()
{
}

void UnitTest::testAll()
{
	testThumbnail();
}

void UnitTest::testThumbnail()
{
	QString fname = "C:\\Data\\tile\\OD08\\tile\\test_10_104001002E88FE00_1.tif";
	
	ImagePainter p;
	p.open(fname);

	QImage img = p.thumbnail(500);
	img.save("C:/temp/thmb.png", "png");
}
