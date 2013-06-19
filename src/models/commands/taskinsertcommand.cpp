#include "taskinsertcommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskInsertCommand::TaskInsertCommand(GuiTaskModel *model, const TaskSharedPointer &parent, int position)
    : QUndoCommand()
    , _model(model)
    , _parent(parent)
    , _position(position)
{
	setText(QString("Insert task to parent %1 at %2")
	        .arg(_parent->description())
	        .arg(position)
	       );
}

void TaskInsertCommand::redo()
{
	if ( _parent.isNull() )
		return;

	TaskSharedPointer task(new Task());

	task->setParent(_parent);

	_parent->insertSubtask(task, _position);
}

void TaskInsertCommand::undo()
{
	if ( _parent.isNull() )
		return;

	_model->layoutAboutToBeChanged();
	_parent->removeSubtask(_position);
	_model->layoutChanged();
}
