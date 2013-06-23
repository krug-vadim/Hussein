#include "guitaskmodel.h"

#include <QtGui/QFont>
#include <QtGui/QPalette>

#include <QtWidgets/QUndoStack>
#include <QtWidgets/QUndoView>

#include "../limbs/task.h"

#include "commands/taskinsertcommand.h"
#include "commands/taskmovecommand.h"
#include "commands/taskremovecommand.h"
#include "commands/tasksetdatacommand.h"

#include <QDebug>

GuiTaskModel::GuiTaskModel(QObject *parent) :
    TaskModel(parent)
{
	_undoStack = new QUndoStack(this);
	_undoStack->clear();
}

QVariant GuiTaskModel::data(const QModelIndex &index, int role) const
{
	QFont font;
	QPalette palette;
	TaskSharedPointer task;

	if ( !index.isValid() )
		return QVariant();

	task = getTask(index);

	switch ( role )
	{
		case Qt::ForegroundRole:
			if ( index.column() == 0 && task->description().isEmpty() )
				return palette.color(QPalette::Disabled, QPalette::Text);
			if ( index.column() == 0 && task->isAboveDone() )
				return palette.color(QPalette::Disabled, QPalette::Text);
			break;

		case Qt::FontRole:
			if ( index.column() == 0 && task->isDone())
			{
				font.setStrikeOut(true);
				return font;
			}
			break;
	}

	return TaskModel::data(index, role);
}

bool GuiTaskModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	bool success = true;

	for(int i = 0; i < rows; i++)
		_undoStack->push(new TaskInsertCommand(this, parent, position));

	return success;
}

bool GuiTaskModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	bool success = true;

	for(int i = 0; i < count; i++)
	{
		_undoStack->beginMacro("dsd");
		_undoStack->push(new TaskMoveCommand(this, sourceParent, sourceRow, destinationParent, destinationChild));
		_undoStack->endMacro();
	}

	return success;

}

bool GuiTaskModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	bool success = true;

	for(int i = 0; i < rows; i++)
		_undoStack->push(new TaskRemoveCommand(this, parent, position));

	return success;
}

bool GuiTaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	_undoStack->push(new TaskSetDataCommand(this, index, value, role));
	return true;
}

void GuiTaskModel::undo()
{
	if ( !_undoStack->canUndo() )
		return;

	_undoStack->undo();
}

void GuiTaskModel::redo()
{
	if ( !_undoStack->canRedo() )
		return;

	_undoStack->redo();
}

QUndoStack *GuiTaskModel::currentUndoStack() const
{
	return _undoStack;
}

void GuiTaskModel::setCurrentUndoStack(QUndoStack *stack)
{
	_undoStack = stack;
}
