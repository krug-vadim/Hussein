#ifndef CHANGEVALUECOMMAND_H
#define CHANGEVALUECOMMAND_H

#include <QtWidgets/QUndoCommand>

#include <QtCore/QVariant>
#include <QtCore/QPersistentModelIndex>

class QAbstractItemModel;

class ChangeValueCommand : public QUndoCommand
{
	public:
		explicit ChangeValueCommand(const QModelIndex &index, const QVariant &value, const int role, QAbstractItemModel *model);

		void redo();
		void undo();

	private:
		QVariant _oldValue;
		QVariant _newValue;
		QPersistentModelIndex parent;

		int _row, _col;
		int _role;
		QAbstractItemModel *_model;
};

#endif // CHANGEVALUECOMMAND_H
