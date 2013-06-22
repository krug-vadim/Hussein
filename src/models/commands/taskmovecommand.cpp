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
	QModelIndex sourceIndex      = _model->pathToIndex(_sourceParentPath);
	QModelIndex destinationIndex = _model->pathToIndex(_destinationParentPath);

	qDebug() << "taskmove redo" << sourceIndex << destinationIndex;
	qDebug() << _sourceParentPath << _destinationParentPath;
	qDebug() << _sourcePosition << _destinationPosition;
	qDebug() << "taskmove redo moving";

	_model->TaskModel::moveRows(sourceIndex, _sourcePosition, 1, destinationIndex, _destinationPosition);
}

void TaskMoveCommand::undo()
{
	QModelIndex sourceIndex      = _model->pathToIndex(_sourceParentPath);
	QModelIndex destinationIndex = _model->pathToIndex(_destinationParentPath);

	qDebug() << "taskmove undo" << sourceIndex << destinationIndex;
	qDebug() << _sourceParentPath << _destinationParentPath;
	qDebug() << _sourcePosition << _destinationPosition;
	qDebug() << "taskmove redo moving";

	_model->TaskModel::moveRows(destinationIndex, _destinationPosition, 1, sourceIndex, _sourcePosition);

}
