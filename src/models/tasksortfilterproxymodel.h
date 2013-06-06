#ifndef TASKSORTFILTERPROXYMODEL_H
#define TASKSORTFILTERPROXYMODEL_H

#include <QtCore/QSortFilterProxyModel>

class TaskSortFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

	public:
		explicit TaskSortFilterProxyModel(QObject *parent = 0);

		bool showDone() const;

		bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild);

	public slots:
		void setShowDone(const bool show);

	protected:
		//bool filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const;
		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

	private:
		bool _showDone;

};

#endif // TASKSORTFILTERPROXYMODEL_H
