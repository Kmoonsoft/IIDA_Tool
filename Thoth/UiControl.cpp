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

// QSplitter Widget의 균등분할 한다.
// 입력 : w 대상 스플리터
void
UiControl::InitSplitter( QSplitter* w, bool slim )
{
	QList<int> l;
	for( int i = 0; i < w->count(); i++ )
	{
		l << 1;// 균등 비율
	}
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QSplitter Widget의 2개의 분할 비율을 지정한다.
// 입력 : w 대상 스플리터, a와 b는 분할 비율
void
UiControl::InitSplitter( QSplitter* w, int a, int b, bool slim )
{
	QList<int> l;
	// 비율 설정
	l << a*10000 << b*10000;
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QSplitter Widget의 3개의 분할 비율을 지정한다.
// 입력 : w는 대상 스플리터,  a, b, c값은 분할 비율이다.
void
UiControl::InitSplitter( QSplitter* w, int a, int b, int c, bool slim )
{
	QList<int> l;
	// 비율 설정
	l << a*10000 << b*10000 << c*10000;
	w->setSizes(l);
	if( slim ) slimSplitter(w);
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);									// 수직 해더 이동 금지
	//v->setHidden(true);										// 수직 해더 숨김
	v->setDefaultSectionSize( 20 );							// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );				// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);									// 수평 해더 이동 금지
	r->setSortIndicatorShown(true);							// 소팅 인디케이더 표시

	w->setAlternatingRowColors(true);						// 한줄씩 회색 채움색 지정
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//에디터 금지

	// 섹션 누름시 소팅 기능 설정
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	//w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable2( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setHidden(true);							// 수직 해더 숨김
	v->setDefaultSectionSize( 18 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(true);				// 소팅 인디케이더 표시

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정

	// 섹션 누름시 소팅 기능 설정
	w->disconnect( r, SIGNAL(sectionPressed(int)), w, SLOT(selectColumn(int)) );
	//w->connect( r, SIGNAL(sectionClicked(int)), w, SLOT(sortByColumn(int)) );
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable3( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setHidden(true);							// 수직 해더 숨김
	v->setDefaultSectionSize( 18 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(false);			// 소팅 인디케이더 숨기기

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정
}

// QTableView Widget을 초기화한다. 정해진 규격으로 초기화
// 입력 : w는 대상 테이블 뷰
void
UiControl::InitTable4( QTableView* w )
{
	// 수직 해더 설정
	QHeaderView* v = w->verticalHeader();
	v->setSectionsMovable(false);						// 수직 해더 이동 금지
	v->setDefaultSectionSize( 20 );				// 수직해더 높이는 18pt
	v->setDefaultAlignment( Qt::AlignVCenter );	// 텍스트 중앙정렬
	v->sectionResizeMode(QHeaderView::Fixed);		// 수직 해더 사이즈 고정

	// 수평 해더 설정
	QHeaderView* r = w->horizontalHeader();
	r->setSectionsMovable(false);						// 수평 해더 이동 금지
	r->setSortIndicatorShown(false);			// 소팅 인디케이더 숨기기

	w->setAlternatingRowColors(true);			// 한줄씩 회색 채움색 지정
	w->setEditTriggers( QAbstractItemView::NoEditTriggers );//에디터 금지
}

QString UiControl::Korean(const char * text)
{
	return QTextCodec::codecForName("EUC-KR")->toUnicode(text);
}

QString UiControl::Korean(const QString & text)
{
	return QTextCodec::codecForName("EUC-KR")->toUnicode(text.toLatin1());
}

// 오브젝트(parent)가 가진 자식 오브젝트가 QLineEdit나 QTextEdit이면 내용을 클리어 시킨다.
//	입력 : parent 부보 오브젝트
void UiControl::ClearEdit(const QObject* parent)
{
	if( !parent ) return;
	
	const QObjectList& list = parent->children();
	for(QObject* o : list) 
	{
		//라인 에디트 클리어
		QLineEdit* e = dynamic_cast<QLineEdit*>(o);
		if( e ) {
			e->clear();
			continue;
		}

		// TextEdit 클리어
		QTextEdit* t  = dynamic_cast<QTextEdit*>(o);
		if( t ) {
			t->clear();
			continue;
		}
		
		if( dynamic_cast<QLabel*>(o)) continue; // 레이블 건너띄기
		if( dynamic_cast<QComboBox*>(o) ) continue;// 콤보박스 건너띄기

		ClearEdit(o);
	}
}

// QStandardItemModel의 수평해더에 툴팁을 설정한다.
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
