/*! 
    @file       SqlTableView.cpp
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief      The SqlTableView class is an extension class of QSqlTableView.
*/

#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>
#include <QProcess>
#include <QFileInfo>
#include <QSettings>

#include "SqlQuery.h"
#include "UiControl.h"
#include "SqlTableView.h"

using namespace MOON;

SqlTableView::SqlTableView(QWidget *parent)
	: QTableView(parent)
{
	UiControl::InitTable(this);

	setStyleSheet( "QTableView {selection-background-color:#0099FF;}" );

	QTableView::setSortingEnabled(true);
	
	_excelAction = addAction( tr("Export to Excel"), this, SLOT(exportToExcel()), false, QIcon(QString::fromUtf8(":/Qx/Resources/excel.png")) );
	_clipAction = addAction( tr("Copy to Clipboard"), this, SLOT(copyToClip()));
	_selRelatedActions << _clipAction;

	setContextMenuPolicy( Qt::ActionsContextMenu );
	QTableView::addAction(_excelAction);
	QTableView::addAction(_clipAction);

	_excelAction->setEnabled(false);
	_clipAction->setEnabled(false);

	_currentRow = -1;
}

SqlTableView::~SqlTableView()
{

}

void SqlTableView::setHorizontalHeaderLabels(const QStringList& labels)
{
	if( model() == 0 ) return;

	int i = 0;
	foreach( const QString& l, labels)
	{
		model()->setHeaderData(i++, Qt::Horizontal, l, Qt::DisplayRole );
	}
}

QModelIndex SqlTableView::selectedIndex()
{
	QModelIndexList list = QTableView::selectedIndexes();
	if( list.isEmpty() ) return QModelIndex();
	return list[0];
}

QModelIndexList SqlTableView::selectedIndexes()
{
	return QTableView::selectedIndexes();
}

QModelIndexList SqlTableView::selectedRows(int column)
{
	QModelIndexList l;
	QModelIndexList list = QTableView::selectedIndexes();
	foreach( const QModelIndex& idx, list )
	{
		if( idx.column() == column ) l << idx;
	}

	return l;
}

bool SqlTableView::beSelected()
{
	QModelIndexList list = QTableView::selectedIndexes();
	return !( list.isEmpty() || !list[0].isValid() );
}

void SqlTableView::itemDoubleClicked(const QModelIndex& index)
{
	emit doubleClicked(index);
}

void SqlTableView::addAction( QAction* action, bool relatedSelection )
{
	if( _excelAction == 0 ) QTableView::addAction(action);
	else insertAction( _excelAction, action );

	if( relatedSelection ) 
	{
		_selRelatedActions << action;
		action->setEnabled(false);
	}
}

void SqlTableView::addAction( QAction* action, const QObject *receiver, const char* member, bool relatedSelection )
{
	if( _excelAction == 0 ) QTableView::addAction(action);
	else insertAction( _excelAction, action );

	connect( action, SIGNAL(triggered()), receiver, member );
	if( relatedSelection ) 
	{
		_selRelatedActions << action;
		action->setEnabled(false);
	}
}

QAction* SqlTableView::addAction( const QString &text, const QObject *receiver, const char* member, bool relatedSelection, const QIcon &icon )
{
	QAction* action;
	if( icon.isNull() ) action = new QAction( text, this );
	else action = new QAction( icon, text, this );

	addAction(action, receiver, member, relatedSelection );
	return action;
}

QAction* SqlTableView::addSeparator()
{
	QAction* action = new QAction(this);
	action->setSeparator(true);
	if( _excelAction == 0 ) QTableView::addAction(action);
	else insertAction( _excelAction, action );
	return action;
}

void SqlTableView::deleteSelected()
{
	QModelIndexList idxList = QTableView::selectedIndexes();

	QList<int> rows;
	foreach( const QModelIndex& idx, idxList )
	{
		int row = idx.row();
		if( rows.contains(row) ) continue;
		rows << row;
	}

	qSort(rows.begin(), rows.end(), qGreater<int>());
	QAbstractItemModel* m = model();
	foreach( int i, rows ) m->removeRow( i );

	_currentRow = -1;
}

