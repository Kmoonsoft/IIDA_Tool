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
	/// QSplitter �ʱ�ȭ
	///-----------------------------------------------------------------------------

	/// QSplitter Widget�� �й跮�� �ʱ�ȭ �Ѵ�.
	/// �Է� : w�� ��� ���ø���
	static void InitSplitter(QSplitter* w, bool slim = true);// ��� ������ �յ���� �Ѵ�.
	static void InitSplitter(QSplitter* w, int a, int b, bool slim = true);	//  2���� ���� ������ ����
	static void InitSplitter(QSplitter* w, int a, int b, int c, bool slim = true); // 3���� ���� ������ ����

	///-----------------------------------------------------------------------------
	/// QTableView �ʱ�ȭ
	///-----------------------------------------------------------------------------

	/// QTableView Widget�� �ʱ�ȭ �Ѵ�
	/// �Է� : w�� ��� ���̺� ��
	static void	InitTable(QTableView* w);
	static void	InitTable2(QTableView* w);
	static void	InitTable3(QTableView* w);
	static void	InitTable4(QTableView* w);

	// ������Ʈ(parent)�� ���� �ڽ� ������Ʈ�� QLineEdit�� QTextEdit�̸� ������ Ŭ���� ��Ų��.
	//	�Է� : parent �κ� ������Ʈ
	static void ClearEdit(const QObject* parent);

	// QStandardItemModel�� �����ش��� ������ �����Ѵ�.
	static void SetToolTipsOnHeader(QStandardItemModel* model, QStringList& tooltips);

	static QString Korean(const char* text);
	static QString Korean(const QString& text);
};
