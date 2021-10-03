/*! 
    @file       ItemDelegate.h
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief      This file contains several delegate class inherits QItemDelegate.
*/

#ifndef DELEGATE_H
#define DELEGATE_H

#include <math.h>
#include <QItemDelegate>
#include <QSqlQueryModel>

#include "qtutility_global.h"

namespace MOON
{
	//-----------------------------------------------------------------------------
	//	특정 Validator를 가지는 아이템 Delegate 구현
	//-----------------------------------------------------------------------------
	class QTUTILITY_EXPORT ItemValidationDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:

		// 생성시 사용할 Validator를 설정해 준다.
		ItemValidationDelegate(QValidator* v, QObject* parent = 0);

		// 모델의 에디터 위젯을 생성하여 반환한다.
		QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	private:
		QValidator*	_v;
	};

	//-----------------------------------------------------------------------------
	//	특정 입력 Mask를 가지는 아이템 Delegate 구현
	//-----------------------------------------------------------------------------
	class QTUTILITY_EXPORT ItemMaskDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:

		// 생성시 사용할 Validator를 설정해 준다.
		ItemMaskDelegate(const QString& mask, QObject* parent = 0);

		// 모델의 에디터 위젯을 생성하여 반환한다.
		QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	private:
		QString		_mask;
	};

	//-----------------------------------------------------------------------------
	//	콤보박스(리스트 type)를 가지는 아이템 Delegate 구현
	//-----------------------------------------------------------------------------
	class QTUTILITY_EXPORT ComboBoxItemDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:

		// 생성시 사용할 Validator를 설정해 준다.
		ComboBoxItemDelegate(const QStringList& list = QStringList(), QObject* parent = 0);
		ComboBoxItemDelegate(const QMap<QVariant, QString>& list, QObject* parent = 0);
		ComboBoxItemDelegate(const QStringList& list, const QVariantList& values, QObject* parent = 0);

		// 모델의 에디터 위젯을 생성하여 반환한다.
		QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		void setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const;

	private:
		QMap<QVariant, QString>	_list;
	};

	//---------------------------------------------------------
	//SpinBox Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT SpinBoxDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		SpinBoxDelegate( int min=-HUGE_VAL, int max=HUGE_VAL, QObject* parent = 0) : QItemDelegate(parent), _min(min), _max(max) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;

		void		setMinMax(int min, int max);	// SpinBox의 최소,최대값 설정

	private:
		int			_min;
		int			_max;
	};

	//---------------------------------------------------------
	//DoubleSpinBox Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT DoubleSpinBoxDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		DoubleSpinBoxDelegate(double min=-HUGE_VAL, double max=HUGE_VAL, int decimals=6, QObject* parent = 0) : QItemDelegate(parent), _min(min), _max(max), _decimals(decimals) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;

	public:
		void		setMinMax(double min, double max);	// SpinBox의 최소,최대값 설정

	private:
		double		_min;
		double		_max;
		int			_decimals;
	};


	//---------------------------------------------------------
	//ComboBox Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT ComboBoxDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		ComboBoxDelegate(QObject* parent = 0) : QItemDelegate(parent), _model(NULL) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;

	public:
		void		setModel(QSqlQueryModel* model);	// ComboBox 모델 지정

	private:
		QSqlQueryModel*	_model;
	};


	//---------------------------------------------------------
	//LineEditDelegate Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT LineEditDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		LineEditDelegate(QObject* parent = 0) : QItemDelegate(parent) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;
	};


	//---------------------------------------------------------
	//DateTime Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT DateTimeDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		DateTimeDelegate(QObject* parent = 0, const QString& format = "yyyy/MM/dd hh:mm:ss") : QItemDelegate(parent), _format(format){;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;

	private:
		QString _format;
	};


	//---------------------------------------------------------
	//Date Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT DateDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		DateDelegate(QObject* parent = 0) : QItemDelegate(parent) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;
	};


	//---------------------------------------------------------
	//Time Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT TimeDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		TimeDelegate(QObject* parent = 0) : QItemDelegate(parent) {;}

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;
	};


	//---------------------------------------------------------
	// Bool Delegate
	//---------------------------------------------------------
	class QTUTILITY_EXPORT BoolDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		BoolDelegate( const QString& trueText="True", const QString& falseText="False", QObject* parent = 0);

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;

	private:
		QString _true;
		QString	_false;
	};

	// Uneditable Delegate
	class QTUTILITY_EXPORT NullDelegate : public QItemDelegate
	{
		Q_OBJECT

	public:
		NullDelegate(QObject* parent = 0);

		QWidget*	createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
		void		setEditorData(QWidget* editor, const QModelIndex& index) const;
		void		setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const;
	};
};
#endif // DELEGATE_H
