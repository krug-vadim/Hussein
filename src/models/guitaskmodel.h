#ifndef GUITASKMODEL_H
#define GUITASKMODEL_H

#include "taskmodel.h"

class QUndoStack;

class GuiTaskModel : public TaskModel
{
	Q_OBJECT

	friend class TaskRemoveCommand;

	public:
		explicit GuiTaskModel(QObject *parent = 0);

		QVariant data(const QModelIndex &index, int role) const;

		bool insertRows(int position, int rows, const QModelIndex &parent);
		bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	public slots:
		void undo();
		void redo();

	private:
		QUndoStack *_undoStack;
};

#endif // GUITASKMODEL_H
