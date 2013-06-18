#include "guitaskmodel.h"

#include <QtGui/QFont>
#include <QtGui/QPalette>

#include <QtWidgets/QUndoStack>

#include "../limbs/task.h"

GuiTaskModel::GuiTaskModel(QObject *parent) :
    TaskModel(parent)
{
}

QVariant GuiTaskModel::data(const QModelIndex &index, int role) const
{
	QFont font;
	QPalette palette;
	Task *task;

	if ( !index.isValid() )
		return QVariant();

	task = static_cast<Task *>(index.internalPointer());

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
