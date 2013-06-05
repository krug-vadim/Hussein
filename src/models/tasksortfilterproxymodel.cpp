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

void TaskSortFilterProxyModel::setShowDone(const bool show)
{
	qDebug() << show;
	_showDone = show;
	invalidateFilter();
}

bool TaskSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	qDebug() << "filtering";

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	bool taskDone = sourceModel()->data(index, TaskModel::TaskDoneRole).toBool();

	if ( !showDone() && taskDone )
		return false;

	return true;
}
