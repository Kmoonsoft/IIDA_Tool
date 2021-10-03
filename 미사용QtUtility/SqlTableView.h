/*! 
    @file       SqlTableView.h
    @date       2011.11.25
    @author     Lucky Kim(eeff@gmail.com)
    @version    v0.1
    @brief      The SqlTableView class is an extension class of QSqlTableView.
*/

#ifndef SQLTABLEVIEW_H
#define SQLTABLEVIEW_H

#include <QTableView>

#include "qtutility_global.h"

class QMenu;

namespace MOON
{
	class QTUTILITY_EXPORT SqlTableView : public QTableView
	{
		Q_OBJECT

	public:
		SqlTableView(QWidget *parent = 0);
		~SqlTableView();
		void setHorizontalHeaderLabels(const QStringList& labels);

		QAction*	exportToExcelAction(){ return _excelAction; };
		QAction*	copyToClipAction(){ return _clipAction; };
		QModelIndex selectedIndex();
		QModelIndexList selectedIndexes();
		QModelIndexList selectedRows(int column=0);
		bool		beSelected();
		void		itemDoubleClicked(const QModelIndex& index);
		int			currentRow() const { return _currentRow; };

		void		addAction( QAction* action, bool relatedSelection = false );
		void		addAction( QAction* action, const QObject *receiver, const char* member, bool relatedSelection = false );
		QAction*	addAction( const QString &text, const QObject *receiver, const char* member, bool relatedSelection = false, const QIcon &icon=QIcon() );
		QAction*	addSeparator();

	public slots:
		void		deleteSelected();

	signals:
		void	currentItemChanged(const QModelIndex &);
		void	currentRowChanged(const QModelIndex &);
		void	selectionChanged(bool);

	protected :
		void	currentChanged ( const QModelIndex & current, const QModelIndex & previous );
		void	selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected );
		void	mousePressEvent( QMouseEvent * event );

	private slots:
		void	copyToClip();
		void	exportToExcel();

	private:
		QAction*	_excelAction;
		QAction*	_clipAction;
		QList<QAction*> _selRelatedActions;
		int			_currentRow;

	};
};
#endif // SQLTABLEVIEW_H
