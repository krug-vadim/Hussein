#include "taskremovecommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskRemoveCommand::TaskRemoveCommand(GuiTaskModel *model, const TaskSharedPointer &task)
    : QUndoCommand()
    , _model(model)
    , _task(task)
{
	setText(QString("Remove task «%1»").arg(_task->description()));
}

void TaskRemoveCommand::redo()
{
	if ( _task->parent().isNull() )
		return;

	_position = _task->parent().toStrongRef()->subtasks().indexOf(_task);
	_task->parent().toStrongRef()->removeSubtask(_position);
}

void TaskRemoveCommand::undo()
{
	if ( _task->parent().isNull() )
		return;

	_model->layoutAboutToBeChanged();

	_task->parent().toStrongRef()->insertSubtask(_task, _position);

	_model->layoutChanged();
}
