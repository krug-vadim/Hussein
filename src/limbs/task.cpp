#include "task.h"

#include <QDebug>

Task::Task()
{
	clear();
}

Task::~Task()
{
	qDebug() << "task" << description() << "deleted";
	clear();
}

TaskWeakPointer Task::parent() const
{
	return _parent;
}

void Task::setParent(const TaskWeakPointer &parent)
{
	_parent = parent;
}

const QString &Task::description() const
{
	return _description;
}

void Task::setDescription(const QString &description)
{
	if ( _description == description )
		return;

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

	while( !_subtasks.isEmpty() )
		_subtasks.takeFirst().clear();
}

const TaskList &Task::subtasks() const
{
	return _subtasks;
}

bool Task::appendSubtask(const TaskSharedPointer &task)
{
	if ( task.isNull() )
		return false;

	_subtasks.append(task);

	return true;
}

bool Task::insertSubtask(const TaskSharedPointer &task, int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	if ( task.isNull() )
		return false;

	_subtasks.insert(position, task);

	return true;
}

bool Task::removeSubtask(int position)
{
	if ( position < 0 || position > subtasks().size() )
		return false;

	_subtasks.removeAt(position);

	return true;
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
