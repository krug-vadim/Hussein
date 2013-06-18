#include "task.h"

#include <QDebug>

Task::Task()
{
	clear();
}

Task::~Task()
{
	clear();
}

TaskSharedPointer Task::parent() const
{
	return _parent;
}

void Task::setParent(const TaskSharedPointer &parent)
{
	_parent = parent;
}

const QString &Task::description() const
{
	return _description;
}

void Task::setDescription(const QString &description)
{
	_description = description;
}

bool Task::isDone() const
{
	return _done;
}

bool Task::isAboveDone() const
{
	if ( isDone() )
		return true;

	TaskSharedPointer p = parent();

	while ( !p.isNull() )
	{
		if ( p->isDone() )
			return true;
		p = p->parent();
	}

	return false;
}

void Task::setDone(const bool done)
{
	if ( _done == done )
		return;

	_done = done;
}

bool Task::isExpanded() const
{
	return _expanded;
}

void Task::setExpanded(const bool expanded)
{
	if ( _expanded == expanded )
		return;

	_expanded = expanded;
}

void Task::clear()
{
	_done = false;
	_expanded = true;

	qDeleteAll(_subtasks);
	_subtasks.clear();
}

const TaskList &Task::subtasks() const
{
	return _subtasks;
}

bool Task::appendSubtask(const TaskSharedPointer &task)
{
	if ( !task )
		return false;

	task->setParent(this);
	_subtasks.append(task);

	return true;
}

bool Task::insertSubtask(const TaskSharedPointer &task, int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	if ( !task )
		return false;

	task->setParent(this);
	_subtasks.insert(position, task);

	return true;
}

bool Task::removeSubtask(int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	_subtasks.at(position)->setParent(0);
	_subtasks.removeAt(position);

	return true;
}

int Task::row() const
{
	if ( !parent() )
		return -1;

	return parent()->subtasks().indexOf( const_cast<Task *>(this) );
}

void Task::getPath(QList<int> &path)
{
	path.clear();

	for(Task *current = this; current->row() != -1; current = current->parent())
		path.append(current->row());
}

QVariant Task::data(int role) const
{
	switch ( role )
	{
		case TaskDescriptionRole:
			return description();
			break;

		case TaskDoneRole:
			return isDone();
			break;

		case TaskExpandRole:
			return isExpanded();
			break;
	}

	return QVariant();
}

bool Task::setData(const QVariant &value, int role)
{
	switch ( role )
	{
		case TaskDescriptionRole:
			setDescription(value.toString());
			break;

		case TaskDoneRole:
			setDone(value.toBool());
			break;

		case TaskExpandRole:
			setExpanded(value.toBool());
			break;

		default:
			return false;
			break;
	}

	return true;
}
