/*! 
    @file       UiControl.h
    @date       2021.09.01
    @author     Lucky
    @version    v1.0
    @brief      The UiControl class provides several convenient methods to controll Qt UI classes.
*/

/*
@since
Date		Editor		Comments
-------------------------------------------------------------------------------
20211003	Lucky		First release(v1.0)
*/

#include <QComboBox>
#include <QSplitter>
#include <QHeaderView>
#include <QTableView>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QStandardItem>
#include <QTextCodec>
#include <math.h>
#include "UiControl.h"

void slimSplitter( QSplitter* w)
{
	w->setHandleWidth(1);
	w->setOpaqueResize(false);
}

// QSplitter Widget�� �յ���� �Ѵ�.
// �Է� : w ��� ���ø���
void
UiControl::InitSplitter( QSplitter* w, bool slim )
{
	QList<int> l;
	for( int i = 0; i < w->count(); i++ )
	{
		l << 1;// �յ� ����
	}
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QSplitter Widget�� 2���� ���� ������ �����Ѵ�.
// �Է� : w ��� ���ø���, a�� b�� ���� ����
void
UiControl::InitSplitter( QSplitter* w, int a, int b, bool slim )
{
	QList<int> l;
	// ���� ����
	l << a*10000 << b*10000;
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QSplitter Widget�� 3���� ���� ������ �����Ѵ�.
// �Է� : w�� ��� ���ø���,  a, b, c���� ���� �����̴�.
void
UiControl::InitSplitter( QSplitter* w, int a, int b, int c, bool slim )
{
	QList<int> l;
	// ���� ����
	l << a*10000 << b*10000 << c*10000;
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);									// ���� �ش� �̵� ����
	//v->setHidden(true);										// ���� �ش� ����
	v->setDefaultSectionSize( 20 );							// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );				// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);									// ���� �ش� �̵� ����
	r->setSortIndicatorShown(true);							// ���� �ε����̴� ǥ��

	w->setAlternatingRowColors(true);						// ���پ� ȸ�� ä��� ����
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//������ ����

	// ���� ������ ���� ��� ����
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	//w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable2( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setHidden(true);							// ���� �ش� ����
	v->setDefaultSectionSize( 18 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(true);				// ���� �ε����̴� ǥ��

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����

	// ���� ������ ���� ��� ����
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	//w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable3( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setHidden(true);							// ���� �ش� ����
	v->setDefaultSectionSize( 18 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(false);			// ���� �ε����̴� �����

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����
}

// QTableView Widget�� �ʱ�ȭ�Ѵ�. ������ �԰����� �ʱ�ȭ
// �Է� : w�� ��� ���̺� ��
void
UiControl::InitTable4( QTableView* w )
{
	// ���� �ش� ����
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// ���� �ش� �̵� ����
	v->setDefaultSectionSize( 20 );				// �����ش� ���̴� 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// �ؽ�Ʈ �߾�����
	v->sectionResizeMode(QHeaderView::Fixed);		// ���� �ش� ������ ����

	// ���� �ش� ����
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// ���� �ش� �̵� ����
	r->setSortIndicatorShown(false);			// ���� �ε����̴� �����

	w->setAlternatingRowColors(true);			// ���پ� ȸ�� ä��� ����
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//������ ����
}

QString UiControl::Korean(const char * text)
{
	return QTextCodec::codecForName("EUC-KR")->toUnicode(text);
}

QString UiControl::Korean(const QString & text)
{
	return QTextCodec::codecForName("EUC-KR")->toUnicode(text.toLatin1());
}

// ������Ʈ(parent)�� ���� �ڽ� ������Ʈ�� QLineEdit�� QTextEdit�̸� ������ Ŭ���� ��Ų��.
//	�Է� : parent �κ� ������Ʈ
void UiControl::ClearEdit(const QObject* parent)
{
	if( !parent ) return;
	
	const QObjectList& list = parent->children();
	for(QObject* o : list) 
	{
		//���� ����Ʈ Ŭ����
		QLineEdit* e = dynamic_cast<QLineEdit*>(o);
		if( e ) {
			e->clear();
			continue;
		}

		// TextEdit Ŭ����
		QTextEdit* t  = dynamic_cast<QTextEdit*>(o);
		if( t ) {
			t->clear();
			continue;
		}
		
		if( dynamic_cast<QLabel*>(o)) continue; // ���̺� �ǳʶ��
		if( dynamic_cast<QComboBox*>(o) ) continue;// �޺��ڽ� �ǳʶ��

		ClearEdit(o);
	}
}

// QStandardItemModel�� �����ش��� ������ �����Ѵ�.
void UiControl::SetToolTipsOnHeader(QStandardItemModel* model, QStringList& tooltips)
{
	int i = 0;
	for( const QString& tooltip : tooltips )
	{
		QStandardItem* item = model->horizontalHeaderItem(i);
		if( item == 0 ) return;
		item->setToolTip(tooltip);
	}
}
