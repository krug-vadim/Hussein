#include "tasksetdatacommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskSetDataCommand::TaskSetDataCommand(GuiTaskModel *model, const QModelIndex &index, QVariant value, int role)
    : QUndoCommand()
    , _model(model)
    , _newValue(value)
    , _role(role)
{
	_oldValue = _model->data(index, role);
	_path = _model->indexToPath(index);

	setText(QString("setData role %1").arg(_role));
}

void TaskSetDataCommand::redo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stRedo);
	stRedo.beginMacro("set data");

	QModelIndex index = _model->pathToIndex(_path);
	_model->TaskModel::setData(index, _newValue, _role);

	qDebug() << "set data redo to " << index << _role;

	stRedo.endMacro();
	_model->setCurrentUndoStack(cur);
}

void TaskSetDataCommand::undo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stUndo);
	stUndo.beginMacro("nested move row");

	QModelIndex index = _model->pathToIndex(_path);
	_model->TaskModel::setData(index, _oldValue, _role);

	qDebug() << "set data undo to " << index << _role;

	stUndo.endMacro();
	_model->setCurrentUndoStack(cur);
	stUndo.clear();
	while(stRedo.canUndo()) stRedo.undo();
	stRedo.clear();
}
