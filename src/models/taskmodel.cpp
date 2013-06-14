#include "taskmodel.h"

#include "../limbs/task.h"
#include "../limbs/taskfactory.h"

#include <QtGui/QFont>
#include <QtGui/QPalette>

#include <QDebug>

TaskModel::TaskModel(QObject *parent) :
    QAbstractItemModel(parent)
{
	_root = 0;
}

TaskModel::~TaskModel()
{
}

QModelIndex TaskModel::index(int row, int column, const QModelIndex &parent) const
{
	if ( !hasIndex(row, column, parent) )
		return QModelIndex();

	Task *parentTask = getTask(parent);

	if ( !parentTask )
		return QModelIndex();

	if ( row < parentTask->subtasks().size() )
		return createIndex(row, column, (void *)parentTask->subtasks().at(row));
	else
		return QModelIndex();
}

QModelIndex TaskModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	Task *subTask;
	Task *parentTask;

	subTask = getTask(index);
	parentTask = subTask->parent();

	if ( !parentTask )
		return QModelIndex();

	if ( parentTask == _root )
		return QModelIndex();

	/*if ( subtask == parent )
		return QModelIndex();*/

	if ( parentTask->parent() )
		return createIndex(parentTask->parent()->subtasks().indexOf(parentTask), 0, parentTask);
	else
		return createIndex(0, 0, parentTask);
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
	QFont font;
	QPalette palette;
	Task *task;

	if ( !index.isValid() )
		return QVariant();

	task = static_cast<Task *>(index.internalPointer());

	switch ( role )
	{
		case Qt::DisplayRole:
			if ( index.column() == 0 )
				return ( task->description().isEmpty() ) ? tr("(empty)") : task->description();
			break;

		case Qt::EditRole:
			if ( index.column() == 0 )
				return task->description();
			break;

		case Qt::ForegroundRole:
			if ( index.column() == 0 && task->description().isEmpty() )
				return palette.color(QPalette::Disabled, QPalette::Text);
			if ( index.column() == 0 && task->isAboveDone() )
				return palette.color(QPalette::Disabled, QPalette::Text);
			break;

		case Qt::FontRole:
			if ( index.column() == 0 && task->isDone())
			{
				font.setStrikeOut(true);
				return font;
			}
			break;

		case TaskModel::TaskDescriptionRole:
			return task->description();
			break;

		case TaskModel::TaskDoneRole:
			return task->isDone();
			break;

		case TaskModel::TaskExpandedRole:
			return task->isExpanded();
			break;

	}

	return QVariant();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
	if ( !index.isValid() )
		return 0;

	return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch ( section )
		{
			case 0 : return tr("Task"); break;
			default: return QVariant(); break;
		}
	}

	return QVariant();
}

int TaskModel::rowCount(const QModelIndex &parent) const
{
	Task *parentTask;

	parentTask = getTask(parent);

	return parentTask->subtasks().size();
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
	if ( getTask(parent) )
		return 1;//return getItem(parent)->columnCount();
	else
		return 0;
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	bool result;
	QModelIndex nextIndex;
	Task *task;

	nextIndex = index;

	task = getTask(index);

	if ( !task )
		return false;

	switch ( role )
	{
		case Qt::EditRole:
		case TaskModel::TaskDescriptionRole:
			//result = item->setData(index.column(), value);
			task->setDescription(value.toString());
			result = true;
			break;

		case TaskDoneRole:
			task->setDone(value.toBool());
			//nextIndex = this->index(index.row() + 1, index.column(), index.parent());
			result = true;
			break;

		case TaskExpandedRole:
			task->setExpanded(value.toBool());
			result = true;
			break;

		/*case TaskDoneToggleRole:
			item->setDone(!item->isDone());
			result = true;
			break;*/

		default:
			result = false;
			break;
	}

	if ( result )
		emit dataChanged(index, nextIndex);

	return result;
}

bool TaskModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	Task *parentTask = getTask(parent);

	if ( !parentTask )
		return false;

	if ( !taskFactory() )
		return false;

	bool success = true;

	beginInsertRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
		success &= parentTask->insertSubtask(taskFactory()->create(), position);
	endInsertRows();

	return success;
}

bool TaskModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	Task *parentTask = getTask(parent);

	if ( !parentTask )
		return false;

	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
		success &= parentTask->removeSubtask(position);
	endRemoveRows();

	return success;
}

bool TaskModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	Task *fromTask = getTask(sourceParent);
	Task *toTask = getTask(destinationParent);

	if ( !fromTask || !toTask )
		return false;

	bool success = true;

	if ( !beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild) )
		return false;

	for(int i = 0; i < count; i++, sourceRow++, destinationChild++)
	{
		Task *current;

		current = fromTask->subtasks().at(sourceRow);
		fromTask->removeSubtask(sourceRow);

		success &= toTask->insertSubtask(current, destinationChild);
	}

	endMoveRows();

	return success;
}

Task *TaskModel::root() const
{
	return _root;
}

void TaskModel::setRoot(Task *root)
{
	beginResetModel();
	_root = root;
	endResetModel();
}

TaskFactory *TaskModel::taskFactory() const
{
	return _taskFactory;
}

void TaskModel::setTaskFactory(TaskFactory *factory)
{
	_taskFactory = factory;
}

void TaskModel::tasksAboutToBeReseted()
{
	emit beginResetModel();
}

void TaskModel::tasksReseted()
{
	emit endResetModel();
}

void TaskModel::taskDataChanged(const QList<int> &path)
{
	QModelIndex index = pathToIndex(path);
	emit dataChanged(index, index);
}

QModelIndex TaskModel::pathToIndex(const QList<int> &path) const
{
	if ( path.isEmpty() )
		return QModelIndex();

	QModelIndex index = this->index(path.last(), 0);

	for(int i = path.size() - 1; i > 0; i--)
		index = index.child(path.at(i-1), 0);

	return index;
}

Task *TaskModel::getTask(const QModelIndex &index) const
{
	if ( index.isValid() )
		return static_cast<Task *>(index.internalPointer());
	else
		return _root;
}
