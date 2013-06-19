#ifndef TASKREMOVECOMMAND_H
#define TASKREMOVECOMMAND_H

#include <QtWidgets/QUndoCommand>

#include "../../limbs/task.h"
#include "../taskmodel.h"

class GuiTaskModel;

class TaskRemoveCommand : public QUndoCommand
{
	public:
		TaskRemoveCommand(GuiTaskModel *model, const QModelIndex &parent, int position);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskModel::Path _path;
		int _position;
		TaskSharedPointer _task;
};

#endif // TASKREMOVECOMMAND_H
