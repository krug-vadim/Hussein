#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QtCore/QAbstractItemModel>
#include "../limbs/task.h"

class TaskModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
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

		TaskSharedPointer root() const;

		void tasksAboutToBeReseted();
		void tasksReseted();

		bool loadTasklist(const QString &fileName);
		bool saveTasklist(const QString &fileName);

	public slots:
		void taskDataChanged(const QList<int> &path);

	private:
		QModelIndex pathToIndex(const QList<int> &path) const;

		TaskSharedPointer getTask(const QModelIndex &index) const;

		TaskSharedPointer _root;
};

#endif // TASKMODEL_H
