#ifndef TASKREMOVECOMMAND_H
#define TASKREMOVECOMMAND_H

#include <QtWidgets/QUndoCommand>

#include "../../limbs/task.h"

class GuiTaskModel;

class TaskRemoveCommand : public QUndoCommand
{
	public:
		explicit TaskRemoveCommand(GuiTaskModel *model, const TaskSharedPointer &task);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskSharedPointer _task;
		int _position;
};

#endif // TASKREMOVECOMMAND_H
