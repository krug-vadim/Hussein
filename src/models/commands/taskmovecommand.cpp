#include "taskmovecommand.h"

#include "../guitaskmodel.h"

#include <QDebug>

TaskMoveCommand::TaskMoveCommand(GuiTaskModel *model, const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationRow)
    : QUndoCommand()
    , _model(model)
    , _sourcePosition(sourceRow)
    , _destinationPosition(destinationRow)
{
	_sourceParentPath      = _model->indexToPath(sourceParent);
	_destinationParentPath = _model->indexToPath(destinationParent);

	setText(QString("move row from %1 to %2").arg(sourceRow).arg(destinationRow));
}

void TaskMoveCommand::redo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stRedo);
	stRedo.beginMacro("move row");

	QModelIndex sourceIndex      = _model->pathToIndex(_sourceParentPath);
	QModelIndex destinationIndex = _model->pathToIndex(_destinationParentPath);

	_model->TaskModel::moveRows(sourceIndex, _sourcePosition, 1, destinationIndex, _destinationPosition);

	stRedo.endMacro();
	_model->setCurrentUndoStack(cur);
}

void TaskMoveCommand::undo()
{
	QUndoStack *cur = _model->currentUndoStack();
	_model->setCurrentUndoStack(&stUndo);
	stUndo.beginMacro("nested move row");

	QModelIndex sourceIndex      = _model->pathToIndex(_sourceParentPath);
	QModelIndex destinationIndex = _model->pathToIndex(_destinationParentPath);

	if ( sourceIndex == destinationIndex )
		_model->TaskModel::moveRows(sourceIndex, _sourcePosition, 1, destinationIndex, _destinationPosition);
	else
		_model->TaskModel::moveRows(destinationIndex, _destinationPosition, 1, sourceIndex, _sourcePosition);

	stUndo.endMacro();
	_model->setCurrentUndoStack(cur);
	stUndo.clear();
	while(stRedo.canUndo()) stRedo.undo();
	stRedo.clear();
}
