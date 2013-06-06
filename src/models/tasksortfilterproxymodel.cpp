#include "tasksortfilterproxymodel.h"

#include "taskmodel.h"

#include <QDebug>

TaskSortFilterProxyModel::TaskSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
	_showDone = false;
}

bool TaskSortFilterProxyModel::showDone() const
{
	return _showDone;
}

bool TaskSortFilterProxyModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	if ( !sourceModel() )
		return false;

	QModelIndex realSourceParent = mapToSource(sourceParent);
	//int realSourceRow = mapToSource(index(sourceRow, 0, sourceParent)).row();
	QModelIndex realDestinationParent = mapToSource(destinationParent);
	int realDestinationChild = mapToSource(index(destinationChild, 0, destinationParent)).row();

	bool ok = true;

	for(int i = 0; i < count; i++)
	{
		int childRow = sourceRow + i;
		int realSourceRow = mapToSource(index(childRow, 0, sourceParent)).row();

		ok = ok && sourceModel()->moveRows(realSourceParent, realSourceRow, 1, realDestinationParent, realDestinationChild);
	}

	return ok;
}

void TaskSortFilterProxyModel::setShowDone(const bool show)
{
	_showDone = show;
	invalidateFilter();
}

bool TaskSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	bool taskDone = sourceModel()->data(index, TaskModel::TaskDoneRole).toBool();

	if ( !showDone() && taskDone )
		return false;

	return true;
}
