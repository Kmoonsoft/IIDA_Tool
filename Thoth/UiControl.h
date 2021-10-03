/*! 
    @file		UiControl.h
    @date		2021.09.01
    @author		Lucky
	@version	v1.0
    @warning	Nobody can edit this file except the author
    @brief		The UiControl class provides several convenient methods for controlling Qt UI classes.
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v1.0)
*/
#pragma once

#include <QMessageBox>

class QObject;
class QSplitter;
class QTableView;
class QStandardItemModel;

#define	MSG_INFORMATION(msg)				QMessageBox::information(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_INFORMATION_T(title, msg)		QMessageBox::information(this, title, msg, tr("&Ok"))
#define	MSG_WARNING(msg)					QMessageBox::warning(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_WARNING_T(title, msg)			QMessageBox::warning(this, title, msg, tr("&Ok"))
#define	MSG_CRITICAL(msg)					QMessageBox::critical(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_CRITICAL_T(title, msg)			QMessageBox::critical(this, title, msg, tr("&Ok"))
#define	MSG_QUESTION(msg)					QMessageBox::question(this, windowTitle(), msg)
#define	MSG_QUESTION_T(title, msg)			QMessageBox::question(this, title, msg, tr("&Yes"), tr("&No"))

class UiControl
{
public:

	///-----------------------------------------------------------------------------
	/// QSplitter 초기화
	///-----------------------------------------------------------------------------

	/// QSplitter Widget의 분배량을 초기화 한다.
	/// 입력 : w는 대상 스플리터
	static void InitSplitter(QSplitter* w, bool slim = true);// 모든 위젯을 균등분할 한다.
	static void InitSplitter(QSplitter* w, int a, int b, bool slim = true);	//  2개의 분할 비율을 지정
	static void InitSplitter(QSplitter* w, int a, int b, int c, bool slim = true); // 3개의 분할 비율을 지정

	///-----------------------------------------------------------------------------
	/// QTableView 초기화
	///-----------------------------------------------------------------------------

	/// QTableView Widget을 초기화 한다
	/// 입력 : w는 대상 테이블 뷰
	static void	InitTable(QTableView* w);
	static void	InitTable2(QTableView* w);
	static void	InitTable3(QTableView* w);
	static void	InitTable4(QTableView* w);

	// 오브젝트(parent)가 가진 자식 오브젝트가 QLineEdit나 QTextEdit이면 내용을 클리어 시킨다.
	//	입력 : parent 부보 오브젝트
	static void ClearEdit(const QObject* parent);

	// QStandardItemModel의 수평해더에 툴팁을 설정한다.
	static void SetToolTipsOnHeader(QStandardItemModel* model, QStringList& tooltips);

	static QString Korean(const char* text);
	static QString Korean(const QString& text);
};
