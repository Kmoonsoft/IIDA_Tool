/*! 
    @file       ItemDelegate.cpp
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief		This file contains several delegate class inherits QItemDelegate.
*/

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTimeEdit>
#include <QDoubleSpinBox>

#include "ItemDelegate.h"

using namespace MOON;

//-----------------------------------------------------------------------------
//	특정 Validator를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
ItemValidationDelegate::ItemValidationDelegate(QValidator* v, QObject* parent ) 
: QItemDelegate( parent ), _v(v)
{
	// do nothing
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ItemValidationDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index );

	QLineEdit* e = new QLineEdit( parent );
	e->setFrame(false);
	e->installEventFilter(const_cast<ItemValidationDelegate*>(this));
	
	e->setValidator( _v );
	return e;
}

//-----------------------------------------------------------------------------
//	특정 입력 마스크를 가지는 아이템 Delegate 구현
//-----------------------------------------------------------------------------
ItemMaskDelegate::ItemMaskDelegate(const QString& mask, QObject* parent ) 
: QItemDelegate( parent ), _mask(mask)
{
	// do nothing
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ItemMaskDelegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
	QVariant v = index.model()->data( index );

	QLineEdit* e = new QLineEdit( parent );
	e->setFrame(false);
	e->installEventFilter(const_cast<ItemMaskDelegate*>(this));
	
	e->setInputMask(_mask);
	return e;
}

//-----------------------------------------------------------------------------
//	콤보박스 아이템
//-----------------------------------------------------------------------------
ComboBoxItemDelegate::ComboBoxItemDelegate(const QStringList& list, QObject* parent ) 
: QItemDelegate( parent )
{
	for (int i = 0; i < list.count(); i++)
	{
		_list[i] = list[i];
	}
}

MOON::ComboBoxItemDelegate::ComboBoxItemDelegate(const QMap<QVariant, QString>& list, QObject * parent)
	: QItemDelegate(parent), _list(list)
{
}

MOON::ComboBoxItemDelegate::ComboBoxItemDelegate(const QStringList & list, const QVariantList & values, QObject * parent)
	: QItemDelegate(parent)
{
	int cnt = qMin(values.count(), list.count());
	for (int i = 0; i < cnt; i++)
	{
		_list[values[i]] = list[i];
	}
}

// 모델의 에디터 위젯을 생성하여 반환한다.
QWidget*
ComboBoxItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QVariant v = index.model()->data(index, Qt::UserRole);

	QComboBox* e = new QComboBox(parent);
	for( const QVariant& key : _list.keys())
	{
		e->addItem(_list[key], key);
	}

	e->installEventFilter(const_cast<ComboBoxItemDelegate*>(this));

	e->setCurrentIndex(v.toInt());
	return e;
}

void ComboBoxItemDelegate::setModelData ( QWidget * w, QAbstractItemModel * model, const QModelIndex & index ) const
{
	QComboBox* cbox = dynamic_cast<QComboBox*>(w);
	if( !w ) return;

	model->setData(index, cbox->currentText());
	model->setData(index, cbox->currentIndex(), Qt::UserRole);
}

//------------------------------------------------
// SpinBox Delegate
//------------------------------------------------
QWidget* SpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QSpinBox* editor = new QSpinBox(parent);
    editor->setMinimum(_min);
    editor->setMaximum(_max);
    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    int value = index.model()->data(index, Qt::EditRole).toInt();
    spinbox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    spinbox->interpretText();
    int value = spinbox->value();

    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::setMinMax(int min, int max)
{
	_min = min;
	_max = max;
}


//------------------------------------------------
// DoubleSpinBox Delegate
//------------------------------------------------
QWidget* DoubleSpinBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
    editor->setMinimum(_min);
    editor->setMaximum(_max);
	editor->setDecimals(_decimals);
    return editor;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QDoubleSpinBox *spinbox = static_cast<QDoubleSpinBox*>(editor);
    double value = index.model()->data(index, Qt::EditRole).toDouble();
    spinbox->setValue(value);
}

void DoubleSpinBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QDoubleSpinBox* spinbox = static_cast<QDoubleSpinBox*>(editor);
    spinbox->interpretText();
    double value = spinbox->value();

    model->setData(index, value, Qt::EditRole);
}

void DoubleSpinBoxDelegate::setMinMax(double min, double max)
{
	_min = min;
	_max = max;
}

//------------------------------------------------
// ComboBox Delegate
//------------------------------------------------
QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QComboBox* editor = new QComboBox(parent);

	if (_model != NULL) {
		editor->setModel(_model);
	}

    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox *combobox = static_cast<QComboBox*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();
    combobox->setCurrentIndex(combobox->findText(value));
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    QString value = combobox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::setModel(QSqlQueryModel* model)
{
	_model = model;
}

//------------------------------------------------
// LineEdit Delegate
//------------------------------------------------
QWidget* LineEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QLineEdit* editor = new QLineEdit(parent);
	editor->setText("");
    return editor;
}

void LineEditDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    QString value = index.model()->data(index, Qt::EditRole).toString();
	lineEdit->setText(value);
}

void LineEditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------
// DateTime Delegate
//------------------------------------------------
QWidget* DateTimeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QDateTimeEdit* editor = new QDateTimeEdit(parent);
	editor->setDateTime(QDateTime::currentDateTime());
    return editor;
}

void DateTimeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QDateTimeEdit *dateTimeEdit = static_cast<QDateTimeEdit*>(editor);
	dateTimeEdit->setDisplayFormat(_format);
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QDateTime value = QDateTime::fromString(text, _format);
	dateTimeEdit->setDateTime(value);
}

void DateTimeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QDateTimeEdit* dateTimeEdit = static_cast<QDateTimeEdit*>(editor);
    QString value = dateTimeEdit->dateTime().toString(_format);
    model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------
// Date Delegate
//------------------------------------------------
QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QDateEdit* editor = new QDateEdit(parent);
	editor->setDate(QDate::currentDate());
    return editor;
}

void DateDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QDateEdit *dateEdit = static_cast<QDateEdit*>(editor);
    QDate value = index.model()->data(index, Qt::EditRole).toDate();
	dateEdit->setDate(value);
}

void DateDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QDateEdit* dateEdit = static_cast<QDateEdit*>(editor);
    QDate value = dateEdit->date();
    model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------
// Time Delegate
//------------------------------------------------
QWidget* TimeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QTimeEdit* editor = new QTimeEdit(parent);
	editor->setTime(QTime::currentTime());
    return editor;
}

void TimeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QTimeEdit *timeEdit = static_cast<QTimeEdit*>(editor);
    QTime value = index.model()->data(index, Qt::EditRole).toTime();
	timeEdit->setTime(value);
}

void TimeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QTimeEdit* timeEdit = static_cast<QTimeEdit*>(editor);
    QTime value = timeEdit->time();
    model->setData(index, value, Qt::EditRole);
}

//------------------------------------------------
// Bool Delegate
//------------------------------------------------
BoolDelegate::BoolDelegate( const QString& trueText, const QString& falseText, QObject* parent)
	:QItemDelegate(parent)
{
	_true = trueText;
	_false = falseText;
}

QWidget* BoolDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
    QComboBox* editor = new QComboBox(parent);
	editor->addItem(_true);
	editor->addItem(_false);
    return editor;
}

void BoolDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox *combobox = static_cast<QComboBox*>(editor);
    bool value = index.model()->data(index, Qt::EditRole).toBool();
	combobox->setCurrentIndex(value == true ? 0 : 1);
}

void BoolDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    QString value = combobox->currentText();
    model->setData(index, value, Qt::EditRole);
}


NullDelegate::NullDelegate(QObject* parent)
	:QItemDelegate(parent)
{
}

QWidget* NullDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const
{
	QLabel* editor = new QLabel(parent);
	return editor;
}

void NullDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QLabel *lbl = static_cast<QLabel*>(editor);
	QString text  = index.model()->data(index, Qt::EditRole).toString();
	lbl->setText(text);
}

void NullDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLabel *lbl = static_cast<QLabel*>(editor);
	QString value = lbl->text();
	model->setData(index, value, Qt::EditRole);
}
