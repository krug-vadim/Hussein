#ifndef TASKINSERTCOMMAND_H
#define TASKINSERTCOMMAND_H

#include <QtWidgets/QUndoCommand>
#include <QtWidgets/QUndoStack>

#include "../taskmodel.h"

class GuiTaskModel;

class TaskInsertCommand : public QUndoCommand
{
	public:
		TaskInsertCommand(GuiTaskModel *model, const QModelIndex &parent, int position);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskModel::Path _path;
		int _position;

		QUndoStack stRedo, stUndo;
};

#endif // TASKINSERTCOMMAND_H
