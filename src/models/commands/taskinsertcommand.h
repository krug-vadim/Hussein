#ifndef TASKINSERTCOMMAND_H
#define TASKINSERTCOMMAND_H

#include <QtWidgets/QUndoCommand>

#include "../../limbs/task.h"

class GuiTaskModel;

class TaskInsertCommand : public QUndoCommand
{
	public:
		TaskInsertCommand(GuiTaskModel *model, const TaskSharedPointer &parent, int position);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskSharedPointer _parent;
		int _position;
};

#endif // TASKINSERTCOMMAND_H
