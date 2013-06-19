#include "taskinsertcommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskInsertCommand::TaskInsertCommand(GuiTaskModel *model, const TaskSharedPointer &parent, int position)
    : QUndoCommand()
    , _model(model)
    , _parent(parent)
    , _position(position)
{
	_task = TaskSharedPointer(new Task());
	_task->setParent(_parent);

	setText(QString("Insert task to parent %1 at %2")
	        .arg(_parent->description())
	        .arg(position)
	       );
}

void TaskInsertCommand::redo()
{
	if ( _parent.isNull() )
		return;

	qDebug() << "redo";

	_parent->insertSubtask(_task, _position);
}

void TaskInsertCommand::undo()
{
	if ( _parent.isNull() )
		return;

	qDebug() << "undo" << _parent->description() << _parent->subtasks().size() << _position;

	_model->layoutAboutToBeChanged();
	_parent->removeSubtask(_position);
	_model->layoutChanged();
}
