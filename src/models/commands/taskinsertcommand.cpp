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
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stRedo);
	stRedo.beginMacro("insert row");

	QModelIndex parent = _model->pathToIndex(_path);

	_model->TaskModel::insertRows(_position, 1, parent);

	stRedo.endMacro();
	_model->setCurrentUndoStack(cur);
}

void TaskInsertCommand::undo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stUndo);
	stUndo.beginMacro("nested move row");

	QModelIndex parent = _model->pathToIndex(_path);
	_model->TaskModel::removeRows(_position, 1, parent);

	stUndo.endMacro();
	_model->setCurrentUndoStack(cur);
	stUndo.clear();
	while(stRedo.canUndo()) stRedo.undo();
	stRedo.clear();
}
