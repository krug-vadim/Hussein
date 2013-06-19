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

		virtual bool insertRows(int position, int rows, const QModelIndex &parent);
		virtual bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild);
		virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

		virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	public slots:
		void undo();
		void redo();

	private:
		QUndoStack *_undoStack;
};

#endif // GUITASKMODEL_H
