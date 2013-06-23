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
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stRedo);
	stRedo.beginMacro("remove row");

	QModelIndex parent = _model->pathToIndex(_path);
	QModelIndex taskIndex = (parent.isValid()) ? parent.child(_position, 0) : _model->index(_position, 0);

	_task = _model->getTask(taskIndex);

	_model->TaskModel::removeRows(_position, 1, parent);

	stRedo.endMacro();
	_model->setCurrentUndoStack(cur);
}

void TaskRemoveCommand::undo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stUndo);
	stUndo.beginMacro("nested move row");

	QModelIndex parent = _model->pathToIndex(_path);
	_model->TaskModel::insertRow(_task, _position, parent);
	_task.clear();

	stUndo.endMacro();
	_model->setCurrentUndoStack(cur);
	stUndo.clear();
	while(stRedo.canUndo()) stRedo.undo();
	stRedo.clear();
}
