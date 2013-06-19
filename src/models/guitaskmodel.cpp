#include "guitaskmodel.h"

#include <QtGui/QFont>
#include <QtGui/QPalette>

#include <QtWidgets/QUndoStack>
#include <QtWidgets/QUndoView>

#include "../limbs/task.h"

#include "commands/taskinsertcommand.h"
#include "commands/taskremovecommand.h"

#include <QDebug>

GuiTaskModel::GuiTaskModel(QObject *parent) :
    TaskModel(parent)
{
	_undoStack = new QUndoStack(this);
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

	TaskSharedPointer parentTask = getTask(parent);

	beginInsertRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
		_undoStack->push(new TaskInsertCommand(this, parentTask, position));
	endInsertRows();

	return success;
}

bool GuiTaskModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TaskSharedPointer parentTask = getTask(parent);

	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
	{
		_undoStack->push(new TaskRemoveCommand(this, parentTask->subtasks().at(position)));
		//success &= parentTask->removeSubtask(position);
	}
	endRemoveRows();

	return success;
}

void GuiTaskModel::undo()
{
	qDebug() << "undo" << _undoStack->undoText();
	_undoStack->undo();
}

void GuiTaskModel::redo()
{
	qDebug() << "redo";
	_undoStack->redo();
}
