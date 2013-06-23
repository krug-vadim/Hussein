#ifndef TASKSETDATACOMMAND_H
#define TASKSETDATACOMMAND_H

#include <QtWidgets/QUndoCommand>
#include <QtWidgets/QUndoStack>

#include "../../limbs/task.h"
#include "../taskmodel.h"

class GuiTaskModel;

class TaskSetDataCommand : public QUndoCommand
{
	public:
		TaskSetDataCommand(GuiTaskModel *model, const QModelIndex &index, QVariant value, int role);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskModel::Path _path;
		QVariant _oldValue;
		QVariant _newValue;
		int _role;

		QUndoStack stRedo, stUndo;
};

#endif // TASKSETDATACOMMAND_H
