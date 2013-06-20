#include "taskmodel.h"

#include "../serialization/yamlserialization.h"

#include <QtCore/QMimeData>
#include <QtCore/QStringList>

#include <QDebug>

TaskModel::TaskModel(QObject *parent) :
    QAbstractItemModel(parent)
{
	_root = TaskSharedPointer(new Task());
	_root->setParent(TaskSharedPointer());
	_root->setDescription(tr("(root)"));
}

TaskModel::~TaskModel()
{
}

QModelIndex TaskModel::index(int row, int column, const QModelIndex &parent) const
{
	if ( !hasIndex(row, column, parent) )
		return QModelIndex();

	return createIndex(row, column, (void *)&getTask(parent)->subtasks().at(row));
}

QModelIndex TaskModel::parent(const QModelIndex &index) const
{
	if ( !index.isValid() )
		return QModelIndex();

	TaskWeakPointer parent = getTask(index)->parent();

	if ( parent.isNull() )
		return QModelIndex();

	if ( parent == _root )
		return QModelIndex();

	TaskWeakPointer grandParent = parent.toStrongRef()->parent();
	int parentRow = grandParent.toStrongRef()->subtasks().indexOf(parent);

	if ( parentRow == - 1 )
		return QModelIndex();

	return createIndex(parentRow, 0, (void *)&grandParent.toStrongRef()->subtasks().at(parentRow));
}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
	TaskSharedPointer task;

	if ( !index.isValid() )
		return QVariant();

	task = getTask(index);

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

		default:
			return task->data(role);
			break;
	}

	return QVariant();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
	if ( !index.isValid() )
		return 0;

	return Qt::ItemIsEditable
	     | Qt::ItemIsEnabled
	     | Qt::ItemIsSelectable
	     //| Qt::ItemIsDragEnabled
	     //| Qt::ItemIsDropEnabled
	     ;
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
	return getTask(parent)->subtasks().size();
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if ( role == Qt::EditRole )
		role = Task::TaskDescriptionRole;

	bool res = getTask(index)->setData(value, role);

	if ( res )
		emit dataChanged(index, index);

	return res;
}

bool TaskModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	bool success = true;

	qDebug() << "TaskModel::insertRows" << position << rows << parent;

	TaskSharedPointer parentTask = getTask(parent);

	beginInsertRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
	{
		TaskSharedPointer task(new Task());

		task->setParent(parentTask);
		success &= parentTask->insertSubtask(task, position);
	}
	endInsertRows();

	return success;
}

bool TaskModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TaskSharedPointer parentTask = getTask(parent);

	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	for(int i = 0; i < rows; i++)
		success &= parentTask->removeSubtask(position);
	endRemoveRows();

	return success;
}

bool TaskModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
	TaskSharedPointer fromTask = getTask(sourceParent);
	TaskSharedPointer toTask = getTask(destinationParent);

	if ( !fromTask || !toTask )
		return false;

	bool success = true;

	qDebug() << "move rows" << fromTask->description() << toTask->description();

	if ( !beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationChild) )
		return false;

	for(int i = 0; i < count; i++, sourceRow++, destinationChild++)
	{
		TaskSharedPointer current;

		current = fromTask->subtasks().at(sourceRow);
		fromTask->removeSubtask(sourceRow);

		current->setParent(toTask);
		success &= toTask->insertSubtask(current, destinationChild);
	}

	endMoveRows();

	qDebug() << "move end";

	return success;
}

const TaskSharedPointer &TaskModel::root() const
{
	return _root;
}

bool TaskModel::insertRow(const TaskSharedPointer &task, int position, const QModelIndex &parent)
{
	bool success = true;

	TaskSharedPointer parentTask = getTask(parent);

	beginInsertRows(parent, position, position);

	task->setParent(parentTask);
	success &= parentTask->insertSubtask(task, position);

	endInsertRows();

	return success;
}

Qt::DropActions TaskModel::supportedDropActions() const
{
	return Qt::MoveAction;
}

QStringList TaskModel::mimeTypes() const
{
	return QStringList() << "text/plain";
}

QMimeData *TaskModel::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly|QIODevice::Text);

	foreach (const QModelIndex &index, indexes)
	{
		if (index.isValid())
		{
			QString text = data(index, Qt::DisplayRole).toString();
			stream << text;
		}
	}

	mimeData->setData("text/plain", encodedData);
	return mimeData;
}

bool TaskModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if ( action == Qt::IgnoreAction )
		return true;

	if ( action != Qt::MoveAction )
		return false;

	if (!data->hasFormat("text/plain"))
		return false;

	if (column > 0)
		return false;

	//qDebug() << "No hope";
	return false;
}

void TaskModel::taskDataChanged(const QList<int> &path)
{
	QModelIndex index = pathToIndex(path);
	emit dataChanged(index, index);
}

QModelIndex TaskModel::pathToIndex(const Path &path) const
{
	QModelIndex iter;

	foreach(int row, path)
		iter = index(row, 0, iter);

	return iter;
}

TaskModel::Path TaskModel::indexToPath(const QModelIndex &index)
{
	Path path;
	QModelIndex p;

	p = index;

	while ( p.isValid() )
	{
		path.push_front(p.row());
		p = p.parent();
	}

	return path;
}

TaskSharedPointer TaskModel::getTask(const QModelIndex &index) const
{
	return ( index.isValid() ) ? *static_cast<TaskSharedPointer *>(index.internalPointer()) : _root;
}

bool TaskModel::loadTasklist(const QString &fileName)
{
	if ( fileName.isEmpty() )
		return false;

	beginResetModel();

	_root->clear();
	bool res = YamlSerialization::deserialize(fileName, _root);

	endResetModel();

	return res;
}

bool TaskModel::saveTasklist(const QString &fileName)
{
	if ( fileName.isEmpty() )
		return false;

	return YamlSerialization::serialize(fileName, _root);
}
