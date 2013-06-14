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

QString TaskSortFilterProxyModel::searchString() const
{
	return _searchString;
}

bool TaskSortFilterProxyModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	if ( !sourceModel() )
		return false;

	QModelIndex realSourceParent = mapToSource(sourceParent);
	//int realSourceRow = mapToSource(index(sourceRow, 0, sourceParent)).row();
	QModelIndex realDestinationParent = mapToSource(destinationParent);

	QModelIndex realDestination = mapToSource(index(destinationChild, 0, destinationParent));
	int realDestinationChild = (realDestination.isValid()) ? realDestination.row()
	                                                       : sourceModel()->rowCount(realDestinationParent);

	bool ok = true;

	for(int i = 0; i < count; i++)
	{
		int childRow = sourceRow + i;
		int realSourceRow = mapToSource(index(childRow, 0, sourceParent)).row();
		//sourceChildsToMove << mapToSource(index(childRow, 0, sourceParent));

		ok = ok && sourceModel()->moveRows(realSourceParent, realSourceRow, 1, realDestinationParent, realDestinationChild);
	}

	return ok;
}

void TaskSortFilterProxyModel::setShowDone(const bool show)
{
	_showDone = show;
	invalidateFilter();
}

void TaskSortFilterProxyModel::setSearchString(const QString &search)
{
	_searchString = search;
	invalidateFilter();
}

bool TaskSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	bool taskDone = sourceModel()->data(index, TaskModel::TaskDoneRole).toBool();
	QString taskDescription = sourceModel()->data(index, TaskModel::TaskDescriptionRole).toString();

	if ( !showDone() && taskDone )
		return false;

	if ( searchString().simplified().isEmpty() )
		return true;

	/*foreach(QString tag, searchTags())
		res = res && task->text().contains(tag);*/

	if ( taskDescription.simplified().contains(searchString().simplified()) )
		return true;

	if ( sourceModel()->hasChildren(index) )
	{
		for(int i = 0; i < sourceModel()->rowCount(index); i++)
		{
			if ( filterAcceptsRow(i, index) )
				return true;
		}
	}

	return false;
}
