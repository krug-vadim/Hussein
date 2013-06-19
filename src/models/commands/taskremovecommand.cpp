#include "taskremovecommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskRemoveCommand::TaskRemoveCommand(GuiTaskModel *model, const QModelIndex &parent, int position)
    : QUndoCommand()
    , _model(model)
    , _position(position)
{
	_path = _model->indexToPath(parent);

	// TODO: change description to more verbose
	setText(QString("Remove task"));
}

void TaskRemoveCommand::redo()
{
	QModelIndex parent = _model->pathToIndex(_path);
	QModelIndex taskIndex = (parent.isValid()) ? parent.child(_position, 0) : _model->index(_position, 0);

	_task = _model->getTask(taskIndex);

	_model->TaskModel::removeRows(_position, 1, parent);
}

void TaskRemoveCommand::undo()
{
	QModelIndex parent = _model->pathToIndex(_path);
	_model->TaskModel::insertRow(_task, _position, parent);
	_task.clear();
}
