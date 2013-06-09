#include "tasktreeview.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QFont>

#include "../models/taskmodel.h"

#include <QDebug>

/*
 *Edit: Alternatively, you could key in on either the delegate's closeEditor or commitData signals to intercept the editor's value and apply it to every selected item. You would have to subclass QTableView to accomplish this, so here's a little sample code to get you started inspired from here:
*/

TaskTreeView::TaskTreeView(QWidget *parent)
    : QTreeView(parent)
{
	setFocusPolicy(Qt::StrongFocus);
	setTabKeyNavigation(true);
	setEditTriggers(QAbstractItemView::AnyKeyPressed
	               |QAbstractItemView::DoubleClicked
	               |QAbstractItemView::EditKeyPressed);
	setAllColumnsShowFocus(true);
	setHeaderHidden(false);

	setSelectionMode(QAbstractItemView::ExtendedSelection);

	connect(this, &TaskTreeView::collapsed,
	        this, &TaskTreeView::taskCollapsed);
	connect(this, &TaskTreeView::expanded,
	        this, &TaskTreeView::taskExpanded);
}

void TaskTreeView::keyPressEvent(QKeyEvent *event)
{
	switch ( event->key() )
	{
		case Qt::Key_Return:
		case Qt::Key_Enter:
			if ( state() == QAbstractItemView::EditingState )
				break;
			if ( event->modifiers() & Qt::ShiftModifier )
				addSubtask();
			else if ( event->modifiers() & Qt::ControlModifier )
				toggleTaskDone();
			else
				addTask();
			break;

		case Qt::Key_Up:
			if ( event->modifiers() & Qt::ControlModifier )
				taskMoveUp();
			else
			{
				QTreeView::keyPressEvent(event);
				return;
			}
			break;

		case Qt::Key_Down:
			if ( event->modifiers() & Qt::ControlModifier )
				taskMoveDown();
			else
			{
				QTreeView::keyPressEvent(event);
				return;
			}
			break;

		case Qt::Key_Tab:
			changeCurrentToSubtask();
			break;

		case Qt::Key_Backtab:
			changeCurrentToTask();
			break;

		case Qt::Key_Delete:
			deleteTask();
			break;

		default:
			QTreeView::keyPressEvent(event);
			return;
	}

	event->accept();
}

void TaskTreeView::paintEvent(QPaintEvent *event)
{
	QPainter painter(viewport());
	QPalette palette;
	QFont font;

	if ( model()->rowCount() == 0 )
	{
		font.setItalic(true);
		font.setPointSize(26);
		painter.setPen(palette.color(QPalette::Disabled, QPalette::Text));
		painter.setFont(font);
		painter.drawText(rect(), Qt::AlignCenter, "click here & press ENTER...");
	}
	else
		QTreeView::paintEvent(event);
}

void TaskTreeView::taskCollapsed(const QModelIndex &index)
{
	model()->setData(index, false, TaskModel::TaskExpandedRole);
}

void TaskTreeView::taskExpanded(const QModelIndex &index)
{
	model()->setData(index, true, TaskModel::TaskExpandedRole);
}

void TaskTreeView::expandTasks()
{
	for(int i = 0;i < model()->rowCount(); i++)
		expandTask(model()->index(i, 0));
}

void TaskTreeView::expandTask(const QModelIndex &index)
{
	setExpanded(index, model()->data(index, TaskModel::TaskExpandedRole).toBool());

	for(int i = 0; i < model()->rowCount(index); i++)
		expandTask(index.child(i, 0));
}

int TaskTreeView::unSelectedRowBefore(const QModelIndex &index)
{
	if ( !index.isValid() )
		return -1;

	if ( index.row() <= 0 )
		return -1;

	int row = index.row() - 1;

	while ( selectionModel()->isSelected(model()->index(row, index.column(), index.parent())) )
	{
		row--;
		if ( row < 0 )
			return -1;
	}

	return row;
}

int TaskTreeView::unSelectedRowAfter(const QModelIndex &index)
{
	if ( !index.isValid() )
		return -1;

	int rows = model()->rowCount(index.parent());

	if ( index.row() >= rows )
		return -1;

	int row = index.row() + 1;

	while ( selectionModel()->isSelected(model()->index(row, index.column(), index.parent())) )
	{
		row++;
		if ( row >= rows )
			return -1;
	}

	return row;
}

