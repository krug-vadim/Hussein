#include "taskinsertcommand.h"

#include "../guitaskmodel.h"

TaskInsertCommand::TaskInsertCommand(GuiTaskModel *model, const QModelIndex &parent, int position)
    : QUndoCommand()
    , _model(model)
    , _position(position)
{
	_path = _model->indexToPath(parent);

	// TODO: change description to more verbose
	setText(QString("Insert new task"));
}

void TaskInsertCommand::redo()
{
	QModelIndex parent = _model->pathToIndex(_path);

	_model->TaskModel::insertRows(_position, 1, parent);
}

void TaskInsertCommand::undo()
{
	QModelIndex parent = _model->pathToIndex(_path);
	_model->TaskModel::removeRows(_position, 1, parent);
}
