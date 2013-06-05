#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QtCore/QAbstractItemModel>

class BasicTask;
class TaskFactory;

class TaskModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		enum
		{
			TaskDoneRole = Qt::UserRole,
			TaskExpandedRole,
		}
		TaskDataRole;

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

		BasicTask *root() const;
		void setRoot(BasicTask *root);

		TaskFactory *taskFactory() const;
		void setTaskFactory(TaskFactory *factory);

		friend class BasicTask;

	private:
		BasicTask *getTask(const QModelIndex &index) const;

		BasicTask *_root;

		TaskFactory *_taskFactory;
};

#endif // TASKMODEL_H
