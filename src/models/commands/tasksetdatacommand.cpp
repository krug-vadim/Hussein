#include "tasksetdatacommand.h"

#include "../guitaskmodel.h"

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
	QModelIndex index = _model->pathToIndex(_path);
	_model->TaskModel::setData(index, _newValue, _role);
}

void TaskSetDataCommand::undo()
{
	QModelIndex index = _model->pathToIndex(_path);
	_model->TaskModel::setData(index, _oldValue, _role);
}