void SqlTableView::currentChanged ( const QModelIndex & current, const QModelIndex & previous )
{
	emit currentItemChanged(current);
	if( current.row() != previous.row() ) emit currentRowChanged(current);
	_currentRow = current.row();

	QTableView::currentChanged(current, previous);
}

void SqlTableView::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
	bool sel = !selected.isEmpty();
	foreach( QAction* a, _selRelatedActions ) a->setEnabled(sel);

	emit selectionChanged(sel);
	QTableView::selectionChanged(selected, deselected);
}

void SqlTableView::mousePressEvent( QMouseEvent * event )
{
	_excelAction->setDisabled( model()==0 || model()->rowCount() == 0 ); // be careful 
	QTableView::mousePressEvent(event);
}

void SqlTableView::copyToClip()
{
	QModelIndexList idxList = QTableView::selectedIndexes();
	
	QString text;
	QMap<int, QStringList> texts;
	foreach( const QModelIndex& i, idxList ) 
	{
		QStringList l = texts.value(i.row(), QStringList());
		l << i.data().toString();

		texts.insert(i.row(), l);
	}
	
	foreach( int key, texts.keys() )
	{
		text += texts.value(key).join(", ") + "\n";
	}

	if( !text.isEmpty() ) text.chop(1);
	QApplication::clipboard()->setText(text);
}

void SqlTableView::exportToExcel()
{
	QAbstractItemModel* m = model();
	if( m == 0 || m->rowCount() == 0 || m->columnCount() == 0 )
	{
		MSG_WARNING( tr("No data") );
		return;
	}	
	
	QSettings s( qApp->applicationDirPath()+"/ExcelSavePath", QSettings::IniFormat, this);
	QString refPath = s.value("FilePath", qApp->applicationDirPath()).toString();

	QString filePath = QFileDialog::getSaveFileName(this, "Export to a [MS Excel] file", refPath, "Excel File(*.xlsb *.xls)" );
	if( filePath.isEmpty() ) return;
	s.setValue("FilePath", filePath);

	QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", QString("Excel%1").arg((long long)this) );
	QString dbName = QString("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%1\";FIL={MS Excel};DBQ=").arg(filePath) + filePath;
	db.setDatabaseName(dbName);

	if( !db.open() )
	{
		MSG_CRITICAL( tr("Fail to create an excel file.\n\n") + db.lastError().text() );
		return;
	}

	QModelIndex parentIndex;
	while(m->canFetchMore(parentIndex))
	{
		m->fetchMore(parentIndex);
	}

	SqlQuery q(db);
	
	int rowCnt = m->rowCount();
	int colCnt = m->columnCount();
	QStringList heads;
	QStringList orgHeads;
	QStringList ps;
	QList<bool> hiddenList;
	for( int c = 0; c < colCnt; c++ )
	{
		bool hide =  isColumnHidden(c);
		hiddenList << hide;
		if( hide ) continue;
		QString head = m->headerData(c, Qt::Horizontal).toString();
		
		if( head.isEmpty() ) head = QString::number(c);
		int k=0;
		QString key = head;
		while( orgHeads.contains(key) )
		{
			k++;
			key = head + QString::number(k);
		}
		orgHeads << key;
		heads << "\"" + key + "\"";
		ps << "?";
	}

	QString qryStr = "create table Sheet ( " + heads.join(" Text,") + " Text)";
	q.exec(qryStr);

	qryStr = "insert into Sheet values(" + ps.join(",") + ")";
	for( int r = 0; r < rowCnt; r++ )
	{
		QVariantList values;
		for( int c = 0; c < colCnt; c++ )
		{
			if( hiddenList[c] ) continue;
			values << m->data(m->index(r, c)).toString();
		}
		q.exec(qryStr, values);

		if( r%10 == 0 ) qApp->processEvents();
	}
	db.close();
	//MSG_INFORMATION( tr("Excel file was saved.\n") + filePath );
	if( 1 == QMessageBox::information(this, tr("Save"), tr("Excel file was saved.\n") + filePath, tr("Ok"), tr("Open Folder") ) )
	{
		QProcess* p = new QProcess(this);
		QFileInfo fi(filePath);
		p->start("explorer.exe /select," + filePath.replace("/", "\\"));
	}
}
