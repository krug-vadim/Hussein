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
	else
		return sourceModel()->moveRows(sourceParent, sourceRow, count, destinationParent, destinationChild);
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
