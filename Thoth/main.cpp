/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky			First release(v0.1)
*/

#ifdef WIN32
#include <Windows.h>
#endif

#include <QtCore>
#include <QtWidgets>

#include "gdal.h"
#include "LabelController.h"
#include "MainWidget.h"
#include "Settings.h"

#include "UnitTest.h"

static QString g_path; ///< 출력을 기록할 경로.
static QString g_appName;

/// 전체 메시지를 파일로 출력
void QtMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QString log = QString("%1\t%2\n").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss")).arg(msg);

	// 디버깅을 목적으로 콘솔로 출력되는 내용을 파일에도 기록한다.
	QFile file(g_path + g_appName + QDate::currentDate().toString("_yyyyMMdd.log"));
	file.open(QIODevice::Append | QIODevice::Text);
	file.write(log.toLatin1());
	file.close();

#ifdef WIN32
	//printf(qPrintable(log)); // Sentinel 검색시 이부분에서 비정상 종료 케이스 있음.
	OutputDebugString((LPCWSTR)log.utf16());// write to console
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QObject parent;
	g_appName = "Thot";

	QString iniFile = qApp->applicationDirPath() + QString("/%1.ini").arg(g_appName);
	Settings::initialize(&parent, iniFile);
	LabelController::create(&parent);

	int rst = -1;
	// Using GDAL
	GDALAllRegister();
	{
		MainWidget w;
		w.show();
		rst = a.exec();
	}
	// Destroy GDAL
	GDALDestroyDriverManager();
	return rst;
}
