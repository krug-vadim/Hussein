#ifndef TASKMOVECOMMAND_H
#define TASKMOVECOMMAND_H

#include <QtWidgets/QUndoCommand>
#include <QtWidgets/QUndoStack>

#include "../taskmodel.h"

class GuiTaskModel;

class TaskMoveCommand : public QUndoCommand
{
	public:
		TaskMoveCommand(GuiTaskModel *model, const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationRow);

		void redo();
		void undo();

	private:
		GuiTaskModel *_model;
		TaskModel::Path _sourceParentPath;
		TaskModel::Path _destinationParentPath;
		int _sourcePosition;
		int _destinationPosition;

		QUndoStack stRedo, stUndo;
};

#endif // TASKMOVECOMMAND_H
