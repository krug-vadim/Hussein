#ifndef GUITASKMODEL_H
#define GUITASKMODEL_H

#include <QtCore/QItemSelection>

#include "taskmodel.h"

class QUndoStack;

class GuiTaskModel : public TaskModel
{
	Q_OBJECT

	friend class TaskInsertCommand;
	friend class TaskMoveCommand;
	friend class TaskRemoveCommand;
	friend class TaskSetDataCommand;

	public:
		explicit GuiTaskModel(QObject *parent = 0);

		QVariant data(const QModelIndex &index, int role) const;

		Q_INVOKABLE virtual bool insertRows(int position, int rows, const QModelIndex &parent);
		Q_INVOKABLE virtual bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild);
		Q_INVOKABLE virtual bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

		Q_INVOKABLE virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

		Q_INVOKABLE bool insertRow(int row, const QModelIndex & parent = QModelIndex());

		Q_INVOKABLE QPersistentModelIndex createPersistentModelIndex(const QModelIndex &index) const;
		Q_INVOKABLE QItemSelection createItemSelection() const;
		Q_INVOKABLE QItemSelection itemSelectionSelect(QItemSelection selection, const QModelIndex &topLeft, const QModelIndex &bottomRight) const;

	public slots:
		void undo();
		void redo();

	private:
		QUndoStack *currentUndoStack() const;
		void setCurrentUndoStack(QUndoStack *stack);

		QUndoStack *_undoStack;
};

#endif // GUITASKMODEL_H