void TaskTreeView::addTask()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	if ( indexes.size() == 0 )
	{
		QModelIndex index = selectionModel()->currentIndex();

		if ( !model()->insertRow(index.row() + 1, index.parent()) )
			return;

		index = model()->index(index.row() + 1, 0, index.parent());

		selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
	}
	else
	{
		QItemSelection selection;

		foreach (const QPersistentModelIndex &i, indexes)
			model()->insertRow(i.row() + 1, i.parent());

		QModelIndex next;

		foreach (const QPersistentModelIndex &i, indexes)
		{
			next = model()->index(i.row() + 1, i.column(), i.parent());
			selection.select(next, next);
		}

		selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
		selectionModel()->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect);
	}
}

void TaskTreeView::addSubtask()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	QItemSelection selection;

	foreach (const QPersistentModelIndex &i, indexes)
		model()->insertRow(0, i);

	QModelIndex next;

	foreach (const QPersistentModelIndex &i, indexes)
	{
		next = model()->index(0, i.column(), i);
		selection.select(next, next);
	}

	selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect);
	selectionModel()->setCurrentIndex(next, QItemSelectionModel::ClearAndSelect);
}

void TaskTreeView::deleteTask()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	QPersistentModelIndex beforeFirst;

	if ( indexes.size() > 0 )
	{
		const QPersistentModelIndex &i = indexes.at(0);

		if ( i.row() == 0 )
			beforeFirst = model()->index(i.parent().row(), i.column(), i.parent().parent());
		else
			beforeFirst = model()->index(i.row() - 1, i.column(), i.parent());
	}

	foreach (const QPersistentModelIndex &i, indexes)
		if ( i.isValid() )
			model()->removeRow(i.row(), i.parent());

	selectionModel()->setCurrentIndex(beforeFirst, QItemSelectionModel::ClearAndSelect);
}

void TaskTreeView::taskMoveUp()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	foreach (const QPersistentModelIndex &i, indexes)
	{
		if ( i.isValid() && i.row() != 0 && unSelectedRowBefore(i) != -1 )
			model()->moveRow(i.parent(), i.row(), i.parent(), i.row()-1);
	}
}

void TaskTreeView::taskMoveDown()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	qSort(indexes.begin(), indexes.end());

	for(int j = indexes.size() - 1; j >= 0; j--)
	{
		const QPersistentModelIndex &i = indexes.at(j);
		int rows = model()->rowCount(i.parent());
		int row;

		if ( i.isValid() && i.row() != rows - 1 && (row = unSelectedRowAfter(i)) != -1 )
		{
			QModelIndex next = model()->index(i.row() + 1, i.column(), i.parent());
			model()->moveRow(next.parent(), next.row(), next.parent(), i.row());
		}
	}
}

void TaskTreeView::changeCurrentToSubtask()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	foreach (const QPersistentModelIndex &i, indexes)
	{
		QModelIndex prev;

		if ( i.row() == 0 )
			continue;

		int row = unSelectedRowBefore(i);

		if ( row == -1 )
			continue;

		prev = model()->index(row, i.column(), i.parent());

		if ( !prev.isValid() )
			continue;

		expand(prev);

		model()->moveRows(i.parent(), i.row(), 1, prev, model()->rowCount(prev));
	}
}

void TaskTreeView::changeCurrentToTask()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	qSort(indexes.begin(), indexes.end());

	for(int j = indexes.size() - 1; j >= 0; j--)
	{
		const QPersistentModelIndex &i = indexes.at(j);

		if ( !i.parent().isValid() )
			continue;

		model()->moveRows(i.parent(), i.row(), 1, i.parent().parent(), i.parent().row() + 1);
	}
}

void TaskTreeView::toggleTaskDone()
{
	QList<QPersistentModelIndex> indexes;

	foreach (const QModelIndex &i, selectionModel()->selectedIndexes())
		indexes << i;

	foreach (const QPersistentModelIndex &i, indexes)
	{
		if ( !i.isValid() )
			return;

		model()->setData(i, !model()->data(i, TaskModel::TaskDoneRole).toBool(), TaskModel::TaskDoneRole);
	}
}
