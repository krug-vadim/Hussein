#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QtCore/QAbstractItemModel>
#include "../limbs/task.h"

class TaskModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		enum TaskRoles
		{
			ExpandedRole = Qt::UserRole + 1
		};

		typedef QList<int> Path;

		explicit TaskModel(QObject *parent = 0);
		~TaskModel();

		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;

		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
		bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
		bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
		bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild);

		// mime
		Qt::DropActions supportedDropActions() const;
		QStringList mimeTypes() const;
		QMimeData *mimeData(const QModelIndexList &indexes) const;
		bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

		Q_INVOKABLE const TaskSharedPointer &root() const;

		//
		Q_INVOKABLE bool insertTaskRow(const TaskSharedPointer &task, int position, const QModelIndex &parent = QModelIndex());

		/*Q_INVOKABLE void tasksAboutToBeReseted();
		Q_INVOKABLE void tasksReseted();*/

		Q_INVOKABLE bool loadTasklist(const QString &fileName);
		Q_INVOKABLE bool saveTasklist(const QString &fileName);

		Q_INVOKABLE QModelIndex pathToIndex(const TaskModel::Path &path) const;
		Q_INVOKABLE TaskModel::Path indexToPath(const QModelIndex &index);

		Q_INVOKABLE QHash<int, QByteArray> roleNames() const;

	protected:
		TaskSharedPointer getTask(const QModelIndex &index) const;

	private:
		TaskSharedPointer _root;
};

#endif // TASKMODEL_H
