/*! 
    @file		UiControl.h
    @date		2018.03.01
    @author		Lucky Kim(eeff@gmail.com)
    @version	v1.1
    @warning	Nobody can edit this file except the author
    @brief		The UiControl class QTUTILITY_EXPORT provides several convenient methods for controlling Qt UI classes.
*/

#ifndef UICONTROL_H
#define UICONTROL_H

#include <QMessageBox>

#include "qtutility_global.h"

class QObject;
class QSplitter;
class QTableView;
class QStandardItemModel;

namespace MOON
{
#define	MSG_INFORMATION(msg)				QMessageBox::information(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_INFORMATION_T(title, msg)		QMessageBox::information(this, title, msg, tr("&Ok"))
#define	MSG_WARNING(msg)					QMessageBox::warning(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_WARNING_T(title, msg)			QMessageBox::warning(this, title, msg, tr("&Ok"))
#define	MSG_CRITICAL(msg)					QMessageBox::critical(this, windowTitle(), msg, tr("&Ok"))
#define	MSG_CRITICAL_T(title, msg)			QMessageBox::critical(this, title, msg, tr("&Ok"))
#define	MSG_QUESTION(msg)					QMessageBox::question(this, windowTitle(), msg)
#define	MSG_QUESTION_T(title, msg)			QMessageBox::question(this, title, msg, tr("&Yes"), tr("&No"))

#define	ENUM_TO_STRINGLIST(ClassName, EnumName, stringList) \
        { \
            const QMetaObject& mo = ClassName::staticMetaObject; \
            int index = mo.indexOfEnumerator(#EnumName); \
            QMetaEnum metaEnum = mo.enumerator(index); \
			int max = metaEnum.value(metaEnum.keyCount() - 1);\
			for (int i = 0; i<max + 1; ++i)\
                stringList << QString(metaEnum.valueToKey(i)); \
        }\


	class QTUTILITY_EXPORT UiControl
	{
	public:

		///-----------------------------------------------------------------------------
		/// QSplitter �ʱ�ȭ
		///-----------------------------------------------------------------------------

		/// QSplitter Widget�� �й跮�� �ʱ�ȭ �Ѵ�.
		/// �Է� : w�� ��� ���ø���
		static void InitSplitter( QSplitter* w, bool slim=true );// ��� ������ �յ���� �Ѵ�.
		static void InitSplitter( QSplitter* w, int a, int b, bool slim=true );	//  2���� ���� ������ ����
		static void InitSplitter( QSplitter* w, int a, int b, int c, bool slim=true ); // 3���� ���� ������ ����

		///-----------------------------------------------------------------------------
		/// QTableView �ʱ�ȭ
		///-----------------------------------------------------------------------------

		/// QTableView Widget�� �ʱ�ȭ �Ѵ�
		/// �Է� : w�� ��� ���̺� ��
		static void	InitTable( QTableView* w );
		static void	InitTable2( QTableView* w );
		static void	InitTable3( QTableView* w );
		static void	InitTable4( QTableView* w );
		
		// ������Ʈ(parent)�� ���� �ڽ� ������Ʈ�� QLineEdit�� QTextEdit�̸� ������ Ŭ���� ��Ų��.
		//	�Է� : parent �κ� ������Ʈ
		static void ClearEdit(const QObject* parent);

		// QStandardItemModel�� �����ش��� ������ �����Ѵ�.
		static void SetToolTipsOnHeader(QStandardItemModel* model, QStringList& tooltips);

		static QString Korean(const char* text);
		static QString Korean(const QString& text);
	};
};
#endif // UICONTROL_H
