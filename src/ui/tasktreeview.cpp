#include "tasktreeview.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtGui/QPalette>
#include <QtGui/QFont>

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

void TaskTreeView::addTask()
{
	if ( !model() )
		return;

	QModelIndex index = selectionModel()->currentIndex();

	if ( !model()->insertRow(index.row() + 1, index.parent()) )
		return;

	index = model()->index(index.row() + 1, 0, index.parent());

	selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void TaskTreeView::addSubtask()
{
	QModelIndex index = selectionModel()->currentIndex();

	if ( !model()->insertRow(0, index) )
		return;

	index = model()->index(0, 0, index);

	selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
}

void TaskTreeView::deleteTask()
{
	QModelIndex index = selectionModel()->currentIndex();

	if ( index.isValid() )
		model()->removeRow(index.row(), index.parent());
}

void TaskTreeView::taskMoveUp()
{
	QModelIndex index = selectionModel()->currentIndex();

	if ( index.row() == 0 )
		return;

	model()->moveRow(index.parent(), index.row(), index.parent(), index.row()-1);
}

void TaskTreeView::taskMoveDown()
{
	QModelIndex index = selectionModel()->currentIndex();

	if ( index.row() == model()->rowCount(index.parent()) - 1 )
		return;

	QModelIndex next = model()->index(index.row() + 1, index.column(), index.parent());

	model()->moveRow(next.parent(), next.row(), next.parent(), next.row()-1);
}

void TaskTreeView::changeCurrentToSubtask()
{
	QModelIndex index = selectionModel()->currentIndex();
	QModelIndex prev;

	if ( index.row() == 0 )
		return;

	prev = model()->index(index.row() - 1, index.column(), index.parent());

	if ( !prev.isValid() )
		return;

	if ( !model()->moveRows(index.parent(), index.row(), 1, prev, model()->rowCount(prev)) )
		return;

	selectionModel()->setCurrentIndex(model()->index(model()->rowCount(prev) - 1, 0, prev), QItemSelectionModel::ClearAndSelect);

	expand(prev);
}

void TaskTreeView::changeCurrentToTask()
{
	QModelIndex index = selectionModel()->currentIndex();

	if ( !index.parent().isValid() )
		return;

	model()->moveRows(index.parent(), index.row(), 1, index.parent().parent(), index.parent().row() + 1);
}

void TaskTreeView::toggleTaskDone()
{
}
